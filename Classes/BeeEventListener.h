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

#ifndef __BEE_EVENT_LISTENER_H__
#define __BEE_EVENT_LISTENER_H__

#include <cstdint>
#include <functional>
#include <utility>
#include "cocos2d.h"

class BeeEventListener {
public:
    void init(cocos2d::Scene& scene, std::function<void()> callback);

    void wait(std::uint8_t count);
    void reset();

private:
    cocos2d::EventListenerCustom* m_beeListener{nullptr};
    std::pair<bool, std::uint8_t> m_waitingCount{std::make_pair(false, 0)};
    std::function<void()> m_callback;
};

#endif  // __BEE_EVENT_LISTENER_H__
