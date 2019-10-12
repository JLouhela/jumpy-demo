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

#ifndef __TUTORIAL_SCENE_H__
#define __TUTORIAL_SCENE_H__

#include "BunnyController.h"
#include "ContactListener.h"
#include "PhysicsWorld.h"
#include "TutorialOverlay.h"
#include "cocos2d.h"

#include "InputHandler.h"
#include "TutorialInputHandler.h"

class b2World;

class TutorialScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    ~TutorialScene();

    virtual bool init() final;
    void update(float dt) final;

private:
    void initTutorialLogic();
    void jumpLeftTutorial();
    void jumpRightTutorial();
    void doubleJumpTutorial();
    void doubleJumpPreparation();
    void doubleJumpClosure();
    void dashPreparation();
    void dashTutorial();
    void tutorialClosure();

    void clearInteractions();
    void delayedText(float delay, const std::string& text);

    PhysicsWorld m_world;
    InputHandler m_inputHandler;
    ContactListener m_contactListener;
    TutorialInputHandler m_tutorialInputHandler;
    TutorialOverlay m_tutorialOverlay;
    BunnyController m_bunnyController;
    cocos2d::EventListenerKeyboard* m_keyListener{nullptr};

    // implement the "static create()" method manually
    CREATE_FUNC(TutorialScene);
};

#endif  // __TUTORIAL_SCENE_H__
