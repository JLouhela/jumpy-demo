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

#include "RetryOverlay.h"
#include "MenuScene.h"

bool RetryOverlay::init(std::function<void()> retryCallback, cocos2d::Scene& scene)
{
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
    auto retryLabel = cocos2d::Label::createWithTTF("Try again", "fonts/Marker Felt.ttf", 100);
    retryLabel->setColor(cocos2d::Color3B::BLACK);
    auto retryButton = cocos2d::MenuItemLabel::create(
        retryLabel, [retryCallback](cocos2d::Ref* sender) { retryCallback(); });

    if (retryButton == nullptr || retryButton->getContentSize().width <= 0 ||
        retryButton->getContentSize().height <= 0) {
        cocos2d::log("Could not load font");
        return false;
    }

    retryButton->setPosition(
        cocos2d::Vec2{origin.x + visibleSize.width / 2,
                      origin.y + visibleSize.height / 2 + ouchLabel->getContentSize().height});

    // Main menu label
    auto menuLabel = cocos2d::Label::createWithTTF("Main Menu", "fonts/Marker Felt.ttf", 50);
    menuLabel->setColor(cocos2d::Color3B::BLACK);
    auto menuButton = cocos2d::MenuItemLabel::create(menuLabel, [](cocos2d::Ref* sender) {
        auto scene = MenuScene::createScene();
        cocos2d::Director::getInstance()->replaceScene(scene);
    });

    if (menuButton == nullptr || menuButton->getContentSize().width <= 0 ||
        menuButton->getContentSize().height <= 0) {
        cocos2d::log("Could not load font");
        return false;
    }

    menuButton->setPosition(
        cocos2d::Vec2{retryButton->getPosition().x,
                      retryButton->getPosition().y - retryLabel->getContentSize().height * 1.5f});

    // create menu, it's an autorelease object
    auto menu = cocos2d::Menu::createWithArray({retryButton, menuButton});
    menu->setPosition(cocos2d::Vec2::ZERO);
    m_overlay = cocos2d::Node::create();
    m_overlay->addChild(ouchLabel);
    m_overlay->addChild(menu, 1);
    scene.addChild(m_overlay);
    hide();
    return true;
}

void RetryOverlay::show()
{
    if (m_overlay) {
        m_overlay->setVisible(true);
    }
}

void RetryOverlay::hide()
{
    if (m_overlay) {
        m_overlay->setVisible(false);
    }
}
