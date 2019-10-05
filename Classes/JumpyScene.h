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

#ifndef __JUMPY_SCENE_H__
#define __JUMPY_SCENE_H__

#include "GLES-Render.h"
#include "GameLogic.h"
#include "PTM.h"
#include "cocos2d.h"

class b2World;

class JumpyScene : public cocos2d::Scene {
public:
    class ScopedWorld {
    public:
        ScopedWorld();
        ~ScopedWorld();

        b2World& getWorld()
        {
            return *m_world;
        }

        const b2World& getWorld() const
        {
            return *m_world;
        }

        ScopedWorld(const ScopedWorld&) = delete;

    private:
        b2World* m_world;
    };
    static cocos2d::Scene* createScene();

    virtual bool init() final;
    void update(float dt) final;
    void render(cocos2d::Renderer* renderer,
                const cocos2d::Mat4& eyeTransform,
                const cocos2d::Mat4* eyeProjection = nullptr) final;

private:
    bool initEnvironment();

    // From Box2D demo
    GLESDebugDraw m_debugDraw{PTM::ptm};
    ScopedWorld m_world;
    GameLogic m_gameLogic;
    double m_lastUpdateTick{-1.0};
    double m_tickAccumulator{0.0};

    // implement the "static create()" method manually
    CREATE_FUNC(JumpyScene);
};

#endif  // __JUMPYS_SCENE_H__
