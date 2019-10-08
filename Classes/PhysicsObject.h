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

#ifndef __PHYSICS_OBJECT_H__
#define __PHYSICS_OBJECT_H__

#include <cstdint>
#include <functional>
#include "Box2D/Box2D.h"

namespace cocos2d {
class Sprite;
}

struct b2Vec2;

static constexpr float invalidCoord{-100.0f};

struct PhysicsObject {
    cocos2d::Sprite* sprite{nullptr};
    b2Vec2 prevPos{invalidCoord, invalidCoord};
    b2Vec2 hiddenPos{invalidCoord, invalidCoord};
    bool active{false};
    std::function<bool(std::uint8_t colliderGroup)> collisionOccurredCallback{nullptr};
    std::function<bool(std::uint8_t colliderGroup)> collisionResolvedCallback{nullptr};
};

#endif  // __PHYSICS_OBJECT_H__
