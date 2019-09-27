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

#ifndef __BUNNY_CONTROLLER_H__
#define __BUNNY_CONTROLLER_H__

#include <Bunny.h>
#include <array>
#include "cocos2d.h"

class BunnyController {
public:
    bool init(cocos2d::Scene& scene);
    bool spawnBunny(const cocos2d::Vec2& pos);
    bool jumpBunny(const cocos2d::Vec2& pos);

private:
    // Bunny_id equals to container index
    std::array<Bunny, 5> m_bunnyContainer{Bunny{0}, Bunny{1}, Bunny{2}, Bunny{3}, Bunny{4}};
    std::vector<std::uint8_t> m_freeBunnies{0, 1, 2, 3, 4};
};

#endif  // __BUNNY_CONTROLLER_H__
