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

#include "InputHandler.h"
#include "BunnyController.h"
#include "Clock.h"

namespace {
constexpr float swipeThreshold{10.0f};

InputHandler::Side getSide(const cocos2d::Vec2& pos)
{
    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    return (pos.x < visibleSize.width / 2) ? InputHandler::Side::left : InputHandler::Side::right;
}

std::uint8_t getIndex(const InputHandler::Side side)
{
    return (side == InputHandler::Side::left) ? 0 : 1;
}

}  // namespace

void InputHandler::init(BunnyController& bunnyController)
{
    m_bunnyController = &bunnyController;

    m_touchListener = cocos2d::EventListenerTouchAllAtOnce::create();
    m_touchListener->onTouchesBegan = [this](const std::vector<cocos2d::Touch*>& touches,
                                             cocos2d::Event* event) {
        bool leftHandled{!m_enabled[getIndex(Side::left)]};
        bool rightHandled{!m_enabled[getIndex(Side::right)]};

        for (const auto touch : touches) {
            if (leftHandled && rightHandled) {
                break;
            }
            auto location = touch->getLocationInView();
            location.y = cocos2d::Director::getInstance()->getVisibleSize().height - location.y;
            const auto side = getSide(location);
            if ((side == Side::left && leftHandled) || (side == Side::right && rightHandled)) {
                continue;
            }
            m_touchBegins[getIndex(side)] =
                std::make_pair(utils::clock::getCurrentTick(), location);

            leftHandled = leftHandled || (side == Side::left);
            rightHandled = rightHandled || (side == Side::right);
        }
        return true;
    };
    m_touchListener->onTouchesEnded = [this](const std::vector<cocos2d::Touch*>& touches,
                                             cocos2d::Event* event) {
        for (const auto& touch : touches) {
            resetCurrentTouch(getSide(touch->getLocationInView()));
        }
        return true;
    };
    m_touchListener->onTouchesMoved = [this](const std::vector<cocos2d::Touch*>& touches,
                                             cocos2d::Event* event) {
        if (!isTouchActive()) {
            return false;
        }

        bool leftHandled{!m_enabled[getIndex(Side::left)]};
        bool rightHandled{!m_enabled[getIndex(Side::right)]};
        for (const auto touch : touches) {
            if (leftHandled && rightHandled) {
                break;
            }
            auto location = touch->getLocationInView();
            location.y = cocos2d::Director::getInstance()->getVisibleSize().height - location.y;

            const auto side = getSide(location);
            if ((side == Side::left && leftHandled) || (side == Side::right && rightHandled)) {
                continue;
            }
            m_curTouches[getIndex(side)] = std::make_pair(true, location);

            leftHandled = leftHandled || (side == Side::left);
            rightHandled = rightHandled || (side == Side::right);
        }
        return true;
    };
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(
        m_touchListener, 2);
    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 1, false);
}

bool InputHandler::resolveInput(const cocos2d::Vec2& screenPos, InputType inputType)
{
    if (inputType == InputType::jump) {
        return m_bunnyController->jumpBunny(screenPos);
    }
    return m_bunnyController->diveBunny(screenPos);
}

InputHandler::~InputHandler()
{
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(m_touchListener);
}

void InputHandler::resetCurrentTouch(Side side)
{
    const auto idx = getIndex(side);
    m_curTouches[idx].first = false;
    m_touchBegins[idx].first = -1.0;
}

void InputHandler::resetCurrentTouches()
{
    resetCurrentTouch(Side::left);
    resetCurrentTouch(Side::right);
}

bool InputHandler::isTouchActive()
{
    bool touchBegin{false};
    for (const auto& beginTouch : m_touchBegins) {
        if (beginTouch.first > 0.0) {
            return true;
        }
    }
    return false;
}

void InputHandler::update(float /*dt*/)
{
    if (!isTouchActive()) {
        return;
    }
    if (m_enabled[getIndex(Side::left)]) {
        resolveTouch(Side::left);
    }
    if (m_enabled[getIndex(Side::right)]) {
        resolveTouch(Side::right);
    }
}

void InputHandler::disable()
{
    resetCurrentTouches();
    m_enabled = {false, false};
}

void InputHandler::disable(Side side)
{
    m_enabled[getIndex(side)] = false;
}

void InputHandler::enable(Side side)
{
    m_enabled[getIndex(side)] = true;
}

bool InputHandler::resolveTouch(Side side)
{
    InputType type = InputType::jump;
    const auto idx = getIndex(side);
    if (m_touchBegins[idx].first < 0.0 || !m_curTouches[idx].first) {
        return false;
    }

    if (m_touchBegins[idx].second.y - m_curTouches[idx].second.y > swipeThreshold) {
        type = InputType::dive;
    }
    else if (m_curTouches[idx].second.y - m_touchBegins[idx].second.y > swipeThreshold) {
        type = InputType::jump;
    }
    else {
        return false;
    }
    resetCurrentTouch(side);
    return resolveInput(m_curTouches[idx].second, type);
}
