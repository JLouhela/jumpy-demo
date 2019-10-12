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

#ifndef __INPUT_HANDLER_H__
#define __INPUT_HANDLER_H__

#include <array>
#include <utility>
#include "cocos2d.h"

class BunnyController;

class InputHandler {
public:
    enum class Side : bool { left, right };
    void init(BunnyController& bunnyController);

    void disable();
    void disable(Side side);

    void enable()
    {
        m_enabled = {true, true};
    }

    void enable(Side side);

    void update(float dt);

    ~InputHandler();

private:
    enum class InputType : bool { jump, dive };

    bool resolveTouch(Side side);
    bool resolveInput(const cocos2d::Vec2& screenPos, InputType inputType);
    void resetCurrentTouch(Side side);
    void resetCurrentTouches();
    bool isTouchActive();

    cocos2d::EventListenerTouchAllAtOnce* m_touchListener{nullptr};
    BunnyController* m_bunnyController{nullptr};
    std::array<bool, 2> m_enabled{false, false};

    std::array<std::pair<double, cocos2d::Vec2>, 2> m_touchBegins{
        std::pair<double, cocos2d::Vec2>{-1.0, cocos2d::Vec2{}},
        std::pair<double, cocos2d::Vec2>{-1.0, cocos2d::Vec2{}}};

    std::array<std::pair<bool, cocos2d::Vec2>, 2> m_curTouches{
        std::pair<double, cocos2d::Vec2>{false, cocos2d::Vec2{}},
        std::pair<double, cocos2d::Vec2>{false, cocos2d::Vec2{}}};
};

#endif  // __INPUT_HANDLER_H__
