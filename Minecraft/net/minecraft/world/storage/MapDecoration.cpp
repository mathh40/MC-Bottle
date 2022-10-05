#include "MapDecoration.h"

#include "math/MathHelper.h"

MapDecoration::Type MapDecoration::Type::PLAYER(false);
MapDecoration::Type MapDecoration::Type::FRAME(true);
MapDecoration::Type MapDecoration::Type::RED_MARKER(false);
MapDecoration::Type MapDecoration::Type::BLUE_MARKER(false);
MapDecoration::Type MapDecoration::Type::TARGET_X(true);
MapDecoration::Type MapDecoration::Type::TARGET_POINT(true);
MapDecoration::Type MapDecoration::Type::PLAYER_OFF_MAP(false);
MapDecoration::Type MapDecoration::Type::PLAYER_OFF_LIMITS(false);
MapDecoration::Type MapDecoration::Type::MANSION(true, 5393476);
MapDecoration::Type MapDecoration::Type::MONUMENT(true, 3830373);

uint8_t MapDecoration::Type::getIcon() const
{
    return icon;
}

bool MapDecoration::Type::isRenderedOnFrame() const
{
    return renderedOnFrame;
}

bool MapDecoration::Type::hasMapColor() const
{
    return mapColor >= 0;
}

uint32_t MapDecoration::Type::getMapColor() const
{
    return mapColor;
}

MapDecoration::Type *MapDecoration::Type::byIcon(uint8_t p_191159_0_) const
{
    return values[MathHelper::clamp<uint8_t>(p_191159_0_, 0, values.size() - 1)];
}

MapDecoration::Type::Type(bool p_i47343_3_) : Type(p_i47343_3_, -1)
{
}

MapDecoration::Type::Type(bool p_i47344_3_, uint32_t p_i47344_4_)
    : ordinal(count), icon(ordinal), renderedOnFrame(p_i47344_3_), mapColor(p_i47344_4_)
{
    count++;
}

MapDecoration::MapDecoration(MapDecoration::Type typeIn, uint8_t xIn, uint8_t yIn, uint8_t rotationIn)
    : type(typeIn), x(xIn), y(yIn), rotation(rotationIn)
{
}

uint8_t MapDecoration::getImage() const
{
    return type.getIcon();
}

MapDecoration::Type MapDecoration::getType() const
{
    return type;
}

uint8_t MapDecoration::getX() const
{
    return x;
}

uint8_t MapDecoration::getY() const
{
    return y;
}

uint8_t MapDecoration::getRotation() const
{
    return rotation;
}

bool MapDecoration::renderOnFrame() const
{
    return type.isRenderedOnFrame();
}

bool operator==(const MapDecoration::Type &lhs, const MapDecoration::Type &rhs)
{
    return lhs.ordinal == rhs.ordinal;
}

bool operator!=(const MapDecoration::Type &lhs, const MapDecoration::Type &rhs)
{
    return !(lhs == rhs);
}

bool operator==(const MapDecoration &lhs, const MapDecoration &rhs)
{
    if (lhs.type != rhs.type)
    {
        return false;
    }
    else if (lhs.rotation != rhs.rotation)
    {
        return false;
    }
    else if (lhs.x != rhs.x)
    {
        return false;
    }
    else
    {
        return lhs.y == rhs.y;
    }
}
