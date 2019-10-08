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

#ifndef __BEE_H__
#define __BEE_H__

#include <cstdint>
#include "Direction.h"
#include "PhysicsObject.h"
#include "cocos2d.h"

class b2World;
class b2Body;

using Bee_id = std::int8_t;
static constexpr Bee_id invalidBeeId{-1};

enum class BeeState : std::uint8_t { active, inactive };

class Bee {
public:
    bool init(Bee_id id, b2World& world);

    Bee_id getId() const
    {
        return m_id;
    }

    BeeState getState() const
    {
        return m_state;
    }

    void activate();

    cocos2d::Sprite* getSprite() const
    {
        return m_sprite;
    }

    void dispose();

    void spawn(const cocos2d::Vec2& pos, Direction dir);

private:
    cocos2d::Sprite* m_sprite{nullptr};
    b2Body* m_body{nullptr};
    PhysicsObject m_physicsObject;
    Bee_id m_id{invalidBeeId};
    cocos2d::Vec2 m_pos{-500, -500};
    BeeState m_state{BeeState::inactive};
};

#endif  // __BEE_H__
