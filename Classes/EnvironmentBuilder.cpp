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

#include "EnvironmentBuilder.h"
#include "Box2D/Box2D.h"
#include "Box2DUtils.h"
#include "CollisionGroup.h"
#include "ZOrders.h"
#include "cocos2d.h"

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

namespace environment {

bool buildEnvironment(cocos2d::Scene& scene, b2World& world)
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
    scene.addChild(bgSprite, ZOrder::background);

    // add ground sprite to the bottom of the screen
    static const float groundHeight = 128.0f;
    auto groundSprite{
        initSprite("./ground_1280_128",
                   cocos2d::Vec2{visibleSize.width / 2 + origin.x, groundHeight / 2 + origin.y})};
    if (!groundSprite) {
        return false;
    }
    static const float treeHeight = 720.0f;
    auto treeSprite{
        initSprite("./tree_256_720", cocos2d::Vec2{visibleSize.width / 2 + origin.x,
                                                   treeHeight / 2 + origin.y + groundHeight - 1})};
    if (!treeSprite) {
        return false;
    }
    // Ground is solid
    b2BodyDef groundBody{};
    groundBody.type = b2_staticBody;
    groundBody.angle = 0;
    groundBody.position = utils::box2d::pixelsToMeters(
        {groundSprite->getBoundingBox().getMidX(), groundSprite->getBoundingBox().getMidY()});

    auto groundStaticBody = world.CreateBody(&groundBody);

    b2FixtureDef groundFixtureDef;
    auto groundShape = utils::box2d::getBoxShape(groundSprite->getContentSize());
    groundFixtureDef.shape = &groundShape;
    groundFixtureDef.density = 1;
    groundFixtureDef.filter.categoryBits = CollisionGroup::ground;
    groundFixtureDef.filter.maskBits = CollisionGroup::bunny;
    groundStaticBody->CreateFixture(&groundFixtureDef);
    scene.addChild(groundSprite, ZOrder::ground);

    // Tree is a sensor
    b2BodyDef treeBody{};
    treeBody.type = b2_staticBody;
    treeBody.angle = 0;
    treeBody.position = utils::box2d::pixelsToMeters(
        {treeSprite->getBoundingBox().getMidX(), treeSprite->getBoundingBox().getMidY()});

    auto treeStaticBody = world.CreateBody(&treeBody);

    b2FixtureDef treeFixtureDef;
    auto treeBoxSize = treeSprite->getContentSize();
    treeBoxSize.width *= 0.4f;
    auto treeShape = utils::box2d::getBoxShape(treeBoxSize);
    treeFixtureDef.shape = &treeShape;
    treeFixtureDef.density = 1;
    treeFixtureDef.isSensor = true;
    treeFixtureDef.filter.categoryBits = CollisionGroup::tree;
    treeFixtureDef.filter.maskBits = CollisionGroup::bee;
    treeStaticBody->CreateFixture(&treeFixtureDef);
    scene.addChild(treeSprite, ZOrder::background);
    return true;
}
}  // namespace environment
