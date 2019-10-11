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

#include "MenuScene.h"
#include "JumpyScene.h"
#include "TutorialScene.h"

cocos2d::Scene* MenuScene::createScene()
{
    return MenuScene::create();
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    const auto origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

    auto bgSprite = cocos2d::Sprite::create("main_menu_1280_720.png");
    bgSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);

    // Start label
    auto startLabel = cocos2d::Label::createWithTTF("Start", "fonts/Marker Felt.ttf", 200);
    auto startLabelButton = cocos2d::MenuItemLabel::create(startLabel, [](Ref* sender) {
        auto scene = JumpyScene::createScene();
        cocos2d::Director::getInstance()->replaceScene(scene);
    });

    if (startLabelButton == nullptr || startLabelButton->getContentSize().width <= 0 ||
        startLabelButton->getContentSize().height <= 0) {
        cocos2d::log("Could not load font");
        return false;
    }
    startLabel->setColor(cocos2d::Color3B(161, 210, 230));
    startLabel->enableOutline(cocos2d::Color4B::BLACK, 2);
    startLabelButton->setPosition(
        cocos2d::Vec2{origin.x + visibleSize.width / 2,
                      origin.y + visibleSize.height / 2 + startLabel->getContentSize().height / 2});

    // tutorial label
    auto tutorialLabel = cocos2d::Label::createWithTTF("Tutorial", "fonts/Marker Felt.ttf", 100);

    auto tutorialLabelButton = cocos2d::MenuItemLabel::create(tutorialLabel, [](Ref* sender) {
        auto scene = TutorialScene::createScene();
        cocos2d::Director::getInstance()->replaceScene(scene);
    });

    if (tutorialLabelButton == nullptr || tutorialLabelButton->getContentSize().width <= 0 ||
        tutorialLabelButton->getContentSize().height <= 0) {
        cocos2d::log("Could not load font");
        return false;
    }
    tutorialLabel->setColor(cocos2d::Color3B(161, 210, 230));
    tutorialLabel->enableOutline(cocos2d::Color4B::BLACK, 2);
    tutorialLabelButton->setPosition(
        cocos2d::Vec2{startLabelButton->getPosition().x,
                      startLabelButton->getPosition().y - startLabel->getContentSize().height});

    // Exit label
    auto exitLabel = cocos2d::Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 100);
    auto exitLabelButton = cocos2d::MenuItemLabel::create(
        exitLabel, [](Ref* sender) { cocos2d::Director::getInstance()->end(); });

    if (exitLabelButton == nullptr || exitLabelButton->getContentSize().width <= 0 ||
        exitLabelButton->getContentSize().height <= 0) {
        cocos2d::log("Could not load font");
        return false;
    }

    exitLabel->setColor(cocos2d::Color3B(161, 210, 230));
    exitLabel->enableOutline(cocos2d::Color4B::BLACK, 2);
    exitLabelButton->setPosition(cocos2d::Vec2{startLabelButton->getPosition().x,
                                               startLabelButton->getPosition().y -
                                                   startLabel->getContentSize().height -
                                                   tutorialLabel->getContentSize().height});

    // create menu, it's an autorelease object
    auto menu =
        cocos2d::Menu::createWithArray({startLabelButton, tutorialLabelButton, exitLabelButton});
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(bgSprite, 0);
    this->addChild(menu, 1);
    return true;
}
