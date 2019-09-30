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
#include "CollisionGroup.h"

namespace {

cocos2d::Sprite* loadSprite()
{
    const auto beeSpriteFrame{
        cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("./bee_32_28")};
    auto beeSprite{cocos2d::Sprite::createWithSpriteFrame(beeSpriteFrame)};
    if (beeSprite == nullptr) {
        cocos2d::log("Could not load bee_32_28");
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
    m_physicsBody->setCategoryBitmask(CollisionGroup::bee);
    m_physicsBody->setVelocityLimit(150.0f);
    m_physicsBody->setContactTestBitmask(CollisionGroup::bunny + CollisionGroup::border);
    m_sprite->setUserData(this);
    m_sprite->addComponent(m_physicsBody);

    dispose();
}

void Bee::dispose()
{
    m_physicsBody->setEnabled(false);
    m_sprite->setVisible(false);
    m_sprite->setPosition(cocos2d::Vec2{-500, -500});
    m_state = BeeState::inactive;
}

void Bee::spawn(const cocos2d::Vec2& pos, direction dir)
{
    m_sprite->setPosition(pos);
    m_sprite->setVisible(true);
    m_physicsBody->setEnabled(true);
    const float velocityX = (dir == direction::right) ? m_physicsBody->getVelocityLimit()
                                                      : (m_physicsBody->getVelocityLimit() * -1);
    m_physicsBody->setVelocity(cocos2d::Vec2{velocityX, 0});
}

void Bee::activate()
{
    m_state = BeeState::active;
}
