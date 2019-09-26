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

#include "BunnyController.h"
#include "ZOrders.h"

bool BunnyController::spawnBunny(const cocos2d::Vec2& pos)
{
    if (m_freeBunnies.empty()) {
        cocos2d::log("No free bunnies to spawn");
        return false;
    }

    const auto nextIdx{m_freeBunnies.back()};
    m_freeBunnies.pop_back();
    m_bunnyContainer[nextIdx].setPosition(pos);
    return true;
}

bool BunnyController::init(cocos2d::Scene& scene)
{
    // Before modifications to scene, check if any bunny was not initialized properly
    for (const auto& bunny : m_bunnyContainer) {
        if (bunny.getId() == invalidBunnyId) {
            return false;
        }
    }
    for (const auto& bunny : m_bunnyContainer) {
        scene.addChild(bunny.getSprite(), ZOrder::bunny);
    }
    return true;
}
