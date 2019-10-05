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

#include "Box2DUtils.h"
#include "PTM.h"

namespace utils {
namespace box2d {

void setPosition(b2BodyDef& body, const cocos2d::Sprite& sprite)
{
    const auto worldCoords{sprite.convertToWorldSpace(
        {sprite.getBoundingBox().getMidX(), sprite.getBoundingBox().getMidY()})};
    body.position = {worldCoords.x / PTM::ptm, worldCoords.y / PTM::ptm};
}

b2PolygonShape getBoxShape(const cocos2d::Vec2& sizePixels)
{
    b2PolygonShape boxShape;
    // Box2D api operates on center point -> half width / height
    boxShape.SetAsBox(sizePixels.x / PTM::ptm / 2, sizePixels.y / PTM::ptm / 2);
    return boxShape;
}

b2Vec2 pixelsToMeters(const cocos2d::Vec2& pixelPos)
{
    return {pixelPos.x / PTM::ptm, pixelPos.y / PTM::ptm};
}

}  // namespace box2d
}  // namespace utils
