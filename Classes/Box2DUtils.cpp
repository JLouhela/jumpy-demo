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
#include "PhysicSObject.h"

namespace {
static constexpr float degToRad = 0.0174533f;
static constexpr float radToDeg = 57.2958f;

}  // namespace

namespace utils {
namespace box2d {

void setPosition(b2BodyDef& body, const cocos2d::Sprite& sprite)
{
    const auto worldCoords{sprite.convertToWorldSpace(sprite.getPosition())};
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

cocos2d::Vec2 metersToPixels(const b2Vec2& meterPos)
{
    return {meterPos.x * PTM::ptm, meterPos.y * PTM::ptm};
}

void transformSprite(cocos2d::Sprite& sprite, const b2Vec2& newPos, float newAngleRad)
{
    sprite.setPosition(metersToPixels(newPos));
    sprite.setRotation(newAngleRad * radToDeg);
}

void syncPhysicsToSprite(PhysicsObject& physObj, const b2Vec2& newPos, float newAngle, float alpha)
{
    if (physObj.prevPos.x < invalidCoord || physObj.prevPos.y < invalidCoord) {
        transformSprite(*physObj.sprite, newPos, newAngle);
        return;
    }
    const auto alphaRemainder = 1.0f - alpha;
    physObj.sprite->setPosition((newPos.x * alpha + physObj.prevPos.x * alphaRemainder) * PTM::ptm,
                                (newPos.y * alpha + physObj.prevPos.y * alphaRemainder) * PTM::ptm);
    physObj.sprite->setRotation(
        {newAngle * radToDeg * alpha + physObj.sprite->getRotation() * alphaRemainder});
}

}  // namespace box2d
}  // namespace utils
