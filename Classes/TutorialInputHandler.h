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

class TutorialInputHandler {
public:
    ~TutorialInputHandler();

    void init(cocos2d::Scene& scene);

    void setClickCallback(std::function<void()> cb)
    {
        m_clickCallback = cb;
    }

    void setClickCallback(std::function<void()> cb, float waitSeconds);

    void setJumpCallback(std::function<void()> cb)
    {
        m_jumpCallback = cb;
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

private:
    cocos2d::EventListenerTouchOneByOne* m_touchListener{nullptr};
    cocos2d::EventListenerCustom* m_jumpListener{nullptr};
    cocos2d::EventListenerCustom* m_doubleJumpListener{nullptr};
    cocos2d::EventListenerCustom* m_diveListener{nullptr};

    std::function<void()> m_clickCallback{nullptr};
    std::function<void()> m_jumpCallback{nullptr};
    std::function<void()> m_doubleJumpCallback{nullptr};
    std::function<void()> m_diveCallback{nullptr};
    cocos2d::Node* m_actionNode{nullptr};
};

#endif  // __TUTORIAL_INPUT_HANDLER_H__
