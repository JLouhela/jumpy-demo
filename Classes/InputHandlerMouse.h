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

#include "cocos2d.h"

class BunnyController;

class InputHandler {
public:
    void init(BunnyController& bunnyController);

    void disable()
    {
        m_enabled = false;
    }

    void enable()
    {
        m_enabled = true;
    }

    ~InputHandler();

private:
    enum class InputType : bool { jump, dive };

    bool resolveInput(const cocos2d::Vec2& screenPos, InputType inputType);

    cocos2d::EventListenerMouse* m_mouseListener{nullptr};
    cocos2d::EventListenerTouchOneByOne* m_touchListener{nullptr};
    BunnyController* m_bunnyController{nullptr};
    bool m_enabled{false};
};

#endif  // __INPUT_HANDLER_H__
