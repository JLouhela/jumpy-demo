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
#include "Box2D/Box2D.h"
#include "Box2DUtils.h"
#include "CollisionGroup.h"
#include "CustomEvents.h"

namespace {

constexpr float velocity{5.0f};

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

bool Bee::init(const Bee_id id, b2World& world)
{
    m_sprite = loadSprite();
    // If ctor fails, leave bee to fail state
    if (!m_sprite) {
        m_id = invalidBeeId;
        return false;
    }

    // Physics init
    b2BodyDef beeBody{};
    beeBody.type = b2_dynamicBody;
    beeBody.angle = 0;
    beeBody.userData = &m_physicsObject;
    beeBody.fixedRotation = true;
    beeBody.gravityScale = 0.0f;
    m_body = world.CreateBody(&beeBody);

    b2FixtureDef beeFixtureDef;
    const auto spriteSize = m_sprite->getContentSize();
    const static cocos2d::Vec2 relativeBodySize{0.9f, 0.8f};

    auto boxShape = utils::box2d::getBoxShape(
        {spriteSize.width * relativeBodySize.x, spriteSize.height * relativeBodySize.y});
    beeFixtureDef.shape = &boxShape;
    beeFixtureDef.density = 1;
    beeFixtureDef.filter.categoryBits = CollisionGroup::bee;
    beeFixtureDef.filter.maskBits = 0x00;
    m_body->CreateFixture(&beeFixtureDef);

    m_physicsObject.sprite = m_sprite;
    m_physicsObject.collisionResolvedCallback = [this](const std::uint8_t colliderGroup) -> bool {
        if ((colliderGroup & m_body->GetFixtureList()->GetFilterData().maskBits) != 0) {
            cocos2d::EventCustom event(CustomEvent::beeThroughEvent);
            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
            // TODO play sound?
            return true;
        }
        return false;
    };
    dispose();
    return true;
}

void Bee::dispose()
{
    m_body->SetLinearVelocity({0.0f, 0.0f});
    m_physicsObject.active = false;
    m_sprite->setVisible(false);
    m_state = BeeState::inactive;
}

void Bee::spawn(const cocos2d::Vec2& pos, Direction dir)
{
    m_physicsObject.active = true;
    m_sprite->setPosition(pos);
    m_sprite->setVisible(true);
    m_body->SetTransform(utils::box2d::pixelsToMeters(m_sprite->getPosition()), 0.0f);
    static const b2Vec2 velocityLeft{-velocity, 0.0f};
    static const b2Vec2 velocityRight{velocity, 0.0f};
    m_body->SetLinearVelocity(dir == Direction::right ? velocityRight : velocityLeft);
    b2Filter filter = m_body->GetFixtureList()->GetFilterData();
    filter.maskBits =
        CollisionGroup::bunny +
        (dir == Direction::right ? CollisionGroup::rightBorder : CollisionGroup::leftBorder);
    m_body->GetFixtureList()->SetFilterData(filter);

    m_sprite->setFlippedX(dir == Direction::right);
}

void Bee::activate()
{
    m_state = BeeState::active;
}
