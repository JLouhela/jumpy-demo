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
#include "cocos2d.h"

using Bee_id = std::int8_t;
static constexpr Bee_id invalidBeeId{-1};

enum class direction : std::uint8_t { left, right };

class Bee {
public:
    explicit Bee(Bee_id id);

    Bee_id getId() const
    {
        return m_id;
    }

    cocos2d::Sprite* getSprite() const
    {
        return m_sprite;
    }

    void dispose();

    void spawn(const cocos2d::Vec2& pos, direction dir);

private:
    cocos2d::Sprite* m_sprite{nullptr};
    cocos2d::PhysicsBody* m_physicsBody{nullptr};
    Bee_id m_id{invalidBeeId};
    cocos2d::Vec2 m_pos{-100, -100};
};

#endif  // __BEE_H__
