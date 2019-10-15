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

#ifndef __BEE_SPAWNER_H__
#define __BEE_SPAWNER_H__

#include <array>
#include <chrono>
#include <cstdint>
#include "Bee.h"
#include "BeeCycles.h"
#include "cocos2d.h"

class b2World;

class BeeSpawner {
public:
    using BeeContainer = std::array<Bee, 20>;
    bool init(cocos2d::Scene& scene, b2World& world);
    bool spawnBees();
    void stop();

private:
    void spawnBee(float y, Direction dir);
    void scheduleSpawn(const std::vector<BeeSpawn>& spawns, Direction dir);
    // Bee_id equals to container index
    BeeContainer m_beeContainer{};
    BeeCycles m_cycles;
    bool m_cycleInitialized{false};

    cocos2d::Node* m_actionNode{nullptr};
};

#endif  // __BEE_SPAWNER_H__
