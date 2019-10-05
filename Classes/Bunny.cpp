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

constexpr float velocityLimitUp{350.0f};
constexpr float velocityLimitDown{500.0f};

cocos2d::Sprite* loadSprite()
{
    const auto bunnySpriteFrame{
        cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("./bunny_stand_96_48")};
    auto bunnySprite{cocos2d::Sprite::createWithSpriteFrame(bunnySpriteFrame)};
    if (bunnySprite == nullptr) {
        cocos2d::log("Could not load bunny_stand_96_48");
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
    /*
    TODO: Fix to use Box2D, see all commented lines

m_physicsBody = cocos2d::PhysicsBody::createBox(cocos2d::Size(36, 84),
                                                cocos2d::PhysicsMaterial(1.0f, 0.01f, 0.0f));
m_physicsBody->setDynamic(true);
m_physicsBody->setGravityEnable(true);
m_physicsBody->setVelocityLimit(velocityLimitUp);
m_physicsBody->setRotationEnable(false);
m_physicsBody->setCategoryBitmask(CollisionGroup::bunny);
m_physicsBody->setCollisionBitmask(CollisionGroup::ground);
m_physicsBody->setContactTestBitmask(CollisionGroup::bee + CollisionGroup::ground);
m_sprite->addComponent(m_physicsBody);
m_sprite->setUserData(this);
    */
    dispose();
    return true;
}

void Bunny::activate(const cocos2d::Vec2& pos)
{
    // No nullcheck, BunnyController shall ensure that only initialized bunnies are accessed
    m_sprite->setVisible(true);
    m_sprite->setPosition(pos);
    // m_physicsBody->setEnabled(true);
}

const cocos2d::Rect Bunny::getBoundingBox() const
{
    return m_sprite->getBoundingBox();
}

void Bunny::jump()
{
    if ((m_state == BunnyState::jumped || m_state == BunnyState::doublejump)  /* &&
        m_physicsBody->getVelocity().y < -25.0f*/) {
        // Downwards dash
        m_state = BunnyState::dash;
        // Allow more speed downwards
        // m_physicsBody->setVelocityLimit(velocityLimitDown);
        // m_physicsBody->applyImpulse(cocos2d::Vec2{0.0f, -4000000.0f});
        m_sprite->setFlippedY(true);
        return;
    }

    if (m_state == BunnyState::grounded || m_state == BunnyState::jumped) {
        m_state = (m_state == BunnyState::grounded) ? BunnyState::jumped : BunnyState::doublejump;
        // m_physicsBody->applyImpulse(cocos2d::Vec2{0.0f, 4000000.0f});
        const auto spriteFrame{
            cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("./bunny_jump_96_48")};
        m_sprite->setSpriteFrame(spriteFrame);
        m_sprite->setFlippedY(false);
        return;
    }
}

void Bunny::resetState()
{
    const auto spriteFrame{
        cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("./bunny_stand_96_48")};
    m_sprite->setSpriteFrame(spriteFrame);
    m_sprite->setFlippedY(false);
    // m_physicsBody->setVelocityLimit(velocityLimitUp);

    m_state = BunnyState::grounded;
}

void Bunny::dispose()
{
    m_state = BunnyState::doublejump;
    m_sprite->setVisible(false);
    // m_physicsBody->setEnabled(false);
    // m_physicsBody->resetForces();
    // m_physicsBody->setVelocity(cocos2d::Vec2{0, 0});
}
