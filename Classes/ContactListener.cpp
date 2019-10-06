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

void ContactListener::BeginContact(b2Contact *contact)
{
    const auto maskA = contact->GetFixtureA()->GetFilterData().categoryBits;
    const auto maskB = contact->GetFixtureB()->GetFilterData().categoryBits;

    auto physA = static_cast<PhysicsObject *>(contact->GetFixtureA()->GetBody()->GetUserData());
    if (physA && physA->collisionOccurredCallback) {
        physA->collisionOccurredCallback(maskB);
    }
    auto physB = static_cast<PhysicsObject *>(contact->GetFixtureB()->GetBody()->GetUserData());
    if (physB && physB->collisionOccurredCallback) {
        physB->collisionOccurredCallback(maskA);
    }
}

void ContactListener::EndContact(b2Contact *contact)
{
    const auto maskA = contact->GetFixtureA()->GetFilterData().categoryBits;
    const auto maskB = contact->GetFixtureB()->GetFilterData().categoryBits;

    auto physA = static_cast<PhysicsObject *>(contact->GetFixtureA()->GetBody()->GetUserData());
    if (physA && physA->collisionResolvedCallback) {
        physA->collisionResolvedCallback(maskB);
    }
    auto physB = static_cast<PhysicsObject *>(contact->GetFixtureB()->GetBody()->GetUserData());
    if (physB && physB->collisionResolvedCallback) {
        physB->collisionResolvedCallback(maskA);
    }
}
void ContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
}
void ContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
{
}
