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
constexpr float beeMid{220.0f};
constexpr float beeHigh{270.0f};

BeeCycle get4to4Bass()
{
    BeeCycle cycle(Direction::left);
    bool ok = cycle.addBeeSpawn(beeLow, 0.0f);
    for (std::uint8_t i = 0; i < 7; ++i) {
        ok = cycle.addBeeSpawn(beeLow, BeeCycle::halfNote);
        if (!ok) {
            cocos2d::log("Failed to create get4to4Bass properly");
        }
    }
    return cycle;
}
BeeCycle get4to4Snare()
{
    BeeCycle cycle(Direction::right);
    cycle.addBeeSpawn(beeLow, BeeCycle::quarterNote);
    for (std::uint8_t i = 0; i < 7; ++i) {
        bool ok = cycle.addBeeSpawn(beeLow, BeeCycle::halfNote);
        if (!ok) {
            cocos2d::log("Failed to create get4to4Bass properly");
        }
    }
    return cycle;
}
}  // namespace

BeeCycles::BeeCycles()
{
    m_snareCycles.emplace_back(get4to4Snare());
    m_bassCycles.emplace_back(get4to4Bass());
}

BeeCycles::BeeCyclePair BeeCycles::getRandomCycles()
{
    return BeeCyclePair{
        m_snareCycles[cocos2d::RandomHelper::random_int(0U, m_snareCycles.size() - 1)],
        m_bassCycles[cocos2d::RandomHelper::random_int(0U, m_bassCycles.size() - 1)]};
}
