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

#ifndef __BUNNY_H__
#define __BUNNY_H__

#include <cstdint>
#include "cocos2d.h"

// Potential refactoring: bee and bunny follow the same pattern
// Base class extraction: "PooledEntity"

using Bunny_id = std::int8_t;
static constexpr Bunny_id invalidBunnyId{-1};

class Bunny {
public:
    bool init(Bunny_id id, cocos2d::Scene& scene);

    Bunny_id getId() const
    {
        return m_id;
    }

    void dispose();

    void resetSprite();

    void activate(const cocos2d::Vec2& pos);

    void jump();

    const cocos2d::Vec2& getPosition() const;

    const cocos2d::Rect getBoundingBox() const;

    cocos2d::Sprite* getSprite() const
    {
        return m_sprite;
    }

private:
    cocos2d::Sprite* m_sprite{nullptr};
    cocos2d::PhysicsBody* m_physicsBody{nullptr};
    Bunny_id m_id{invalidBunnyId};
    cocos2d::Vec2 m_pos{-100, -100};
};

#endif  // __BUNNY_H__
