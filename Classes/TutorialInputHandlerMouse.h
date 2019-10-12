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

#ifndef __TUTORIAL_INPUT_HANDLER_H__
#define __TUTORIAL_INPUT_HANDLER_H__

#include <functional>
#include <tuple>
#include "cocos2d.h"

class BunnyController;

class TutorialInputHandler : public TutorialInputHandler {
public:
    TutorialInputHandler();
    ~TutorialInputHandler();

    void setClickCallback(std::function<void()> cb);

    void setLeftJumpCallback(std::function<void()> cb)
    {
        m_leftJumpCallback = cb;
    }

    void setRightJumpCallback(std::function<void()> cb)
    {
        m_rightJumpCallback = cb;
    }
    void setDoubleJumpCallback(std::function<void()> cb)
    {
        m_doubleJumpCallback = cb;
    }
    void setDiveCallback(std::function<void()> cb)
    {
        m_diveCallback = cb;
    }

    void resetCallbacks();
    void resetLastClick(bool hardReset);

private:
    enum class Side : bool { left, right };
    enum class InputType : bool { jump, dive };
    void resolveInput(const cocos2d::Vec2& screenPos, InputType inputType);

    cocos2d::EventListenerTouchOneByOne* m_touchListener{nullptr};
    cocos2d::EventListenerMouse* m_mouseListener{nullptr};
    std::function<void()> m_leftJumpCallback{nullptr};
    std::function<void()> m_rightJumpCallback{nullptr};
    std::function<void()> m_doubleJumpCallback{nullptr};
    std::function<void()> m_diveCallback{nullptr};
    std::tuple<double, Side, InputType> m_lastClick{-1.0f, Side::left, InputType::jump};
};

#endif  // __TUTORIAL_INPUT_HANDLER_H__
