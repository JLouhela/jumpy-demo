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

#ifndef __BEE_CYCLE_H__
#define __BEE_CYCLE_H__

#include <cstdint>
#include <utility>
#include <vector>
#include "Direction.h"

struct BeeSpawn {
    BeeSpawn(float ay, float atime) : y{ay}, timeFromCycleStart{atime}
    {
    }
    float y{0.0f};
    float timeFromCycleStart{0.0f};
};

class BeeCycle {
public:
    // TODO check
    static constexpr float bpm{110.0f};
    static constexpr float quarterNote{545.45f};
    static constexpr float halfNote{quarterNote * 2.0f};
    static constexpr float beatLength{quarterNote * 4.0f};
    static constexpr float eightNote{quarterNote / 2.0f};
    static constexpr float sixteenthNote{eightNote / 2.0f};
    static constexpr float triplet{quarterNote * 0.6667f};
    static constexpr float cycleLength{beatLength * 4.0f};

    explicit BeeCycle(Direction dir) : m_dir{dir}
    {
    }

    Direction getDirection() const
    {
        return m_dir;
    }

    const std::vector<BeeSpawn>& getCycle() const
    {
        return m_cycle;
    }

    bool addBeeSpawn(float y, float deltaTime);
    bool addBreak(float breakTime);

private:
    Direction m_dir{Direction::left};
    float m_usedTime{0.0f};
    std::vector<BeeSpawn> m_cycle;
};

#endif  // _BEE_CYCLE_H__
