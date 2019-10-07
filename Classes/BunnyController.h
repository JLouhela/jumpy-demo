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

#include <array>
#include <cstdint>
#include <utility>
#include "Bunny.h"
#include "cocos2d.h"

class b2World;

class BunnyController {
public:
    static constexpr std::uint8_t bunnyCount{2};

    bool init(cocos2d::Scene& scene, b2World& world);
    void spawnBunnies();
    void disposeBunnies();
    bool jumpBunny(const cocos2d::Vec2& pos);

private:
    std::vector<cocos2d::Vec2> BunnyController::getSpawnPoints(const std::uint8_t bunnyCount);

    // Bunny_id equals to container index.
    // Hardcoded init sufficient for demonstration purposes.
    std::array<Bunny, bunnyCount> m_bunnyContainer;
};

#endif  // __BUNNY_CONTROLLER_H__
