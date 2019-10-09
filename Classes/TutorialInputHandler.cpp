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

namespace {

double getCurrentTick()
{
    static struct timeval currentTime;
    cocos2d::gettimeofday(&currentTime, nullptr);
    return (currentTime.tv_sec) + (currentTime.tv_usec / 1000000.0);
}
}  // namespace

TutorialInputHandler::TutorialInputHandler()
{
#ifdef JUMPY_USE_MOUSE
    m_mouseListener = cocos2d::EventListenerMouse::create();
    m_mouseListener->onMouseDown = [this](cocos2d::EventMouse* event) {
        const auto location = event->getLocationInView();
        InputType inputType =
            (event->getMouseButton() == cocos2d::EventMouse::MouseButton::BUTTON_LEFT)
                ? InputType::jump
                : InputType::dive;
        resolveInput(location, inputType);
        return false;  // never consume
    };
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(
        m_mouseListener, 1);
#else

    // TODO swipe handling
    m_touchListener = cocos2d::EventListenerTouchOneByOne::create();
    m_touchListener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
        auto location = touch->getLocationInView();
        location.y = cocos2d::Director::getInstance()->getVisibleSize().height - location.y;
        return false;  // Never consume here
    };
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(
        m_touchListener, 1);
#endif
}

void TutorialInputHandler::resolveInput(const cocos2d::Vec2& screenPos, InputType inputType)
{
    const auto curTick = getCurrentTick();
    const auto dt = m_lastClick.first - curTick;

    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    Side side = (screenPos.x < visibleSize.width / 2) ? Side::left : Side::right;
    m_lastClick = std::make_pair(curTick, side);
    if (dt <= 2.0 && m_doubleJumpCallback) {
        m_doubleJumpCallback();
    }
    if (inputType == InputType::dive && m_diveCallback) {
        m_diveCallback();
    }
    if (side == Side::left && m_leftJumpCallback) {
        m_leftJumpCallback();
    }
    if (m_rightJumpCallback) {
        m_rightJumpCallback();
    }
}

TutorialInputHandler::~TutorialInputHandler()
{
#ifdef JUMPY_USE_MOUSE
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(m_mouseListener);
#else
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(m_touchListener);
#endif
}
