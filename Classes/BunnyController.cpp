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

bool BunnyController::spawnBunny(const cocos2d::Vec2& pos)
{
    auto& nextBunny = getNextAvailableBunny();
    if (!nextBunny.first) {
        return false;
    }
    nextBunny.second.setPosition(pos);
    nextBunny.first = false;
    return true;
}

BunnyController::AvailableBunny& BunnyController::getNextAvailableBunny()
{
    for (auto& bunny : m_bunnyContainer) {
        if (bunny.first == true) {
            return bunny;
        }
    }
    cocos2d::log("No more free bunnies");
    return m_bunnyContainer.front();
}

bool BunnyController::init(cocos2d::Scene& scene)
{
    Bunny_id id{0};
    // Before modifications to scene, check if any bunny was not initialized properly
    for (auto& bunny : m_bunnyContainer) {
        if (!bunny.second.init(id, scene)) {
            return false;
        }
        ++id;
    }
    for (const auto& bunny : m_bunnyContainer) {
        scene.addChild(bunny.second.getSprite(), ZOrder::bunny);
    }
    return true;
}

bool BunnyController::jumpBunny(const cocos2d::Vec2& pos)
{
    for (auto& bunny : m_bunnyContainer) {
        // Make input more forgiving by adding extra threshold
        // to the bounding box of a bunny, especially vertically.
        auto rect = bunny.second.getBoundingBox();
        static constexpr float extraWidth = 20.0f;
        static constexpr float extraHeight = 60.0f;
        rect.size.width += extraWidth;
        rect.size.height += extraHeight;
        rect.origin.x -= extraWidth / 2;
        rect.origin.y -= extraHeight / 2;

        // In this game bunnies cannot overlap horizontally.
        // First match can consume the jump trigger.
        if (rect.containsPoint(pos)) {
            bunny.second.jump();
            return true;
        }
    }
    return false;
}
