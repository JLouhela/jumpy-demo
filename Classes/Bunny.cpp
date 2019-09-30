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

#include "Bunny.h"
#include "CollisionGroup.h"

namespace {

cocos2d::Sprite* loadSprite()
{
    const auto bunnySpriteFrame{
        cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("./bunny_stand_64_32")};
    auto bunnySprite{cocos2d::Sprite::createWithSpriteFrame(bunnySpriteFrame)};
    if (bunnySprite == nullptr) {
        cocos2d::log("Could not load bunny_stand_64_32");
    }
    return bunnySprite;
}

}  // namespace
bool Bunny::init(Bunny_id id, cocos2d::Scene& scene)
{
    m_sprite = loadSprite();

    if (!m_sprite) {
        return false;
    }
    m_id = id;
    // Add physics body
    m_physicsBody = cocos2d::PhysicsBody::createBox(cocos2d::Size(32, 64),
                                                    cocos2d::PhysicsMaterial(1.0f, 0.01f, 0.0f));
    m_physicsBody->setDynamic(true);
    m_physicsBody->setGravityEnable(true);
    m_physicsBody->setVelocityLimit(300.0f);
    m_physicsBody->setRotationEnable(false);
    m_physicsBody->setCategoryBitmask(CollisionGroup::bunny);
    m_physicsBody->setCollisionBitmask(CollisionGroup::ground);
    m_physicsBody->setContactTestBitmask(CollisionGroup::bee + CollisionGroup::ground);
    m_sprite->addComponent(m_physicsBody);
    m_sprite->setUserData(this);
    dispose();
    return true;
}

const cocos2d::Vec2& Bunny::getPosition() const
{
    // Redundancy vs. mental model: feels weird to have position only on sprite
    // but feels weird to store position for no reason. Decide after other use cases for pos.
    return m_sprite->getPosition();
}

void Bunny::activate(const cocos2d::Vec2& pos)
{
    // No nullcheck, BunnyController shall ensure that only initialized bunnies are accessed
    m_sprite->setPosition(pos);
    m_physicsBody->setEnabled(true);
}

const cocos2d::Rect Bunny::getBoundingBox() const
{
    return m_sprite->getBoundingBox();
}

void Bunny::jump()
{
    if (m_state == BunnyState::grounded || m_state == BunnyState::jump) {
        m_state = (m_state == BunnyState::grounded) ? BunnyState::jump : BunnyState::doublejump;
        m_physicsBody->applyImpulse(cocos2d::Vec2{0.0f, 2000000.0f});
        const auto spriteFrame{
            cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("./bunny_jump_64_32")};
        m_sprite->setSpriteFrame(spriteFrame);
        m_sprite->setFlippedY(false);
    }
    else if ((m_state == BunnyState::jump || m_state == BunnyState::doublejump) &&
             m_physicsBody->getVelocity().y < 0) {
        // Downwards dash
        m_state = BunnyState::dash;
        m_physicsBody->applyImpulse(cocos2d::Vec2{0.0f, -1500.0f});
        m_sprite->setFlippedY(true);
    }
}

void Bunny::resetState()
{
    const auto spriteFrame{
        cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("./bunny_stand_64_32")};
    m_sprite->setSpriteFrame(spriteFrame);
    m_sprite->setFlippedY(false);

    m_state = BunnyState::grounded;
}

void Bunny::dispose()
{
    m_physicsBody->setEnabled(false);
    m_physicsBody->resetForces();
    m_physicsBody->setVelocity(cocos2d::Vec2{0, 0});
    m_sprite->setPosition(cocos2d::Vec2{-100, -100});
}
