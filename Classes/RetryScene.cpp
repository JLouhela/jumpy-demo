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

#include "RetryScene.h"
#include "JumpyScene.h"
#include "MenuScene.h"

cocos2d::Scene* RetryScene::createScene()
{
    return RetryScene::create();
}

// on "init" you need to initialize your instance
bool RetryScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    const auto origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

    // ouch label
    auto ouchLabel = cocos2d::Label::createWithTTF("Ouch!", "fonts/Marker Felt.ttf", 100);
    ouchLabel->setRotation(315);
    ouchLabel->setColor(cocos2d::Color3B::RED);

    if (ouchLabel == nullptr || ouchLabel->getContentSize().width <= 0 ||
        ouchLabel->getContentSize().height <= 0) {
        cocos2d::log("Could not load font");
        return false;
    }
    ouchLabel->setPosition(cocos2d::Vec2{
        origin.x + visibleSize.width / 3,
        origin.y + 3 * visibleSize.height / 4 + ouchLabel->getContentSize().height / 2});

    // Retry label
    auto retryLabel = cocos2d::Label::createWithTTF("Retry", "fonts/Marker Felt.ttf", 100);
    auto retryLabelButton = cocos2d::MenuItemLabel::create(
        retryLabel, [](Ref* sender) { cocos2d::Director::getInstance()->popScene(); });

    if (retryLabelButton == nullptr || retryLabelButton->getContentSize().width <= 0 ||
        retryLabelButton->getContentSize().height <= 0) {
        cocos2d::log("Could not load font");
        return false;
    }

    retryLabelButton->setPosition(
        cocos2d::Vec2{origin.x + visibleSize.width / 2,
                      origin.y + visibleSize.height / 2 + ouchLabel->getContentSize().height});

    // Main menu label
    auto menuLabel = cocos2d::Label::createWithTTF("Main Menu", "fonts/Marker Felt.ttf", 50);
    auto menuLabelButton = cocos2d::MenuItemLabel::create(menuLabel, [](Ref* sender) {
        auto scene = MenuScene::createScene();
        cocos2d::Director::getInstance()->replaceScene(scene);
    });

    if (menuLabelButton == nullptr || menuLabelButton->getContentSize().width <= 0 ||
        menuLabelButton->getContentSize().height <= 0) {
        cocos2d::log("Could not load font");
        return false;
    }

    menuLabelButton->setPosition(cocos2d::Vec2{
        retryLabelButton->getPosition().x,
        retryLabelButton->getPosition().y - retryLabel->getContentSize().height * 1.5f});

    // create menu, it's an autorelease object
    auto menu = cocos2d::Menu::createWithArray({retryLabelButton, menuLabelButton});
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(ouchLabel);
    this->addChild(menu, 1);
    return true;
}
