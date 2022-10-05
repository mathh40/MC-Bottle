#include "Vec2f.h"

#include <limits>

Vec2f Vec2f::ZERO            = Vec2f(0.0F, 0.0F);
Vec2f Vec2f::ONE             = Vec2f(1.0F, 1.0F);
Vec2f Vec2f::UNIT_X          = Vec2f(1.0F, 0.0F);
Vec2f Vec2f::NEGATIVE_UNIT_X = Vec2f(-1.0F, 0.0F);
Vec2f Vec2f::UNIT_Y          = Vec2f(0.0F, 1.0F);
Vec2f Vec2f::NEGATIVE_UNIT_Y = Vec2f(0.0F, -1.0F);
Vec2f Vec2f::MAX             = Vec2f(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
Vec2f Vec2f::MIN             = Vec2f(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

Vec2f::Vec2f(float xIn, float yIn) : x(xIn), y(yIn)
{
}
