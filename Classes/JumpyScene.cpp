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

#include "JumpyScene.h"
#include "Box2D/Box2D.h"
#include "Box2DUtils.h"
#include "CollisionGroup.h"
#include "SimpleAudioEngine.h"
#include "cocos2d/extensions/cocos-ext.h"

#include "ZOrders.h"
namespace {
cocos2d::Sprite* initSprite(const std::string& name, const cocos2d::Vec2& pos)
{
    const auto spriteFrame{cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(name)};
    auto sprite{cocos2d::Sprite::createWithSpriteFrame(spriteFrame)};
    if (sprite == nullptr) {
        cocos2d::log("Error while loading: %s\n", name.c_str());
    }
    else {
        sprite->setPosition(pos);
    }
    return sprite;
}
cocos2d::extension::PhysicsSprite* initPhysicsSprite(const std::string& name)
{
    const auto spriteFrame{cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(name)};

    auto sprite{cocos2d::extension::PhysicsSprite::createWithSpriteFrame(spriteFrame)};
    if (sprite == nullptr) {
        cocos2d::log("Error while loading: %s\n", name.c_str());
    }
    return sprite;
}

double getCurrentTick()
{
    static struct timeval currentTime;
    cocos2d::gettimeofday(&currentTime, nullptr);
    return (currentTime.tv_sec) + (currentTime.tv_usec / 1000000.0);
}

}  // namespace

cocos2d::Scene* JumpyScene::createScene()
{
    return JumpyScene::create();
}

// on "init" you need to initialize your instance
bool JumpyScene::init()
{
    // super init always first
    if (!cocos2d::Scene::init()) {
        cocos2d::log("Could not initialize parent scene");
        return false;
    }

    if (!initEnvironment()) {
        cocos2d::log("Could not initialize gfx for scene");
        return false;
    }

    if (!m_gameLogic.init(*this, m_world.getWorld())) {
        cocos2d::log("Could not initialize game logic");
        return false;
    }

    m_world.setDrawDebug(true);

    return true;
}

bool JumpyScene::initEnvironment()
{
    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    const cocos2d::Vec2 origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

    // Add bg sprite to the center of the screen
    // Could (should?) be node instead of sprite, but spriteframe doesn't seem to support it.
    auto bgSprite{initSprite("./bg_1280_720", cocos2d::Vec2(visibleSize.width / 2 + origin.x,
                                                            visibleSize.height / 2 + origin.y))};
    if (!bgSprite) {
        return false;
    }

    // add the sprite as a child to this layer
    this->addChild(bgSprite, ZOrder::background);

    // add ground sprite to the bottom of the screen
    static const float groundHeight = 128.0f;
    auto groundSprite{
        initSprite("./ground_1280_128",
                   cocos2d::Vec2{visibleSize.width / 2 + origin.x, groundHeight / 2 + origin.y})};
    if (!groundSprite) {
        return false;
    }

    // Ground is solid
    b2BodyDef groundBody{};
    groundBody.type = b2_staticBody;
    groundBody.angle = 0;
    groundBody.position = utils::box2d::pixelsToMeters(
        {groundSprite->getBoundingBox().getMidX(), groundSprite->getBoundingBox().getMidY()});

    auto staticBody = m_world.getWorld().CreateBody(&groundBody);

    b2FixtureDef groundFixtureDef;
    auto boxShape = utils::box2d::getBoxShape(groundSprite->getContentSize());
    groundFixtureDef.shape = &boxShape;
    groundFixtureDef.density = 1;
    groundFixtureDef.filter.categoryBits = CollisionGroup::ground;
    groundFixtureDef.filter.maskBits = CollisionGroup::bunny;
    staticBody->CreateFixture(&groundFixtureDef);
    this->addChild(groundSprite, ZOrder::ground);

    // Add (invisible) border colliders for bees
    // Left
    b2BodyDef leftBeeColliderBody{};
    leftBeeColliderBody.type = b2_staticBody;
    leftBeeColliderBody.angle = 0;
    leftBeeColliderBody.position = utils::box2d::pixelsToMeters({1, visibleSize.height / 2});
    auto staticLeftBeeColliderBody = m_world.getWorld().CreateBody(&leftBeeColliderBody);

    b2FixtureDef leftBeeColliderFixtureDef;
    auto leftBeeCollidershape = utils::box2d::getBoxShape({2, visibleSize.height});
    leftBeeColliderFixtureDef.shape = &leftBeeCollidershape;
    leftBeeColliderFixtureDef.density = 1;
    leftBeeColliderFixtureDef.filter.categoryBits = CollisionGroup::border;
    leftBeeColliderFixtureDef.filter.maskBits = CollisionGroup::bee;
    leftBeeColliderFixtureDef.isSensor = true;  // TODO verify
    staticLeftBeeColliderBody->CreateFixture(&leftBeeColliderFixtureDef);

    // Right
    b2BodyDef rightBeeColliderBody{};
    rightBeeColliderBody.type = b2_staticBody;
    rightBeeColliderBody.angle = 0;
    rightBeeColliderBody.position =
        utils::box2d::pixelsToMeters({visibleSize.width - 1, visibleSize.height / 2});
    auto staticRightBeeColliderBody = m_world.getWorld().CreateBody(&rightBeeColliderBody);

    b2FixtureDef rightBeeColliderFixtureDef;
    auto rightBeeCollidershape = utils::box2d::getBoxShape({2, visibleSize.height});
    rightBeeColliderFixtureDef.shape = &rightBeeCollidershape;
    rightBeeColliderFixtureDef.density = 1;
    rightBeeColliderFixtureDef.filter.categoryBits = CollisionGroup::border;
    rightBeeColliderFixtureDef.filter.maskBits = CollisionGroup::bee;
    rightBeeColliderFixtureDef.isSensor = true;  // TODO verify
    staticRightBeeColliderBody->CreateFixture(&rightBeeColliderFixtureDef);

    // World stepping
    scheduleUpdate();

    return true;
}

void JumpyScene::update(const float dt)
{
    cocos2d::Scene::update(dt);
    m_world.update(dt);
}

void JumpyScene::render(cocos2d::Renderer* renderer,
                        const cocos2d::Mat4& eyeTransform,
                        const cocos2d::Mat4* eyeProjection)
{
    cocos2d::Scene::render(renderer, eyeTransform, eyeProjection);
    m_world.drawDebug(eyeTransform);
}
