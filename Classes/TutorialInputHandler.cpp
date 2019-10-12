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

#include "TutorialInputHandler.h"
#include "BunnyController.h"
#include "Clock.h"
#include "CustomEvents.h"

void TutorialInputHandler::init(cocos2d::Scene& scene)
{
    m_touchListener = cocos2d::EventListenerTouchOneByOne::create();
    m_touchListener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
        if (m_clickCallback) {
            m_clickCallback();
        }
        return false;
    };

    m_jumpListener = cocos2d::EventListenerCustom::create(CustomEvent::bunnyJumpEvent,
                                                          [this](cocos2d::EventCustom* event) {
                                                              if (m_jumpCallback) {
                                                                  m_jumpCallback();
                                                              }
                                                          });

    m_doubleJumpListener = cocos2d::EventListenerCustom::create(
        CustomEvent::bunnyDoubleJumpEvent, [this](cocos2d::EventCustom* event) {
            if (m_doubleJumpCallback) {
                m_doubleJumpCallback();
            }
        });

    m_diveListener = cocos2d::EventListenerCustom::create(CustomEvent::bunnyDiveEvent,
                                                          [this](cocos2d::EventCustom* event) {
                                                              if (m_diveCallback) {
                                                                  m_diveCallback();
                                                              }
                                                          });

    m_actionNode = cocos2d::Node::create();
    scene.addChild(m_actionNode);
    scene.getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_jumpListener, &scene);
    scene.getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_doubleJumpListener,
                                                                       &scene);
    scene.getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_diveListener, &scene);

    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(
        m_touchListener, 1);
}

void TutorialInputHandler::resetCallbacks()
{
    m_doubleJumpCallback = nullptr;
    m_diveCallback = nullptr;
    m_jumpCallback = nullptr;
    m_clickCallback = nullptr;
}

void TutorialInputHandler::setClickCallback(std::function<void()> cb, float waitSeconds)
{
    auto delay = cocos2d::DelayTime::create(waitSeconds);
    auto setCallback = cocos2d::CallFunc::create([this, cb]() { m_clickCallback = cb; });
    m_actionNode->runAction(cocos2d::Sequence::create(delay, setCallback, nullptr));
}

TutorialInputHandler::~TutorialInputHandler()
{
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(m_touchListener);
}
