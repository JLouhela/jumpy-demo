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
#include "JumpyScene.h"
#include "MenuScene.h"
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

    // android back press event
    m_keyListener = cocos2d::EventListenerKeyboard::create();
    m_keyListener->onKeyReleased = [](cocos2d::EventKeyboard::KeyCode keyCode,
                                      cocos2d::Event* event) {
        if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_BACK ||
            keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
            auto scene = MenuScene::createScene();
            cocos2d::Director::getInstance()->replaceScene(scene);
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_keyListener, this);

    initTutorialLogic();
    return true;
}

TutorialScene::~TutorialScene()
{
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(m_keyListener);
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
            {origin.x + visibleSize.width / 9, origin.y + visibleSize.height / 4 * 3},
            "Welcome to the tutorial");

        std::function<void()> changeText = [this]() {
            clearInteractions();
            m_tutorialOverlay.hide();
            m_tutorialOverlay.showText("You will learn how to bunny");
            delayedText(0.5f, "Let's jump right into action!");
            m_tutorialInputHandler.setClickCallback([this]() { jumpLeftTutorial(); });

            auto delayNoclick = cocos2d::DelayTime::create(5.0f);
            auto transitionCallback = cocos2d::CallFunc::create([this]() { jumpLeftTutorial(); });
            runAction(cocos2d::Sequence::create(delayNoclick, transitionCallback, nullptr));
        };
        m_tutorialInputHandler.setClickCallback(changeText);
    });
    runAction(cocos2d::Sequence::create(delay, callback, nullptr));

    delayedText(5.0f, " .. tap the screen to continue!");
}

void TutorialScene::jumpLeftTutorial()
{
    m_inputHandler.disable();
    clearInteractions();
    m_tutorialOverlay.hide();

    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    const cocos2d::Vec2 origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

    m_tutorialOverlay.showText(
        {origin.x + visibleSize.width / 9, origin.y + visibleSize.height / 4 * 3},
        "You can jump by tapping the screen");

    auto delayAction = cocos2d::DelayTime::create(2.0f);
    auto callbackLambda = [this]() {
        clearInteractions();
        m_tutorialOverlay.showSecondaryText("Try tapping left side of the screen");
        m_tutorialOverlay.showLeftArea();
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

        auto delayInput = cocos2d::DelayTime::create(0.1f);
        auto inputCallback = cocos2d::CallFunc::create([this]() { m_inputHandler.enable(); });
        runAction(cocos2d::Sequence::create(delayInput, inputCallback, nullptr));
    };

    m_tutorialInputHandler.setClickCallback(callbackLambda);
    auto callback = cocos2d::CallFunc::create([callbackLambda]() { callbackLambda(); });
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
        {origin.x + visibleSize.width / 9, origin.y + visibleSize.height / 4 * 3},
        "Can you guess the next step?");

    auto delayAction = cocos2d::DelayTime::create(2.0f);
    auto callbackLambda = [this]() {
        clearInteractions();
        m_tutorialOverlay.showSecondaryText("Yup. Now try tapping the right side");
        m_tutorialOverlay.showRightArea();
        m_tutorialInputHandler.setRightJumpCallback([this]() {
            clearInteractions();
            m_tutorialInputHandler.setClickCallback([this]() { doubleJumpPreparation(); });
            m_tutorialOverlay.hide();
            m_tutorialOverlay.showText("Bunny hop!");
            delayedText(0.2f, "Masterful bunny hop!");

            auto delayNoclick = cocos2d::DelayTime::create(5.0f);
            auto transitionCallback =
                cocos2d::CallFunc::create([this]() { doubleJumpPreparation(); });
            runAction(cocos2d::Sequence::create(delayNoclick, transitionCallback, nullptr));
        });

        auto delayInput = cocos2d::DelayTime::create(0.1f);
        auto inputCallback = cocos2d::CallFunc::create([this]() { m_inputHandler.enable(); });
        runAction(cocos2d::Sequence::create(delayInput, inputCallback, nullptr));
    };
    m_tutorialInputHandler.setClickCallback(callbackLambda);
    auto callback = cocos2d::CallFunc::create([callbackLambda]() { callbackLambda(); });
    runAction(cocos2d::Sequence::create(delayAction, callback, nullptr));
}

void TutorialScene::doubleJumpPreparation()
{
    clearInteractions();
    m_inputHandler.disable();
    m_tutorialOverlay.hide();

    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    const cocos2d::Vec2 origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

    m_tutorialOverlay.showText(
        {origin.x + visibleSize.width / 9, origin.y + visibleSize.height / 4 * 3}, "How about..");

    auto delayAction_1 = cocos2d::DelayTime::create(0.5f);
    auto callback_1 = cocos2d::CallFunc::create([this]() {
        m_tutorialOverlay.showSecondaryText("DOUBLE JUMP?");
        clearInteractions();

        auto delayAction_2 = cocos2d::DelayTime::create(3.0f);
        auto lambdaCallback_2 = [this]() {
            clearInteractions();
            m_tutorialOverlay.hide();
            m_tutorialOverlay.showText("Let's give it a shot");

            m_tutorialInputHandler.setClickCallback([this]() { doubleJumpTutorial(); });
            auto delayNoclick = cocos2d::DelayTime::create(5.0f);
            auto transitionCallback = cocos2d::CallFunc::create([this]() { doubleJumpTutorial(); });
            runAction(cocos2d::Sequence::create(delayNoclick, transitionCallback, nullptr));
        };

        m_tutorialInputHandler.setClickCallback(lambdaCallback_2);
        auto callback_2 = cocos2d::CallFunc::create([lambdaCallback_2]() { lambdaCallback_2(); });
        runAction(cocos2d::Sequence::create(delayAction_2, callback_2, nullptr));
    });
    runAction(cocos2d::Sequence::create(delayAction_1, callback_1, nullptr));
}

void TutorialScene::doubleJumpTutorial()
{
    clearInteractions();
    m_inputHandler.disable();
    m_tutorialOverlay.hide();

    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    const cocos2d::Vec2 origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

    m_tutorialOverlay.showText(
        {origin.x + visibleSize.width / 9, origin.y + visibleSize.height / 4 * 3},
        "Tap once for a single jump");

    auto delayAction = cocos2d::DelayTime::create(0.5f);
    auto callbackLambda = [this]() {
        clearInteractions();
        m_tutorialOverlay.showSecondaryText("Tap twice for double jump, try it!");
        // Workaround no time, this is getting nasty.
        // Should init bunnycontroller in tutorial mode instead and fire events from
        // successful actions. Now this is a guessing game.
        m_tutorialInputHandler.setClickCallback([this]() {
            static bool jumped{false};
            if (jumped) {
                m_tutorialInputHandler.resetLastClick(false);
                jumped = false;
            }
            else {
                jumped = true;
            }
        });
        m_tutorialInputHandler.setDoubleJumpCallback([this]() {
            clearInteractions();
            auto transitionLambda = [this]() {
                clearInteractions();
                m_tutorialInputHandler.resetCallbacks();
                m_tutorialInputHandler.setClickCallback([this]() { doubleJumpClosure(); });
                m_tutorialOverlay.hide();
                m_tutorialOverlay.showText("Splendid!");
                delayedText(0.1f, "just remember..!");
                m_inputHandler.disable();
                auto delayNoclick = cocos2d::DelayTime::create(5.0f);
                auto transitionCallback =
                    cocos2d::CallFunc::create([this]() { doubleJumpClosure(); });
                runAction(cocos2d::Sequence::create(delayNoclick, transitionCallback, nullptr));
            };
            auto delayTransition = cocos2d::DelayTime::create(0.1f);
            auto transitionCallback =
                cocos2d::CallFunc::create([transitionLambda]() { transitionLambda(); });
            runAction(cocos2d::Sequence::create(delayTransition, transitionCallback, nullptr));
        });

        auto delayInput = cocos2d::DelayTime::create(0.1f);
        auto inputCallback = cocos2d::CallFunc::create([this]() {
            m_inputHandler.enable();
            m_tutorialInputHandler.resetLastClick(true);
        });
        runAction(cocos2d::Sequence::create(delayInput, inputCallback, nullptr));
    };
    m_tutorialInputHandler.setClickCallback(callbackLambda);
    auto callback = cocos2d::CallFunc::create([callbackLambda]() { callbackLambda(); });
    runAction(cocos2d::Sequence::create(delayAction, callback, nullptr));
}

void TutorialScene::doubleJumpClosure()
{
    clearInteractions();
    m_tutorialOverlay.hide();

    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    const cocos2d::Vec2 origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

    auto delay = cocos2d::DelayTime::create(0.1f);
    auto callback = cocos2d::CallFunc::create([this]() {
        const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
        const cocos2d::Vec2 origin{cocos2d::Director::getInstance()->getVisibleOrigin()};
        m_tutorialOverlay.showText(
            {origin.x + visibleSize.width / 9, origin.y + visibleSize.height / 4 * 3},
            "It's called DOUBLE jump");

        std::function<void()> changeText = [this]() {
            clearInteractions();
            m_tutorialOverlay.hide();
            m_tutorialOverlay.showText("You cannot jump again more");
            delayedText(0.1f, "than once until you land again.");
            m_tutorialInputHandler.setClickCallback([this]() { dashPreparation(); });

            auto delayNoclick = cocos2d::DelayTime::create(5.0f);
            auto transitionCallback = cocos2d::CallFunc::create([this]() { dashPreparation(); });
            runAction(cocos2d::Sequence::create(delayNoclick, transitionCallback, nullptr));
        };
        m_tutorialInputHandler.setClickCallback(changeText);
    });
    runAction(cocos2d::Sequence::create(delay, callback, nullptr));
}

void TutorialScene::dashPreparation()
{
    clearInteractions();
    m_inputHandler.disable();
    m_tutorialOverlay.hide();

    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    const cocos2d::Vec2 origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

    m_tutorialOverlay.showText(
        {origin.x + visibleSize.width / 9, origin.y + visibleSize.height / 4 * 3},
        "Huh. Only one more trick..");

    auto delayAction_1 = cocos2d::DelayTime::create(0.1f);
    auto callback_1 = cocos2d::CallFunc::create([this]() {
        m_tutorialOverlay.showSecondaryText(".. one more!");
        clearInteractions();

        auto delayAction_2 = cocos2d::DelayTime::create(3.0f);
        auto lambdaCallback_2 = [this]() {
            clearInteractions();
            m_tutorialOverlay.hide();
            m_tutorialOverlay.showText("*drumroll* .. it's DIVE");

            m_tutorialInputHandler.setClickCallback([this]() { dashTutorial(); });
            auto delayNoclick = cocos2d::DelayTime::create(5.0f);
            auto transitionCallback = cocos2d::CallFunc::create([this]() { dashTutorial(); });
            runAction(cocos2d::Sequence::create(delayNoclick, transitionCallback, nullptr));
        };

        m_tutorialInputHandler.setClickCallback(lambdaCallback_2);
        auto callback_2 = cocos2d::CallFunc::create([lambdaCallback_2]() { lambdaCallback_2(); });
        runAction(cocos2d::Sequence::create(delayAction_2, callback_2, nullptr));
    });
    runAction(cocos2d::Sequence::create(delayAction_1, callback_1, nullptr));
}

void TutorialScene::dashTutorial()
{
    m_inputHandler.disable();
    clearInteractions();
    m_tutorialOverlay.hide();

    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    const cocos2d::Vec2 origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

    m_tutorialOverlay.showText(
        {origin.x + visibleSize.width / 9, origin.y + visibleSize.height / 4 * 3},
        "Diving is simple");
    delayedText(0.1f, "yet it may save your tail one day.");

    auto delayAction = cocos2d::DelayTime::create(3.0f);
    auto callbackLambda = [this]() {
        clearInteractions();
        m_tutorialOverlay.hide();
        m_tutorialOverlay.showText("Get airborne first");
        delayedText(0.1f, "And swipe down the screen to dive. Try it!");
        // TODO display finger
        m_tutorialInputHandler.setDiveCallback([this]() {
            clearInteractions();
            m_tutorialInputHandler.setClickCallback([this]() { tutorialClosure(); });
            m_tutorialOverlay.hide();
            m_tutorialOverlay.showText("WOOHOO!");
            delayedText(0.2f, "What a speed!");
            auto delayNoclick = cocos2d::DelayTime::create(5.0f);
            auto transitionCallback = cocos2d::CallFunc::create([this]() { tutorialClosure(); });
            runAction(cocos2d::Sequence::create(delayNoclick, transitionCallback, nullptr));
        });

        auto delayInput = cocos2d::DelayTime::create(0.1f);
        auto inputCallback = cocos2d::CallFunc::create([this]() { m_inputHandler.enable(); });
        runAction(cocos2d::Sequence::create(delayInput, inputCallback, nullptr));
    };

    m_tutorialInputHandler.setClickCallback(callbackLambda);
    auto callback = cocos2d::CallFunc::create([callbackLambda]() { callbackLambda(); });
    runAction(cocos2d::Sequence::create(delayAction, callback, nullptr));
}

void TutorialScene::tutorialClosure()
{
    m_inputHandler.disable();
    clearInteractions();
    m_tutorialOverlay.hide();
    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    const cocos2d::Vec2 origin{cocos2d::Director::getInstance()->getVisibleOrigin()};
    m_tutorialOverlay.showText(
        {origin.x + visibleSize.width / 9, origin.y + visibleSize.height / 4 * 3},
        "You are ready to begin!");
    delayedText(0.1f, "Tap to continue");
    m_tutorialInputHandler.setClickCallback([this]() {
        auto scene = JumpyScene::createScene();
        cocos2d::Director::getInstance()->replaceScene(scene);
    });
}
