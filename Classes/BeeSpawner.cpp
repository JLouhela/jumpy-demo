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

#include "BeeSpawner.h"
#include <cstdint>
#include "ZOrders.h"

namespace {

Bee* getAvailableBee(std::array<Bee, 10>& bees)
{
    for (auto& bee : bees) {
        if (bee.getState() == BeeState::inactive) {
            // Shortcut: should be tied to spawn, but this menas
            // bee should offer functionality to spawn after certain time.
            bee.activate();
            return &bee;
        }
    }
    return nullptr;
}

}  // namespace

bool BeeSpawner::spawnBees(const BeeSpawns& spawn)
{
    for (const auto& spawnBee : spawn) {
        auto bee = getAvailableBee(m_beeContainer);
        if (!bee) {
            cocos2d::log("No free bees to spawn");
            return false;
        }

        const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
        static constexpr float xOffset = 20.0f;
        const float x = (spawnBee.dir == direction::left) ? (visibleSize.width - xOffset) : xOffset;

        auto delayAction = cocos2d::DelayTime::create(spawnBee.delay);
        auto callback = cocos2d::CallFunc::create([bee, spawnBee, x]() {
            bee->spawn(cocos2d::Vec2{x, spawnBee.y}, spawnBee.dir);
        });
        m_actionNode->runAction(cocos2d::Sequence::create(delayAction, callback, nullptr));
    }
    return true;
}

bool BeeSpawner::init(cocos2d::Scene& scene, b2World& world)
{
    // Before modifications to scene, check if any bunny was not initialized properly
    Bee_id id{0};
    for (auto& bee : m_beeContainer) {
        if (!bee.init(id++, world)) {
            return false;
        }
    }
    m_actionNode = cocos2d::Node::create();
    scene.addChild(m_actionNode);
    for (const auto& bee : m_beeContainer) {
        scene.addChild(bee.getSprite(), ZOrder::bee);
    }
    return true;
}

void BeeSpawner::disposeBees()
{
    for (auto& bee : m_beeContainer) {
        bee.dispose();
    }
    m_actionNode->cleanup();
}
