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

#include "Bee.h"

namespace {

cocos2d::Sprite* loadSprite()
{
    const auto beeSpriteFrame{
        cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("./bee_16_16")};
    auto beeSprite{cocos2d::Sprite::createWithSpriteFrame(beeSpriteFrame)};
    if (beeSprite == nullptr) {
        cocos2d::log("Could not load bee_16_16");
    }
    return beeSprite;
}

}  // namespace

Bee::Bee(const Bee_id id) : m_id{id}, m_sprite{loadSprite()}
{
    // If ctor fails, put bee to fail state
    if (!m_sprite) {
        m_id = invalidBeeId;
        return;
    }
    // Add physics body
    m_physicsBody = cocos2d::PhysicsBody::createBox(cocos2d::Size(16, 16),
                                                    cocos2d::PhysicsMaterial(0.1f, 0.1f, 0.0f));
    m_physicsBody->setDynamic(true);
    m_physicsBody->setGravityEnable(false);
    m_sprite->addComponent(m_physicsBody);

    // TODO add collider to each side of screen, dispose on_collision and notify game logic somehow
    dispose();
}

void Bee::dispose()
{
    m_physicsBody->resetForces();
    m_sprite->setPosition(cocos2d::Vec2{-500, -500});
}

void Bee::spawn(const cocos2d::Vec2& pos, direction dir)
{
    m_sprite->setPosition(pos);
    static constexpr float beeForce = 200000.0f;
    float xForce = (dir == direction::right) ? beeForce : (beeForce * -1);
    m_physicsBody->applyForce(cocos2d::Vec2{xForce, 0});
}
