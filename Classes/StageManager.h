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

#ifndef __STAGE_MANAGER_H__
#define __STAGE_MANAGER_H__

#include <vector>
#include "BeeSpawn.h"

struct StageInfo {
    BeeSpawns beeSpawns;
    std::uint8_t bunnyCount;
};

class StageManager {
public:
    StageInfo* getNext() const;

private:
    // Hard coded stages for demonstration purposes.
    // For efficient testing this should be file read -> no recompile on change.
    std::vector<StageInfo> m_stages{
        {{{0.0f, 145.0f, direction::left}}, 1},
        {{{0.0f, 145.0f, direction::left}, {0.5f, 240.0f, direction::right}}, 1},
        {{{0.0f, 145.0f, direction::left}, {1.5f, 145.0f, direction::left}}, 2},
        {{{0.0f, 145.0f, direction::left},
          {1.5f, 145.0f, direction::left},
          {1.0f, 240.0f, direction::right}},
         2}};
    mutable std::vector<StageInfo>::iterator m_curStage{m_stages.begin()};
};

#endif  // __STAGE_MANAGER_H__
