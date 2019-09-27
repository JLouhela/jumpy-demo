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

namespace {

cocos2d::Sprite* loadSprite()
{
    const auto bunnySpriteFrame{
        cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("./bunny_sit_32_32")};
    auto bunnySprite{cocos2d::Sprite::createWithSpriteFrame(bunnySpriteFrame)};
    if (bunnySprite == nullptr) {
        cocos2d::log("Could not load bunny_sit_32_32");
    }
    return bunnySprite;
}

}  // namespace

Bunny::Bunny(const Bunny_id id) : m_id{id}, m_sprite{loadSprite()}
{
    // If ctor fails, put bunny to fail state
    if (!m_sprite) {
        m_id = invalidBunnyId;
        return;
    }
    // Add physics body
    m_physicsBody = cocos2d::PhysicsBody::createBox(cocos2d::Size(32, 32),
                                                    cocos2d::PhysicsMaterial(1.0f, 0.1f, 0.0f));
    m_physicsBody->setDynamic(true);
    m_physicsBody->setGravityEnable(false);
    m_physicsBody->setVelocityLimit(300.0f);
    m_sprite->addComponent(m_physicsBody);

    setPosition(cocos2d::Vec2{-100, -100});
}

const cocos2d::Vec2& Bunny::getPosition() const
{
    // Redundancy vs. mental model: feels weird to have position only on sprite
    // but feels weird to store position for no reason. Decide after other use cases for pos.
    return m_sprite->getPosition();
}

void Bunny::setPosition(const cocos2d::Vec2& pos)
{
    // No nullcheck, BunnyController shall ensure that only initialized bunnies are accessed
    m_sprite->setPosition(pos);
    m_physicsBody->setGravityEnable(true);
}

const cocos2d::Rect Bunny::getBoundingBox() const
{
    return m_sprite->getBoundingBox();
}

void Bunny::jump()
{
    if (m_physicsBody->getVelocity().y >= 0.0f) {
        // Jump
        m_physicsBody->applyImpulse({0.0f, m_physicsBody->getMass() * 150.0f});
    }
    else {
        // Downwards dash
        m_physicsBody->applyImpulse({0.0f, m_physicsBody->getMass() * -150.0f});
    }
}
