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
    m_inputHandler.disable();

    scheduleUpdate();
    m_world.setDrawDebug(false);

    initTutorialLogic();

    return true;
}

void TutorialScene::update(const float dt)
{
    cocos2d::Scene::update(dt);
    m_world.update(dt);
}

void TutorialScene::clearInteractions()
{
    stopAllActions();
    m_tutorialInputHandler.resetCallbacks();
}

void TutorialScene::delayedText(const float delay, const std::string& text)
{
    auto delayAction = cocos2d::DelayTime::create(delay);
    auto callback =
        cocos2d::CallFunc::create([this, text]() { m_tutorialOverlay.showSecondaryText(text); });
    runAction(cocos2d::Sequence::create(delayAction, callback, nullptr));
}

void TutorialScene::initTutorialLogic()
{
    auto delay = cocos2d::DelayTime::create(0.5f);
    auto callback = cocos2d::CallFunc::create([this]() {
        const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
        const cocos2d::Vec2 origin{cocos2d::Director::getInstance()->getVisibleOrigin()};
        m_tutorialOverlay.showText(
            {origin.x + visibleSize.width / 8, origin.y + visibleSize.height / 4 * 3},
            "Do you feel bunny punk?");

        std::function<void()> changeText = [this]() {
            clearInteractions();
            m_tutorialOverlay.hide();
            m_tutorialOverlay.showText("Great!");
            delayedText(1.0f, "Let's jump to tutorial");
            m_tutorialInputHandler.setClickCallback([this]() { jumpLeftTutorial(); });

            auto delayNoclick = cocos2d::DelayTime::create(5.0f);
            auto transitionCallback = cocos2d::CallFunc::create([this]() { jumpLeftTutorial(); });
            runAction(cocos2d::Sequence::create(delayNoclick, transitionCallback, nullptr));
        };
        m_tutorialInputHandler.setClickCallback(changeText);
    });
    runAction(cocos2d::Sequence::create(delay, callback, nullptr));

    delayedText(5.0f, " .. tap the screen if you do!");
}

void TutorialScene::jumpLeftTutorial()
{
    clearInteractions();
    m_tutorialOverlay.hide();

    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    const cocos2d::Vec2 origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

    m_tutorialOverlay.showText(
        {origin.x + visibleSize.width / 10, origin.y + visibleSize.height / 4 * 3},
        "You can jump by tapping the screen");

    auto delayAction = cocos2d::DelayTime::create(1.0f);
    auto callback = cocos2d::CallFunc::create([this]() {
        m_tutorialOverlay.showSecondaryText("Try clicking left side");
        m_tutorialOverlay.showLeftArea();
        m_tutorialInputHandler.resetCallbacks();
        m_inputHandler.enable();
        m_tutorialInputHandler.setLeftJumpCallback([this]() {
            clearInteractions();
            m_tutorialInputHandler.setClickCallback([this]() { jumpRightTutorial(); });
            m_tutorialOverlay.hide();
            m_tutorialOverlay.showText("WOW!");
            delayedText(0.2f, "Well done!");
            auto delayNoclick = cocos2d::DelayTime::create(5.0f);
            auto transitionCallback = cocos2d::CallFunc::create([this]() { jumpRightTutorial(); });
            runAction(cocos2d::Sequence::create(delayNoclick, transitionCallback, nullptr));
        });
    });
    runAction(cocos2d::Sequence::create(delayAction, callback, nullptr));
}

void TutorialScene::jumpRightTutorial()
{
    clearInteractions();
    m_inputHandler.disable();
    m_tutorialOverlay.hide();

    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    const cocos2d::Vec2 origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

    m_tutorialOverlay.showText(
        {origin.x + visibleSize.width / 10, origin.y + visibleSize.height / 4 * 3},
        "Can you guess the next step?");

    auto delayAction = cocos2d::DelayTime::create(1.5f);
    auto callbackLambda = [this]() {
        clearInteractions();
        m_tutorialOverlay.showSecondaryText("Yup. Now try tapping the right side");
        m_tutorialOverlay.showRightArea();
        m_tutorialInputHandler.resetCallbacks();
        m_inputHandler.enable();
        m_tutorialInputHandler.setRightJumpCallback([this]() {
            clearInteractions();
            m_tutorialInputHandler.setClickCallback([this]() { doubleJumpTutorial(); });
            m_tutorialOverlay.hide();
            m_tutorialOverlay.showText("Bunny hop!");
            delayedText(0.2f, "Masterful bunny hop!");

            auto delayNoclick = cocos2d::DelayTime::create(5.0f);
            auto transitionCallback = cocos2d::CallFunc::create([this]() { doubleJumpTutorial(); });
            runAction(cocos2d::Sequence::create(delayNoclick, transitionCallback, nullptr));
        });
    };
    m_tutorialInputHandler.setClickCallback(callbackLambda);
    auto callback = cocos2d::CallFunc::create([callbackLambda]() { callbackLambda(); });
    runAction(cocos2d::Sequence::create(delayAction, callback, nullptr));
}

void TutorialScene::doubleJumpTutorial()
{
    clearInteractions();
    m_inputHandler.disable();
    m_tutorialOverlay.hide();

    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    const cocos2d::Vec2 origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

    m_tutorialOverlay.showText(
        {origin.x + visibleSize.width / 10, origin.y + visibleSize.height / 4 * 3},
        "Double jump placeholder?");
}
