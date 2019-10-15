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
constexpr float beeMid{210.0f};
constexpr float beeHigh{290.0f};

BeeCycle getBassInit()
{
    BeeCycle cycle(Direction::left);
    bool ok = cycle.addBeeSpawn(beeLow, 0.0f);
    for (std::uint8_t i = 0; i < 5; ++i) {
        ok = cycle.addBeeSpawn(beeLow, BeeCycle::halfNote);
        if (!ok) {
            cocos2d::log("Failed to create getBassInit properly");
        }
    }
    return cycle;
}

BeeCycle getSnareInit()
{
    BeeCycle cycle(Direction::right);
    cycle.addBeeSpawn(beeLow, BeeCycle::quarterNote);
    for (std::uint8_t i = 0; i < 5; ++i) {
        bool ok = cycle.addBeeSpawn(beeLow, BeeCycle::halfNote);
        if (!ok) {
            cocos2d::log("Failed to create getSnareInit properly");
        }
    }
    return cycle;
}

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

BeeCycle getBassDoubles()
{
    BeeCycle cycle(Direction::left);
    // First half
    bool ok = cycle.addBeeSpawn(beeLow, 0.0f);
    ok = cycle.addBeeSpawn(beeLow, BeeCycle::halfNote);
    ok = cycle.addBeeSpawn(beeLow, BeeCycle::halfNote);
    ok = cycle.addBeeSpawn(beeHigh, BeeCycle::triplet);
    ok = cycle.addBeeSpawn(beeLow, BeeCycle::triplet);
    ok = cycle.addBeeSpawn(beeLow, BeeCycle::triplet + BeeCycle::halfNote);
    ok = cycle.addBeeSpawn(beeLow, BeeCycle::halfNote);

    // second
    ok = cycle.addBeeSpawn(beeLow, BeeCycle::halfNote);
    ok = cycle.addBeeSpawn(beeLow, BeeCycle::halfNote);
    ok = cycle.addBeeSpawn(beeMid, BeeCycle::eightNote);
    ok = cycle.addBreak(BeeCycle::eightNote);
    if (!ok) {
        cocos2d::log("Failed to create getBassDoubles properly");
    }
    return cycle;
}

BeeCycle get4to4DiveBass()
{
    BeeCycle cycle(Direction::left);
    bool ok = cycle.addBeeSpawn(beeLow, 0.0f);
    for (std::uint8_t i = 0; i < 7; ++i) {
        ok = cycle.addBeeSpawn(beeHigh, BeeCycle::triplet);
        ok = cycle.addBeeSpawn(beeLow, BeeCycle::triplet * 2);
        if (!ok) {
            cocos2d::log("Failed to create get4to4Bass properly");
        }
    }
    return cycle;
}

BeeCycle get4to2DiveBass()
{
    BeeCycle cycle(Direction::left);
    bool ok = cycle.addBeeSpawn(beeLow, 0.0f);
    for (std::uint8_t i = 0; i < 7; ++i) {
        if (i % 2 == 0) {
            ok = cycle.addBeeSpawn(beeLow, BeeCycle::halfNote);
        }
        else {
            ok = cycle.addBeeSpawn(beeHigh, BeeCycle::triplet);
            ok = cycle.addBeeSpawn(beeLow, BeeCycle::triplet * 2);
        }
        if (!ok) {
            cocos2d::log("Failed to create get4to4Bass properly");
        }
    }
    return cycle;
}

BeeCycle get2to4DiveBass()
{
    BeeCycle cycle(Direction::left);
    bool ok = cycle.addBeeSpawn(beeLow, 0.0f);
    for (std::uint8_t i = 0; i < 7; ++i) {
        if (i % 2 == 1) {
            ok = cycle.addBeeSpawn(beeLow, BeeCycle::halfNote);
        }
        else {
            ok = cycle.addBeeSpawn(beeHigh, BeeCycle::triplet);
            ok = cycle.addBeeSpawn(beeLow, BeeCycle::triplet * 2);
        }
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

BeeCycle getSnareDiveEnd()
{
    BeeCycle cycle(Direction::right);
    cycle.addBeeSpawn(beeLow, BeeCycle::quarterNote);
    for (std::uint8_t i = 0; i < 6; ++i) {
        bool ok = cycle.addBeeSpawn(beeLow, BeeCycle::halfNote);
        if (!ok) {
            cocos2d::log("Failed to create getSnareDiveEnd properly");
        }
    }
    if (!cycle.addBeeSpawn(beeMid, BeeCycle::eightNote) ||
        !cycle.addBeeSpawn(beeHigh, BeeCycle::quarterNote + BeeCycle::eightNote)) {
        cocos2d::log("Failed to create getSnareDiveEnd properly");
    }

    return cycle;
}

BeeCycle getSnareDoubleEnd()
{
    BeeCycle cycle(Direction::right);
    bool ok = cycle.addBeeSpawn(beeLow, BeeCycle::quarterNote);
    for (std::uint8_t i = 0; i < 6; ++i) {
        ok = cycle.addBeeSpawn(beeLow, BeeCycle::halfNote);
    }
    ok = cycle.addBeeSpawn(beeMid, BeeCycle::triplet);
    ok = cycle.addBeeSpawn(beeLow, BeeCycle::triplet);

    ok = cycle.addBreak(BeeCycle::triplet);
    if (!ok) {
        cocos2d::log("Failed to create getSnareDoubleEnd properly");
    }

    return cycle;
}

BeeCycle get4to2diveSnare()
{
    BeeCycle cycle(Direction::right);
    bool ok = cycle.addBeeSpawn(beeLow, BeeCycle::quarterNote);
    for (std::uint8_t i = 0; i < 7; ++i) {
        if (i % 2 == 0) {
            ok = cycle.addBeeSpawn(beeLow, BeeCycle::halfNote);
        }
        else {
            ok = cycle.addBeeSpawn(beeHigh, BeeCycle::triplet);
            ok = cycle.addBeeSpawn(beeLow, BeeCycle::triplet * 2);
        }
    }
    if (!ok) {
        cocos2d::log("Failed to create get4to4Bass properly");
    }
    return cycle;
}

BeeCycle get2to4diveSnare()
{
    BeeCycle cycle(Direction::right);
    bool ok = cycle.addBeeSpawn(beeLow, BeeCycle::quarterNote);
    for (std::uint8_t i = 0; i < 7; ++i) {
        if (i % 2 == 1) {
            ok = cycle.addBeeSpawn(beeLow, BeeCycle::halfNote);
        }
        else {
            ok = cycle.addBeeSpawn(beeHigh, BeeCycle::triplet);
            ok = cycle.addBeeSpawn(beeLow, BeeCycle::triplet * 2);
        }
    }
    if (!ok) {
        cocos2d::log("Failed to create get4to4Bass properly");
    }
    return cycle;
}

}  // namespace

BeeCycles::BeeCycles()
{
    m_snareCycles.emplace_back(getSnareInit());
    m_snareCycles.emplace_back(get4to4Snare());
    m_snareCycles.emplace_back(getSnareDiveEnd());
    m_snareCycles.emplace_back(getSnareDoubleEnd());
    m_snareCycles.emplace_back(get4to2diveSnare());
    m_snareCycles.emplace_back(get2to4diveSnare());
    m_bassCycles.emplace_back(getBassInit());
    m_bassCycles.emplace_back(get4to4Bass());
    m_bassCycles.emplace_back(getBassDoubles());
    m_bassCycles.emplace_back(get4to4DiveBass());
    m_bassCycles.emplace_back(get4to2DiveBass());
    m_bassCycles.emplace_back(get2to4DiveBass());
}

const BeeCycle& BeeCycles::getSnare(std::uint32_t index) const
{
    return m_snareCycles[index];
}

const BeeCycle& BeeCycles::getBass(std::uint32_t index) const
{
    return m_bassCycles[index];
}

BeeCycles::BeeCyclePair BeeCycles::getRandomCycles() const
{
    return BeeCyclePair{cocos2d::RandomHelper::random_int(0U, m_snareCycles.size() - 2) + 1,
                        cocos2d::RandomHelper::random_int(0U, m_bassCycles.size() - 2) + 1};
}

BeeCycles::BeeCyclePair BeeCycles::getInitialCycles() const
{
    return BeeCyclePair{0, 0};
}

BeeCycles::BeeCyclePair BeeCycles::getDevCycles() const
{
    return BeeCyclePair{5, 5};
}
