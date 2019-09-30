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
#include "MenuScene.h"
#include "RetryScene.h"

bool GameLogic::init(cocos2d::Scene& scene)
{
    m_scene = &scene;
    bool ok = m_beeSpawner.init(scene);
    ok = ok && m_bunnyController.init(scene);
    ok = ok && m_inputHandler.init(m_bunnyController, scene.getEventDispatcher());
    ok = ok && m_contactListener.init(scene);
    if (!ok) {
        cocos2d::log("Game logic initialization failed");
        return false;
    }

    const auto stage = m_stageManager.getStage(m_curLvl);
    if (!stage) {
        cocos2d::log("Cannot get first stage");
        return false;
    }

    m_beeEventListener.init(scene, [this]() {
        if (m_state == GameState::active) {
            endStage();
        }
    });

    m_bunnyEventListener = cocos2d::EventListenerCustom::create(
        CustomEvent::bunnyHitEvent, [this](cocos2d::EventCustom* event) {
            if (m_state == GameState::active) {
                endGame();
            }
        });
    scene.getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_bunnyEventListener,
                                                                       &scene);

    m_actionNode = cocos2d::Node::create();
    scene.addChild(m_actionNode);

    initStage(*stage);
    return true;
}

void GameLogic::initStage(const StageInfo& stageInfo)
{
    m_bunnyController.disposeBunnies();
    m_bunnyController.spawnBunnies(stageInfo.bunnyCount);
    m_beeSpawner.disposeBees();
    m_beeSpawner.spawnBees(stageInfo.beeSpawns);
    m_beeEventListener.wait(stageInfo.beeSpawns.size());
    m_inputHandler.enable();
    m_state = GameState::active;
}

void GameLogic::endStage()
{
    m_state = GameState::wait;
    ++m_curLvl;
    m_inputHandler.disable();
    const auto stageTransitionDelay = cocos2d::DelayTime::create(2.0f);
    auto stageTransitionCallback = cocos2d::CallFunc::create([this]() {
        const auto stage = m_stageManager.getStage(m_curLvl);
        if (!stage) {
            // Victory text
            const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
            const auto origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

            auto victoryLabel =
                cocos2d::Label::createWithTTF("Victory!", "fonts/Marker Felt.ttf", 200);
            victoryLabel->setRotation(355);
            victoryLabel->setColor(cocos2d::Color3B::GREEN);

            if (victoryLabel == nullptr || victoryLabel->getContentSize().width <= 0 ||
                victoryLabel->getContentSize().height <= 0) {
                cocos2d::log("Could not load font for victory text");
            }
            else {
                victoryLabel->setPosition(cocos2d::Vec2{
                    origin.x + visibleSize.width / 3,
                    origin.y + visibleSize.height / 2 + victoryLabel->getContentSize().height / 2});
                m_scene->addChild(victoryLabel);
            }

            const auto gameEndDelay = cocos2d::DelayTime::create(5.0f);
            auto menuTransitionCallback = cocos2d::CallFunc::create([this]() {
                auto menuScene = MenuScene::create();
                cocos2d::Director::getInstance()->replaceScene(menuScene);
            });
            m_actionNode->runAction(
                cocos2d::Sequence::create(gameEndDelay, menuTransitionCallback, nullptr));
            return;
        }
        initStage(*stage);
    });

    m_actionNode->runAction(
        cocos2d::Sequence::create(stageTransitionDelay, stageTransitionCallback, nullptr));
}

void GameLogic::endGame()
{
    // Guaranteed to be found as we just ended this stage.
    const auto stage = m_stageManager.getStage(m_curLvl);
    initStage(*stage);
    cocos2d::Director::getInstance()->pushScene(m_scene);
    auto retryScene = RetryScene::create();
    cocos2d::Director::getInstance()->replaceScene(retryScene);
}
