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

#include "ContactListener.h"
#include <cstdint>
#include "Bee.h"
#include "Bunny.h"
#include "CollisionGroup.h"
#include "CustomEvents.h"

bool bunnyToBeeCollision(const std::int32_t a, const std::int32_t b)
{
    return (a == CollisionGroup::bee || a == CollisionGroup::bunny) &&
           (b == CollisionGroup::bee || b == CollisionGroup::bunny) && (a != b);
}

bool bunnyToGroundCollision(const std::int32_t a, const std::int32_t b)
{
    return (a == CollisionGroup::ground || a == CollisionGroup::bunny) &&
           (b == CollisionGroup::ground || b == CollisionGroup::bunny) && (a != b);
}

bool beeToBorderCollision(const std::int32_t a, const std::int32_t b)
{
    return (a == CollisionGroup::bee || a == CollisionGroup::border) &&
           (b == CollisionGroup::bee || b == CollisionGroup::border) && (a != b);
}

void fireBunnyHitEvent(cocos2d::EventDispatcher& eventDispatcher)
{
    cocos2d::EventCustom event(CustomEvent::bunnyHitEvent);
    eventDispatcher.dispatchEvent(&event);
}
void fireBeeThroughEvent(cocos2d::EventDispatcher& eventDispatcher)
{
    cocos2d::EventCustom event(CustomEvent::beeThroughEvent);
    eventDispatcher.dispatchEvent(&event);
}

bool ContactListener::init(cocos2d::Scene& scene)
{
    m_eventDispatcher = scene.getEventDispatcher();
    if (!m_eventDispatcher) {
        cocos2d::log("Could not get event dispatcher");
        return false;
    }
    // TODO inherit from b2ContactListener
    //   m_world->SetContactListener(&myContactListenerInstance);
    // etc, see https://www.iforce2d.net/b2dtut/collision-callbacks

    /*
m_contactListener = cocos2d::EventListenerPhysicsContact::create();
if (!m_contactListener) {
    cocos2d::log("Could not create contact listener");
    return false;
}
m_contactListener->setEnabled(true);
m_contactListener->onContactBegin = [this](cocos2d::PhysicsContact& contact) -> bool {
    const auto maskA = contact.getShapeA()->getBody()->getCategoryBitmask();
    const auto maskB = contact.getShapeB()->getBody()->getCategoryBitmask();
    if (bunnyToBeeCollision(maskA, maskB)) {
        fireBunnyHitEvent(*m_eventDispatcher);
        return true;
    }
    // Not sure how I feel about directly invoking Bunny methods here.
    // api getting bloated, although usage is in logical place.
    // Ideally Bunny would be responsible for it's own collisions, but cocos2d seems to
    // prefer single collision detector which for sure is more efficient on this design.
    if (bunnyToGroundCollision(maskA, maskB)) {
        if (maskA == CollisionGroup::bunny) {
            static_cast<Bunny*>(contact.getShapeA()->getBody()->getNode()->getUserData())
                ->resetState();
        }
        if (maskB == CollisionGroup::bunny) {
            static_cast<Bunny*>(contact.getShapeB()->getBody()->getNode()->getUserData())
                ->resetState();
        }
        return true;
    }

            
    return false;
};

m_contactListener->onContactSeparate = [this](cocos2d::PhysicsContact& contact) -> bool {
    const auto maskA = contact.getShapeA()->getBody()->getCategoryBitmask();
    const auto maskB = contact.getShapeB()->getBody()->getCategoryBitmask();

    if (beeToBorderCollision(maskA, maskB)) {
        // Could also dispose the bee, probably no point.
        fireBeeThroughEvent(*m_eventDispatcher);
        return true;
    }
    return false;
};

scene.getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_contactListener, &scene);
*/
    return true;
}
