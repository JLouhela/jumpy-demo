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
#include "CollisionGroup.h"
#include "SimpleAudioEngine.h"
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
}  // namespace

cocos2d::Scene* JumpyScene::createScene()
{
    return JumpyScene::create();
}

// on "init" you need to initialize your instance
bool JumpyScene::init()
{
    // super init always first
    if (!cocos2d::Scene::initWithPhysics()) {
        cocos2d::log("Could not initialize parent scene");
        return false;
    }
    getPhysicsWorld()->setGravity(cocos2d::Vect(0, -500));

    if (!initEnvironment()) {
        cocos2d::log("Could not initialize gfx for scene");
        return false;
    }

    if (!m_gameLogic.init(this)) {
        cocos2d::log("Could not initialize game logic");
        return false;
    }

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
    getPhysicsWorld()->setDebugDrawMask(0xFFFFFFFF);

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
    auto groundPhysicsBody = cocos2d::PhysicsBody::createBox(
        cocos2d::Size(visibleSize.width, 128), cocos2d::PhysicsMaterial(1.0f, 1.0f, 0.0f));
    groundPhysicsBody->setDynamic(false);
    groundPhysicsBody->setCategoryBitmask(CollisionGroup::ground);
    groundPhysicsBody->setCollisionBitmask(CollisionGroup::bunny);
    groundPhysicsBody->setContactTestBitmask(CollisionGroup::bunny);

    groundSprite->addComponent(groundPhysicsBody);
    this->addChild(groundSprite, ZOrder::ground);

    // Add (invisible) border colliders for bees
    // Left
    auto leftBorderNode{cocos2d::Node::create()};
    leftBorderNode->setPosition({0.0f, 0.0f});
    leftBorderNode->setContentSize({1, visibleSize.height});

    auto leftBorderPhysicsBody = cocos2d::PhysicsBody::createBox(
        cocos2d::Size(1, visibleSize.height), cocos2d::PhysicsMaterial(1.0f, 1.0f, 0.0f));
    leftBorderPhysicsBody->setCategoryBitmask(CollisionGroup::border);
    leftBorderPhysicsBody->setContactTestBitmask(CollisionGroup::bee);
    leftBorderPhysicsBody->setCollisionBitmask(0x00);
    leftBorderPhysicsBody->setEnabled(true);
    leftBorderPhysicsBody->setDynamic(false);
    leftBorderNode->addComponent(leftBorderPhysicsBody);
    this->addChild(leftBorderNode);

    // Right
    auto rightBorderNode{cocos2d::Node::create()};
    rightBorderNode->setPosition({visibleSize.width - 1.0f, 0.0f});

    rightBorderNode->setContentSize({1, visibleSize.height});
    auto rightBorderPhysicsBody = cocos2d::PhysicsBody::createBox(
        {1, visibleSize.height}, cocos2d::PhysicsMaterial(1.0f, 1.0f, 0.0f));
    rightBorderPhysicsBody->setCategoryBitmask(CollisionGroup::border);
    rightBorderPhysicsBody->setContactTestBitmask(CollisionGroup::bee);
    rightBorderPhysicsBody->setCollisionBitmask(0x00);
    rightBorderPhysicsBody->setDynamic(false);

    rightBorderNode->addComponent(rightBorderPhysicsBody);
    this->addChild(rightBorderNode, 10);

    return true;
}
