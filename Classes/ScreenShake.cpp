/// Copyright (c) 2019 Joni Louhela
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to
/// deal in the Software without restriction, including without limitation the
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.

#include "ScreenShake.h"

namespace {

// Perlin noise magic: https://stackoverflow.com/questions/16569660/2d-perlin-noise-in-c
float noise(int x, int y)
{
    int n = x + y * 57;
    n = (n << 13) ^ n;
    return (1.0 - ((n * ((n * n * 15731) + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}
}  // namespace

// Loaned from
// https://discuss.cocos2d-x.org/t/tutorial-cocos2d-camera-shake-effect-from-unity-updates/27215
void utils::shaker::shakeNode(cocos2d::Node* node, const bool lessen)
{
    float interval = 0.f;
    float duration = 0.12f;
    float speed = 2.0f;
    float magnitude = 1.7f;
    if (lessen) {
        magnitude -= 0.3f;
        duration -= 0.03f;
    }
    // Attention: single caller only
    static float elapsed = 0.f;

    node->schedule(
        [=](float dt) {
            float randomStart = cocos2d::random(-1000.0f, 1000.0f);
            elapsed += dt;

            const float percentComplete = elapsed / duration;

            // We want to reduce the shake from full power to 0 starting half way through
            const float damper = 1.0f - cocos2d::clampf(2.0f * percentComplete - 1.0f, 0.0f, 1.0f);

            // Calculate the noise parameter starting randomly and going as fast as speed allows
            const float alpha = randomStart + speed * percentComplete;

            // map noise to [-1, 1]
            float x = noise(alpha, 0.0f) * 2.0f - 1.0f;
            float y = noise(0.0f, alpha) * 2.0f - 1.0f;

            x *= magnitude * damper;
            y *= magnitude * damper;
            node->setPosition(x, y);

            if (elapsed >= duration) {
                elapsed = 0;
                node->unschedule("Shake");
                node->setPosition(cocos2d::Vec2::ZERO);
            }
        },
        interval, CC_REPEAT_FOREVER, 0.f, "Shake");
}
