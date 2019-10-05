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

#ifndef __PHYSICS_WORLD_H__
#define __PHYSICS_WORLD_H__

#include "GLES-Render.h"
#include "PTM.h"

class PhysicsWorld {
public:
    PhysicsWorld();
    ~PhysicsWorld();
    PhysicsWorld(const PhysicsWorld&) = delete;

    b2World& getWorld()
    {
        return *m_world;
    }

    const b2World& getWorld() const
    {
        return *m_world;
    }

    void setDrawDebug(bool value)
    {
        m_drawDebug = value;
    }

    void drawDebug(const cocos2d::Mat4& transform);

    void update(double dt);

private:
    void updateSprites(float alpha);

    b2World* m_world;
    GLESDebugDraw m_debugDraw{PTM::ptm};
    double m_lastUpdateTick{-1.0};
    double m_tickAccumulator{0.0};
    bool m_drawDebug{false};
};

#endif  // __PHYSICS_WORLD_H__
