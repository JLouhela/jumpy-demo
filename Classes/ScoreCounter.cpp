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

#include "ScoreCounter.h"
#include <string>
#include "CustomEvents.h"
#include "ZOrders.h"

void ScoreCounter::init(cocos2d::Scene& scene)
{
    m_beeListener = cocos2d::EventListenerCustom::create(
        CustomEvent::beeThroughEvent,
        [& score = m_score, &label = m_label](cocos2d::EventCustom* event) {
            score++;
            label->setString("Score: " + std::to_string(score));
        });

    scene.getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_beeListener, &scene);

    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    const auto origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

    m_label = cocos2d::Label::createWithTTF("Score: 0", "fonts/Marker Felt.ttf", 32);
    m_label->setColor(cocos2d::Color3B::WHITE);

    if (m_label == nullptr || m_label->getContentSize().width <= 0 ||
        m_label->getContentSize().height <= 0) {
        cocos2d::log("Could not load font");
    }
    m_label->setPosition(cocos2d::Vec2{origin.x + visibleSize.width / 10 * 9,
                                       origin.y + visibleSize.height / 10 * 9});
    m_label->enableOutline(cocos2d::Color4B::BLACK, 2);

    m_scoreOverlay = cocos2d::Node::create();
    m_scoreOverlay->addChild(m_label);
    scene.addChild(m_scoreOverlay, ZOrder::overlay);
}

void ScoreCounter::reset()
{
    m_score = 0;
}
