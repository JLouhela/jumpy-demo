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

#include "GameLogic.h"
#include "CustomEvents.h"

bool GameLogic::init(cocos2d::Scene* scene)
{
    bool ok = (scene != nullptr);
    ok = ok && m_beeSpawner.init(*scene);
    ok = ok && m_bunnyController.init(*scene);
    ok = ok && m_inputHandler.init(m_bunnyController, scene->getEventDispatcher());
    if (!ok) {
        cocos2d::log("Game logic initialization failed");
        return false;
    }

    const auto stage = m_stageManager.getNext();
    if (!stage) {
        cocos2d::log("Cannot get next stage");

        return false;
    }

    m_beeEventListener.init(*scene, [this]() {
        if (m_state == GameState::active) {
            m_state = GameState::wait;
        }
    });

    m_bunnyEventListener = cocos2d::EventListenerCustom::create(
        CustomEvent::bunnyHitEvent, [this](cocos2d::EventCustom* event) {
            if (m_state == GameState::active) {
                m_state = GameState::end;
            }
        });
    scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_bunnyEventListener,
                                                                        scene);

    initStage(*stage);
    return true;
}

void GameLogic::initStage(StageInfo& stageInfo)
{
    // Stages never contain less bunnies than previous stage by design.
    while (m_bunnyCount < stageInfo.bunnyCount) {
        ++m_bunnyCount;
        const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
        const float xOffset = visibleSize.width / BunnyController::maxBunnyCount;
        // Should spread from center towards borders, if there's extra time for implementation.
        // There's only single stage, thus fixed yOffset
        static constexpr float yOffset{128.0f};
        const cocos2d::Vec2 bunnyPos{xOffset * m_bunnyCount, yOffset};
        m_bunnyController.spawnBunny(bunnyPos);
    }
    m_beeSpawner.spawnBees(stageInfo.beeSpawns);
}
