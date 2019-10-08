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
#include "Box2D/Box2D.h"
#include "Box2DUtils.h"
#include "CollisionGroup.h"
#include "CustomEvents.h"
#include "PTM.h"

namespace {
const cocos2d::Vec2 relativeBodySize{0.95f, 0.9f};
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
bool Bunny::init(Bunny_id id, cocos2d::Scene& scene, b2World& world)
{
    m_sprite = loadSprite();

    if (!m_sprite) {
        return false;
    }
    m_id = id;

    // Physics init
    b2BodyDef bunnyBody{};
    bunnyBody.type = b2_dynamicBody;
    bunnyBody.angle = 0;
    bunnyBody.userData = &m_physicsObject;
    bunnyBody.fixedRotation = true;
    m_body = world.CreateBody(&bunnyBody);

    b2FixtureDef bunnyFixtureDef;
    const auto spriteSize = m_sprite->getContentSize();
    auto boxShape = utils::box2d::getBoxShape(
        {spriteSize.width * relativeBodySize.x, spriteSize.height * relativeBodySize.y});
    bunnyFixtureDef.shape = &boxShape;
    bunnyFixtureDef.density = 1;
    bunnyFixtureDef.filter.categoryBits = CollisionGroup::bunny;
    bunnyFixtureDef.filter.maskBits = CollisionGroup::bee + CollisionGroup::ground;
    m_body->CreateFixture(&bunnyFixtureDef);
    m_body->SetSleepingAllowed(false);
    m_physicsObject.sprite = m_sprite;
    m_physicsObject.hiddenPos = {-300.0f, -300.0f};
    m_physicsObject.collisionOccurredCallback = [this](const std::uint8_t colliderGroup) -> bool {
        if (colliderGroup == CollisionGroup::ground) {
            land();
            return true;
        }
        if (colliderGroup == CollisionGroup::bee) {
            cocos2d::EventCustom event(CustomEvent::bunnyHitEvent);
            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
            // TODO play sound?
            return true;
        }
        return false;
    };
    dispose();
    return true;
}

void Bunny::activate(const cocos2d::Vec2& pos)
{
    // No nullcheck, BunnyController shall ensure that only initialized bunnies are accessed
    m_physicsObject.active = true;
    m_body->SetTransform(utils::box2d::pixelsToMeters(pos), 0.0f);
}

const cocos2d::Rect Bunny::getBoundingBox() const
{
    return m_sprite->getBoundingBox();
}

void Bunny::jump()
{
    if ((m_state == BunnyState::jumped || m_state == BunnyState::doublejump) &&
        m_body->GetLinearVelocity().y < -25.0f) {
        // Downwards dash
        m_state = BunnyState::dash;
        m_body->ApplyLinearImpulse(b2Vec2{0.0f, 30.0f}, m_body->GetWorldCenter(), true);

        m_sprite->setFlippedY(true);
        return;
    }

    if (m_state == BunnyState::grounded || m_state == BunnyState::jumped) {
        m_state = (m_state == BunnyState::grounded) ? BunnyState::jumped : BunnyState::doublejump;

        const auto spriteFrame{
            cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("./bunny_jump_96_48")};
        m_sprite->setSpriteFrame(spriteFrame);

        static constexpr float jumpForce = 15.0f;
        static constexpr float doubleJumpForce = 10.0f;
        auto forceY = jumpForce;
        if (m_state == BunnyState::doublejump) {
            forceY = doubleJumpForce;
            auto rotateBy = cocos2d::RotateBy::create(0.3f, 180.0f);
            auto rotateEaseIn = cocos2d::EaseInOut::create(rotateBy, 1.0f);
            m_sprite->runAction(rotateEaseIn);
        }
        m_body->SetLinearVelocity(b2Vec2{0.0f, forceY});
    }
}

void Bunny::land()
{
    m_sprite->stopAllActions();
    const auto spriteFrame{
        cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("./bunny_stand_96_48")};
    m_sprite->setSpriteFrame(spriteFrame);
    m_sprite->setFlippedY(false);
    m_sprite->setRotation(0);

    m_state = BunnyState::grounded;
}

void Bunny::dispose()
{
    m_state = BunnyState::doublejump;
    m_physicsObject.active = false;
}
