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

#include "TutorialOverlay.h"
#include "MenuScene.h"
#include "ZOrders.h"

bool TutorialOverlay::init(cocos2d::Scene& scene)
{
    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    const auto origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

    // Ouch label
    m_label = cocos2d::Label::createWithTTF("Placeholder", "fonts/Marker Felt.ttf", 80);
    m_label->setColor(cocos2d::Color3B::YELLOW);

    if (m_label == nullptr || m_label->getContentSize().width <= 0 ||
        m_label->getContentSize().height <= 0) {
        cocos2d::log("Could not load font");
        return false;
    }
    m_label->setPosition(
        cocos2d::Vec2{origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2});

    m_leftRect = cocos2d::RenderTexture::create(visibleSize.width / 2, visibleSize.height);
    m_leftRect->clear(0.2f, 0.2f, 0.0f, 0.0f);
    m_leftRect->setPosition(
        cocos2d::Vec2{origin.x + visibleSize.width / 4, origin.y + visibleSize.height / 2});

    m_overlay = cocos2d::Node::create();
    m_overlay->addChild(m_label);
    m_overlay->addChild(m_leftRect, ZOrder::overlay);
    scene.addChild(m_overlay, ZOrder::overlay);
    hide();
    return true;
}

void TutorialOverlay::hide()
{
    if (m_overlay) {
        m_overlay->setVisible(false);
        for (auto child : m_overlay->getChildren()) {
            child->setVisible(false);
        }
    }
}

void TutorialOverlay::showText(const cocos2d::Vec2& pos, const std::string& text)
{
    m_label->setString(text);
    m_label->setPosition(pos);
    m_label->setVisible(true);
    m_overlay->setVisible(true);
}

void TutorialOverlay::showLeftArea()
{
    m_overlay->setVisible(true);
    m_leftRect->setVisible(true);
}
void TutorialOverlay::showRightArea()
{
    // TODO
    m_overlay->setVisible(false);
}
void TutorialOverlay::showFinger(const cocos2d::Vec2& pos)
{
    // TODO
    m_overlay->setVisible(false);
}
