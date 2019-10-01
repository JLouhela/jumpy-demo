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

bool InputHandler::init(BunnyController& bunnyController)
{
    m_bunnyController = &bunnyController;

    m_touchListener = cocos2d::EventListenerTouchOneByOne::create();

    m_touchListener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
        if (!m_enabled) {
            return true;
        }
        auto location = touch->getLocationInView();
        location.y = cocos2d::Director::getInstance()->getVisibleSize().height - location.y;
        return resolveInput(location);  // consume if an action was taken
    };

    // Separate by build target..? If no issues, don't bother for the demo.
    // These are only input listeners in the scene, fixed priority ok.
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(
        m_touchListener, 2);
    return true;
}

bool InputHandler::resolveInput(const cocos2d::Vec2& screenPos)
{
    return m_bunnyController->jumpBunny(screenPos);
}

InputHandler::~InputHandler()
{
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(m_touchListener);
}
