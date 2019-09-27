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
#include "SimpleAudioEngine.h"
#include "ZOrders.h"

namespace {
cocos2d::Sprite* initSprite(const std::string& name, const cocos2d::Vec2& pos)
{
    const auto spriteFrame{cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(name)};
    auto sprite{cocos2d::Sprite::createWithSpriteFrame(spriteFrame)};
    if (sprite == nullptr) {
        cocos2d::log("Error while loading: %s\n", name);
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
        return false;
    }

    if (!initGfx()) {
        return false;
    }
    if (!initEntities()) {
        return false;
    }
    m_inputHandler.init(m_bunnyController, getEventDispatcher());
    return true;
}

bool JumpyScene::initGfx()
{
    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    const cocos2d::Vec2 origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

    // add bg sprite to the center of the screen
    auto bgSprite{initSprite("./bg_1280_720", cocos2d::Vec2(visibleSize.width / 2 + origin.x,
                                                            visibleSize.height / 2 + origin.y))};
    if (!bgSprite) {
        return false;
    }

    // add the sprite as a child to this layer
    this->addChild(bgSprite, ZOrder::background);

    // add ground sprite to the bottom of the screen
    static constexpr std::int16_t groundHeight = 128;
    auto groundSprite{
        initSprite("./ground_1280_128",
                   cocos2d::Vec2(visibleSize.width / 2 + origin.x, (groundHeight / 2) + origin.y))};
    if (!groundSprite) {
        return false;
    }

    // Ground is solid
    auto groundPhysicsBody = cocos2d::PhysicsBody::createBox(
        cocos2d::Size(visibleSize.width, 128), cocos2d::PhysicsMaterial(1.0f, 1.0f, 0.0f));
    groundPhysicsBody->setDynamic(false);

    groundSprite->addComponent(groundPhysicsBody);
    this->addChild(groundSprite, ZOrder::ground);

    return true;
}

bool JumpyScene::initEntities()
{
    if (!m_bunnyController.init(*this)) {
        cocos2d::log("Bunny manager not initialized properly");
        return false;
    }

    if (!m_beeSpawner.init(*this)) {
        cocos2d::log("Bee spawner not initialized properly");
        return false;
    }

    const auto visibleSize{cocos2d::Director::getInstance()->getVisibleSize()};
    const cocos2d::Vec2 origin{cocos2d::Director::getInstance()->getVisibleOrigin()};

    // Start with a single bunny
    if (!m_bunnyController.spawnBunny(
            cocos2d::Vec2(visibleSize.width / 4 + origin.x, visibleSize.height / 4 + origin.y))) {
        cocos2d::log("Could not spawn first bunny");
        return false;
    }

    // test
    m_beeSpawner.spawnBees({{0.0f, 200.0f, direction::left}, {1.0f, 300.0f, direction::right}});

    return true;
}
