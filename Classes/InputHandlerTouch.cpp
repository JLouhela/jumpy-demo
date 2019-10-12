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

#include "InputHandlerTouch.h"
#include "BunnyController.h"
#include "Clock.h"

void InputHandler::init(BunnyController& bunnyController)
{
    m_bunnyController = &bunnyController;

    m_touchListener = cocos2d::EventListenerTouchOneByOne::create();
    m_touchListener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
        if (!m_enabled) {
            return false;
        }
        auto location = touch->getLocationInView();
        location.y = cocos2d::Director::getInstance()->getVisibleSize().height - location.y;
        m_touchBegin = std::make_pair(utils::clock::getCurrentTick(), location);
        return true;
    };
    m_touchListener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
        if (!m_enabled || m_touchBegin.first < 0.0) {
            return false;
        }
        auto location = touch->getLocationInView();
        location.y = cocos2d::Director::getInstance()->getVisibleSize().height - location.y;
        m_touchBegin.first = -1;
        return resolveInput(location, InputType::jump);
    };
    m_touchListener->onTouchMoved = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
        if (!m_enabled || m_touchBegin.first < 0.0) {
            return false;
        }
        auto curY = cocos2d::Director::getInstance()->getVisibleSize().height -
                    touch->getLocationInView().y;
        InputType type = InputType::jump;
        static constexpr float swipeThreshold{1.0f};
        if (m_touchBegin.second.y - curY > swipeThreshold) {
            type = InputType::dive;
        }
        m_touchBegin.first = -1;
        return resolveInput(m_touchBegin.second, type);
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

void InputHandler::update(float /*dt*/)
{
    if (!m_enabled || m_touchBegin.first < 0.0) {
        return;
    }
    const auto tick = utils::clock::getCurrentTick();
    static constexpr float resolveTouchTime{1.0f / 30};
    if (tick - m_touchBegin.first > resolveTouchTime) {
        resolveInput(m_touchBegin.second, InputType::jump);
        m_touchBegin.first = -1.0;
    }
}

void InputHandler::disable()
{
    m_touchBegin.first = -1.0;
    m_enabled = false;
}
