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
#include <cstdint>
#include "ZOrders.h"

void BunnyController::spawnBunnies(std::uint8_t count)
{
    if (count > maxBunnyCount) {
        cocos2d::log("Too many bunnies requested (%d), maximum is (%d)", count, maxBunnyCount);
        count = maxBunnyCount;
    }
    const auto spawnPoints = getSpawnPoints(count);

    for (std::uint8_t i{0}; i < count; ++i) {
        m_bunnyContainer[i].activate(spawnPoints[i]);
    }
}

void BunnyController::disposeBunnies()
{
    for (auto& bunny : m_bunnyContainer) {
        bunny.dispose();
    }
}

bool BunnyController::init(cocos2d::Scene& scene)
{
    Bunny_id id{0};
    // Before modifications to scene, check if any bunny was not initialized properly
    for (auto& bunny : m_bunnyContainer) {
        if (!bunny.init(id, scene)) {
            return false;
        }
        ++id;
    }
    for (const auto& bunny : m_bunnyContainer) {
        scene.addChild(bunny.getSprite(), ZOrder::bunny);
    }
    return true;
}

std::vector<cocos2d::Vec2> BunnyController::getSpawnPoints(const std::uint8_t bunnyCount)
{
    std::vector<cocos2d::Vec2> res;
    for (std::uint8_t i{0}; i < bunnyCount; ++i) {
        const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
        // FIXME 16.0f should come from bunny origin delta
        const float xOffset = visibleSize.width / (bunnyCount + 1) - 24.0f;
        // There's only single stage, thus fixed yOffset. Ground height + bunny height / 2
        static constexpr float yOffset{177.0f};
        res.emplace_back(xOffset * (i + 1), yOffset);
    }
    return res;
}

bool BunnyController::jumpBunny(const cocos2d::Vec2& pos)
{
    for (auto& bunny : m_bunnyContainer) {
        // Make input more forgiving by adding extra threshold
        // to the bounding box of a bunny, especially vertically.
        auto rect = bunny.getBoundingBox();
        static constexpr float extraWidth = 20.0f;
        static constexpr float extraHeight = 60.0f;
        rect.size.width += extraWidth;
        rect.size.height += extraHeight;
        rect.origin.x -= extraWidth / 2;
        rect.origin.y -= extraHeight / 2;

        // In this game bunnies cannot overlap horizontally.
        // First match can consume the jump trigger.
        if (rect.containsPoint(pos)) {
            bunny.jump();
            return true;
        }
    }
    return false;
}
