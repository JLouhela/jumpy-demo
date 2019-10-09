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

#include "TutorialScene.h"
#include "EnvironmentBuilder.h"
#include "SimpleAudioEngine.h"

cocos2d::Scene* TutorialScene::createScene()
{
    return TutorialScene::create();
}

// on "init" you need to initialize your instance
bool TutorialScene::init()
{
    // super init always first
    if (!cocos2d::Scene::init()) {
        cocos2d::log("Could not initialize parent scene");
        return false;
    }

    if (!environment::buildEnvironment(*this, m_world.getWorld())) {
        cocos2d::log("Could not initialize gfx for scene");
        return false;
    }
    m_tutorialOverlay.init(*this);
    m_world.getWorld().SetContactListener(&m_contactListener);
    m_bunnyController.init(*this, m_world.getWorld());
    m_bunnyController.spawnBunnies();
    m_inputHandler.init(m_bunnyController);

    initTutorialLogic();
    scheduleUpdate();
    m_world.setDrawDebug(false);

    return true;
}

void TutorialScene::update(const float dt)
{
    cocos2d::Scene::update(dt);
    m_world.update(dt);
}

void TutorialScene::initTutorialLogic()
{
    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    const cocos2d::Vec2 origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

    // TODO display briefing text
    // wait for any click..
    // TODO show left shader + instructions
    // wait for left click
    m_tutorialOverlay.showLeftArea();
    m_tutorialOverlay.showText(
        {origin.x + visibleSize.height / 2, origin.y + visibleSize.height / 2}, "paska testi");
    // TODO wait for callback left click
    // Display text
}
