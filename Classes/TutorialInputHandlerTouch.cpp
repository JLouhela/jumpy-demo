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

#include "TutorialInputHandlerTouch.h"
#include "BunnyController.h"
#include "Clock.h"

void TutorialInputHandler::init(cocos2d::Scene& scene)
{
    // TODO swipe handling
    m_touchListener = cocos2d::EventListenerTouchOneByOne::create();
    m_touchListener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
        return false;
    };
    m_touchListener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
        auto location = touch->getLocationInView();
        location.y = cocos2d::Director::getInstance()->getVisibleSize().height - location.y;
        resolveInput(location, InputType::jump);
        return false;  // Never consume here
    };

    m_actionNode = cocos2d::Node::create();
    scene.addChild(m_actionNode);

    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(
        m_touchListener, 1);
}

void TutorialInputHandler::resolveInput(const cocos2d::Vec2& screenPos, InputType inputType)
{
    const auto curTick = utils::clock::getCurrentTick();
    const auto dt = curTick - std::get<0>(m_lastInput);

    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    Side side = (screenPos.x < visibleSize.width / 2) ? Side::left : Side::right;
    auto prevSide = std::get<1>(m_lastInput);
    auto prevInput = std::get<2>(m_lastInput);
    m_lastInput = std::make_tuple(curTick, side, inputType);
    if (prevInput == InputType::jump && inputType == InputType::jump && side == prevSide &&
        dt <= 1.0 && m_doubleJumpCallback) {
        m_doubleJumpCallback();
        return;
    }
    if (prevInput == InputType::jump && inputType == InputType::dive && side == prevSide &&
        dt <= 1.0 && m_diveCallback) {
        m_diveCallback();
        return;
    }
    if (side == Side::left && m_leftJumpCallback) {
        m_leftJumpCallback();
        return;
    }
    else if (side == Side::right && m_rightJumpCallback) {
        m_rightJumpCallback();
        return;
    }
}

void TutorialInputHandler::resetCallbacks()
{
    m_doubleJumpCallback = nullptr;
    m_diveCallback = nullptr;
    m_leftJumpCallback = nullptr;
    m_rightJumpCallback = nullptr;
    resetLastClick(true);
}

void TutorialInputHandler::resetLastClick(const bool hardReset)
{
    if (hardReset) {
        m_lastInput = std::make_tuple(-1.0f, Side::left, InputType::jump);
        return;
    }
    m_lastInput = std::make_tuple(utils::clock::getCurrentTick(), std::get<1>(m_lastInput),
                                  std::get<2>(m_lastInput));
}

void TutorialInputHandler::setClickCallback(std::function<void()> cb)
{
    m_leftJumpCallback = cb;
    m_rightJumpCallback = cb;
}

void TutorialInputHandler::setClickCallback(std::function<void()> cb, float waitSeconds)
{
    auto delay = cocos2d::DelayTime::create(waitSeconds);
    auto setCallback = cocos2d::CallFunc::create([this, cb]() {
        m_leftJumpCallback = cb;
        m_rightJumpCallback = cb;
    });
    m_actionNode->runAction(cocos2d::Sequence::create(delay, setCallback, nullptr));
}

TutorialInputHandler::~TutorialInputHandler()
{
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(m_touchListener);
}
