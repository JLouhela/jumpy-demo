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

namespace {
cocos2d::Label* createLabel(cocos2d::Scene& scene)
{
    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    const auto origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

    auto label = cocos2d::Label::createWithTTF("Placeholder", "fonts/Marker Felt.ttf", 80);
    label->setColor(cocos2d::Color3B::YELLOW);

    if (label == nullptr || label->getContentSize().width <= 0 ||
        label->getContentSize().height <= 0) {
        cocos2d::log("Could not load font");
        return nullptr;
    }
    label->setPosition(
        cocos2d::Vec2{origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2});

    label->setAnchorPoint({0.0f, 1.0f});
    label->enableOutline(cocos2d::Color4B::BLACK, 2);
    return label;
}
}  // namespace

bool TutorialOverlay::init(cocos2d::Scene& scene)
{
    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    const auto origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

    m_label = createLabel(scene);
    m_secondaryLabel = createLabel(scene);
    m_secondaryLabel->setScale(0.75f);

    m_leftRect = cocos2d::RenderTexture::create(visibleSize.width / 2, visibleSize.height);
    m_leftRect->clear(0.2f, 0.2f, 0.0f, 0.0f);
    m_leftRect->setPosition(
        cocos2d::Vec2{origin.x + visibleSize.width / 4, origin.y + visibleSize.height / 2});

    m_rightRect = cocos2d::RenderTexture::create(visibleSize.width / 2, visibleSize.height);
    m_rightRect->clear(0.2f, 0.2f, 0.0f, 0.0f);
    m_rightRect->setPosition(
        cocos2d::Vec2{origin.x + visibleSize.width / 4 * 3, origin.y + visibleSize.height / 2});

    m_overlay = cocos2d::Node::create();
    m_overlay->addChild(m_label);
    m_overlay->addChild(m_secondaryLabel);
    m_overlay->addChild(m_leftRect, ZOrder::overlay);
    m_overlay->addChild(m_rightRect, ZOrder::overlay);
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
    showLabel(m_label, pos, text);
}

void TutorialOverlay::showText(const std::string& text)
{
    showLabel(m_label, m_label->getPosition(), text);
}

void TutorialOverlay::showSecondaryText(const std::string& text)
{
    auto pos = m_label->getPosition();
    pos.y -= m_label->getContentSize().height * 1.1f;
    showLabel(m_secondaryLabel, pos, text);
}

void TutorialOverlay::showLabel(cocos2d::Label* label,
                                const cocos2d::Vec2& pos,
                                const std::string& text)
{
    auto moveTo = cocos2d::MoveTo::create(0.1f, pos);
    label->setString(text);
    label->setPosition({-label->getContentSize().width, pos.y});
    label->runAction(moveTo);
    label->setVisible(true);
    m_overlay->setVisible(true);
}

void TutorialOverlay::showLeftArea()
{
    m_overlay->setVisible(true);
    m_leftRect->setVisible(true);
}
void TutorialOverlay::showRightArea()
{
    m_rightRect->setVisible(true);
    m_overlay->setVisible(true);
}
void TutorialOverlay::showFinger(const cocos2d::Vec2& pos)
{
    // TODO
    m_overlay->setVisible(false);
}
