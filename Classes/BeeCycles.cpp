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

#include "BeeCycles.h"
#include "cocos2d.h"

namespace {

constexpr float beeLow{170.0f};

BeeCycle getSimpleCycle_1_left()
{
    BeeCycle cycle(Direction::left);
    bool ok{true};
    while (ok) {
        ok = cycle.addBeeSpawn(beeLow, BeeCycle::beatLength);
    }
    return cycle;
}
BeeCycle getSimpleCycle_1_right()
{
    BeeCycle cycle(Direction::right);
    bool ok{true};
    while (ok) {
        ok = cycle.addBeeSpawn(beeLow, BeeCycle::beatLength);
    }
    return cycle;
}

BeeCycle getSimpleCycle_2_left()
{
    BeeCycle cycle(Direction ::left);
    auto ok = cycle.addBreak(BeeCycle::eightNote);
    while (ok) {
        ok = cycle.addBeeSpawn(beeLow, BeeCycle::beatLength);
    }
    return cycle;
}

BeeCycle getSimpleCycle_2_right()
{
    BeeCycle cycle(Direction::right);
    auto ok = cycle.addBreak(BeeCycle::eightNote);
    while (ok) {
        ok = !cycle.addBeeSpawn(beeLow, BeeCycle::beatLength);
    }
    return cycle;
}
}  // namespace

BeeCycles::BeeCycles()
{
    m_leftCycles.emplace_back(getSimpleCycle_1_left());
    m_leftCycles.emplace_back(getSimpleCycle_2_left());
    m_rightCycles.emplace_back(getSimpleCycle_1_right());
    m_rightCycles.emplace_back(getSimpleCycle_2_right());
}

const BeeCycle& BeeCycles::getRandomCycle(Direction dir)
{
    if (dir == Direction::left) {
        return m_leftCycles[cocos2d::RandomHelper::random_int(0U, m_leftCycles.size() - 1)];
    }
    return m_rightCycles[cocos2d::RandomHelper::random_int(0U, m_rightCycles.size() - 1)];
}
