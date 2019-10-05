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

#ifndef __GAME_LOGIC_H__
#define __GAME_LOGIC_H__

#include <cstdint>
#include "BeeEventListener.h"
#include "BeeSpawner.h"
#include "BunnyController.h"
#include "ContactListener.h"
#include "InputHandler.h"
#include "RetryOverlay.h"
#include "StageManager.h"
#include "cocos2d.h"

class BunnyController;
class BeeSpawner;
class InputHandler;
class b2World;

enum GameState : std::uint8_t { start, active, wait, end };

class GameLogic {
public:
    bool init(cocos2d::Scene& scene, b2World& world);

private:
    void initStage(const StageInfo& stageInfo);
    void endStage();
    void cleanStage();

    cocos2d::Scene* m_scene;
    BunnyController m_bunnyController;
    BeeSpawner m_beeSpawner;
    StageManager m_stageManager;
    RetryOverlay m_retryOverlay;
    cocos2d::Node* m_actionNode{nullptr};
    BeeEventListener m_beeEventListener;
    ContactListener m_contactListener;
    cocos2d::EventListenerCustom* m_bunnyEventListener{nullptr};
    GameState m_state{GameState::start};
    std::int32_t m_curLvl{0};
    std::uint8_t m_bunnyCount{0};
    InputHandler m_inputHandler;
};

#endif  // __GAME_LOGIC_H__
