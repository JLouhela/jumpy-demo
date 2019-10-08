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
#include "Box2D/Box2D.h"
#include "CustomEvents.h"
#include "MenuScene.h"

namespace {
void displayVictoryLabel(cocos2d::Scene& scene)
{
    auto victoryLabel = cocos2d::Label::createWithTTF("Victory!", "fonts/Marker Felt.ttf", 200);
    victoryLabel->setRotation(355);
    victoryLabel->setColor(cocos2d::Color3B::GREEN);

    if (victoryLabel == nullptr || victoryLabel->getContentSize().width <= 0 ||
        victoryLabel->getContentSize().height <= 0) {
        cocos2d::log("Could not load font for victory text");
    }
    else {
        const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
        const auto origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

        victoryLabel->setPosition(cocos2d::Vec2{
            origin.x + visibleSize.width / 3,
            origin.y + visibleSize.height / 2 + victoryLabel->getContentSize().height / 2});
        scene.addChild(victoryLabel);
    }
}
}  // namespace

bool GameLogic::init(cocos2d::Scene& scene, b2World& world)
{
    m_scene = &scene;
    bool ok = m_beeSpawner.init(scene, world);
    ok = ok && m_bunnyController.init(scene, world);
    ok = ok && m_inputHandler.init(m_bunnyController);
    world.SetContactListener(&m_contactListener);

    auto retryCallback = [this]() {
        m_gameOverOverlay.hide();
        initGame();
    };

    ok = ok && m_gameOverOverlay.init(retryCallback, scene);
    if (!ok) {
        cocos2d::log("Game logic initialization failed");
        return false;
    }
    m_scoreCounter.init(scene);

    m_bunnyEventListener = cocos2d::EventListenerCustom::create(
        CustomEvent::bunnyHitEvent, [this](cocos2d::EventCustom* event) {
            if (m_state == GameState::active) {
                const auto finalScore = m_scoreCounter.getScore();
                cleanState();
                m_gameOverOverlay.show(finalScore);
            }
        });
    scene.getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_bunnyEventListener,
                                                                       &scene);

    m_actionNode = cocos2d::Node::create();
    scene.addChild(m_actionNode);

    initGame();
    return true;
}

void GameLogic::initGame()
{
    m_bunnyController.spawnBunnies();
    m_beeSpawner.spawnBees();
    m_scoreCounter.reset();
    m_inputHandler.enable();
    m_state = GameState::active;
}

void GameLogic::endGame()
{
    // TODO
}

void GameLogic::cleanState()
{
    m_bunnyController.disposeBunnies();
    m_beeSpawner.stop();
    m_scoreCounter.reset();
}
