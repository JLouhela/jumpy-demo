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

#ifndef __BEE_CYCLES_H__
#define __BEE_CYCLES_H__

#include <vector>
#include "BeeCycle.h"
#include "Direction.h"

class BeeCycles {
public:
    struct BeeCyclePair {
        BeeCyclePair(const BeeCycle& asnareCycle, const BeeCycle& abassCycle)
            : snareCycle{asnareCycle}, bassCycle{abassCycle}
        {
        }
        const BeeCycle& snareCycle;
        const BeeCycle& bassCycle;
    };

    BeeCycles();
    BeeCyclePair getRandomCycles();

private:
    std::vector<BeeCycle> m_snareCycles;
    std::vector<BeeCycle> m_bassCycles;
};

#endif  // __BEE_CYCLES_H__
