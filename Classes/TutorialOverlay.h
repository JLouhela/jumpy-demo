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

#ifndef __TUTORIAL_OVERLAY_H__
#define __TUTORIAL_OVERLAY_H__

#include <cstdint>
#include <functional>
#include "cocos2d.h"

class TutorialOverlay {
public:
    enum class FingerDirection : std::uint8_t { up, upup, updown };

    bool init(cocos2d::Scene& scene);
    void showText(const cocos2d::Vec2& pos, const std::string& text);
    void showText(const std::string& text);
    void showSecondaryText(const std::string& text);
    void showLeftArea();
    void showRightArea();
    void showFinger(const cocos2d::Vec2& pos, FingerDirection dir);
    void hide();

private:
    void showLabel(cocos2d::Label* label, const cocos2d::Vec2& pos, const std::string& text);
    cocos2d::Label* m_label{nullptr};
    cocos2d::Label* m_secondaryLabel{nullptr};
    cocos2d::RenderTexture* m_leftRect{nullptr};
    cocos2d::RenderTexture* m_rightRect{nullptr};
    cocos2d::Sprite* m_finger{nullptr};
    cocos2d::Node* m_overlay{nullptr};
};

#endif  // __TUTORIAL_OVERLAY_H__
