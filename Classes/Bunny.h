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
#include "PhysicsObject.h"
#include "cocos2d.h"

class b2World;
class b2Body;

// Potential refactoring: bee and bunny follow the same pattern
// Base class extraction: "PooledEntity"

using Bunny_id = std::int8_t;
static constexpr Bunny_id invalidBunnyId{-1};

class Bunny {
public:
    bool init(Bunny_id id, cocos2d::Scene& scene, b2World& world);

    Bunny_id getId() const
    {
        return m_id;
    }

    void dispose();

    void activate(const cocos2d::Vec2& pos);

    void jump();

    const cocos2d::Rect getBoundingBox() const;

    cocos2d::Sprite* getSprite() const
    {
        return m_sprite;
    }

private:
    enum class BunnyState : std::uint8_t {
        grounded,    // Landed on ground
        jumped,      // Has jumped once
        doublejump,  // Has jumped twice
        dash         // Dashing downwards
    };
    void land();

    cocos2d::Sprite* m_sprite{nullptr};
    b2Body* m_body{nullptr};
    Bunny_id m_id{invalidBunnyId};
    PhysicsObject m_physicsObject;
    BunnyState m_state{BunnyState::doublejump};  // use doublejump as default to prevent jumping if
                                                 // bunny spawns in free fall
};

#endif  // __BUNNY_H__
