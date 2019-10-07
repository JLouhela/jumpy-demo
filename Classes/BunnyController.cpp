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

namespace {
constexpr std::uint8_t leftBunny{0};
constexpr std::uint8_t rightBunny{1};
}  // namespace

void BunnyController::spawnBunnies()
{
    const auto spawnPoints = getSpawnPoints(2);
    m_bunnyContainer[leftBunny].activate(spawnPoints[leftBunny]);
    m_bunnyContainer[rightBunny].activate(spawnPoints[rightBunny]);
}

void BunnyController::disposeBunnies()
{
    for (auto& bunny : m_bunnyContainer) {
        bunny.dispose();
    }
}

bool BunnyController::init(cocos2d::Scene& scene, b2World& world)
{
    Bunny_id id{0};
    // Before modifications to scene, check if any bunny was not initialized properly
    for (auto& bunny : m_bunnyContainer) {
        if (!bunny.init(id, scene, world)) {
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
        const float xOffset = visibleSize.width / (bunnyCount + 1);
        // There's only single stage, thus fixed yOffset. Ground height + bunny height / 2
        static constexpr float yOffset{176};
        res.emplace_back(xOffset * (i + 1), yOffset);
    }
    return res;
}

bool BunnyController::jumpBunny(const cocos2d::Vec2& pos)
{
    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    if (pos.x < visibleSize.width / 2) {
        m_bunnyContainer[leftBunny].jump();
    }
    else {
        m_bunnyContainer[rightBunny].jump();
    }
    return true;
}
