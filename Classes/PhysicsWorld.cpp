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

#include "PhysicsWorld.h"
#include "Box2DUtils.h"
#include "PhysicsObject.h"
#include "cocos2d.h"

PhysicsWorld::PhysicsWorld()
{
    static const b2Vec2 gravity{0, -30.0f};
    m_world = new b2World(gravity);

    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    flags += b2Draw::e_aabbBit;
    flags += b2Draw::e_centerOfMassBit;
    m_debugDraw.SetFlags(flags);
    m_world->SetDebugDraw(&m_debugDraw);
}

PhysicsWorld::~PhysicsWorld()
{
    delete m_world;
}

void PhysicsWorld::update(double dt)
{
    m_tickAccumulator += dt;

    static constexpr double secondsPerUpdate{1.0f / 60.0f};
    bool updatePerformed{false};
    // update the world with the same seconds per update
    while (m_tickAccumulator >= secondsPerUpdate) {
        m_tickAccumulator -= secondsPerUpdate;

        // perform a single step of the physics simulation
        m_world->Step(static_cast<float>(secondsPerUpdate), 8, 3);
        updatePerformed = true;
    }
    m_world->ClearForces();
    if (updatePerformed) {
        syncBodies(m_tickAccumulator / secondsPerUpdate);
    }
}

void PhysicsWorld::syncBodies(float alpha)
{
    for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext()) {
        if (b->GetType() == b2BodyType::b2_staticBody) {
            continue;
        }
        if (!b->GetUserData()) {
            continue;
        }
        auto physicsObj = static_cast<PhysicsObject*>(b->GetUserData());

        // Dirty activation / deactivation:
        if (physicsObj->active && !b->IsActive()) {
            b->SetActive(true);
            physicsObj->sprite->setVisible(true);
        }
        else if (!physicsObj->active && b->IsActive()) {
            b->SetActive(false);
            physicsObj->sprite->setVisible(false);
            b->SetTransform(physicsObj->hiddenPos, 0.0f);
        }
        // sprite sync: position only, rotation not considered in this game
        utils::box2d::syncPhysicsToSprite(*physicsObj, b->GetPosition(), alpha);
        physicsObj->prevPos = b->GetPosition();
    }
}

void PhysicsWorld::drawDebug(const cocos2d::Mat4& transform)
{
    if (!m_drawDebug) {
        return;
    }
    auto director = cocos2d::Director::getInstance();
    director->pushMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
    cocos2d::GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
    m_world->DrawDebugData();
    director->popMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}
