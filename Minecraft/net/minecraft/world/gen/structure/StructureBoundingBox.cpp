#include "StructureBoundingBox.h"

#include "EnumFacing.h"
#include "math/MathHelper.h"

StructureBoundingBox::StructureBoundingBox(std::vector<int32_t> coords)
{
    if (coords.size() == 6)
    {
        minX = coords[0];
        minY = coords[1];
        minZ = coords[2];
        maxX = coords[3];
        maxY = coords[4];
        maxZ = coords[5];
    }
}

StructureBoundingBox::StructureBoundingBox(int32_t xMin, int32_t yMin, int32_t zMin, int32_t xMax, int32_t yMax,
                                           int32_t zMax)
    : minX(xMin), minY(yMin), minZ(zMin), maxX(xMax), maxY(yMax), maxZ(zMax)
{
}

StructureBoundingBox::StructureBoundingBox(const Vec3i &vec1, const Vec3i &vec2)
    : minX(MathHelper::min(vec1.getx(), vec2.getx())), minY(MathHelper::min(vec1.gety(), vec2.gety())),
      minZ(MathHelper::min(vec1.getz(), vec2.getz())), maxX(MathHelper::max(vec1.getx(), vec2.getx())),
      maxY(MathHelper::max(vec1.gety(), vec2.gety())), maxZ(MathHelper::max(vec1.getz(), vec2.getz()))
{
}

StructureBoundingBox::StructureBoundingBox(int32_t xMin, int32_t zMin, int32_t xMax, int32_t zMax)
    : minX(xMin), minY(1), minZ(zMin), maxX(xMax), maxY(512), maxZ(zMax)
{
}

bool StructureBoundingBox::intersectsWith(const StructureBoundingBox &structurebb) const
{
    return maxX >= structurebb.minX && minX <= structurebb.maxX && maxZ >= structurebb.minZ &&
           minZ <= structurebb.maxZ && maxY >= structurebb.minY && minY <= structurebb.maxY;
}

bool StructureBoundingBox::intersectsWith(int32_t minXIn, int32_t minZIn, int32_t maxXIn, int32_t maxZIn) const
{
    return maxX >= minXIn && minX <= maxXIn && maxZ >= minZIn && minZ <= maxZIn;
}

void StructureBoundingBox::expandTo(const StructureBoundingBox &sbb)
{
    minX = MathHelper::min(minX, sbb.minX);
    minY = MathHelper::min(minY, sbb.minY);
    minZ = MathHelper::min(minZ, sbb.minZ);
    maxX = MathHelper::max(maxX, sbb.maxX);
    maxY = MathHelper::max(maxY, sbb.maxY);
    maxZ = MathHelper::max(maxZ, sbb.maxZ);
}

void StructureBoundingBox::offset(int32_t x, int32_t y, int32_t z)
{
    minX += x;
    minY += y;
    minZ += z;
    maxX += x;
    maxY += y;
    maxZ += z;
}

bool StructureBoundingBox::isVecInside(const Vec3i &vec) const
{
    return vec.getx() >= minX && vec.getx() <= maxX && vec.getz() >= minZ && vec.getz() <= maxZ && vec.gety() >= minY &&
           vec.gety() <= maxY;
}

Vec3i StructureBoundingBox::getLength() const
{
    return Vec3i(maxX - minX, maxY - minY, maxZ - minZ);
}

int32_t StructureBoundingBox::getXSize() const
{
    return maxX - minX + 1;
}

int32_t StructureBoundingBox::getYSize() const
{
    return maxY - minY + 1;
}

int32_t StructureBoundingBox::getZSize() const
{
    return maxZ - minZ + 1;
}

std::string StructureBoundingBox::toString()
{
    return MoreObjects.toStringHelper(this)
        .add("x0", minX)
        .add("y0", minY)
        .add("z0", minZ)
        .add("x1", maxX)
        .add("y1", maxY)
        .add("z1", maxZ)
        .toString();
}

std::unique_ptr<NBTTagIntArray> StructureBoundingBox::toNBTTagIntArray() const
{
    std::vector<int32_t> temp = {minX, minY, minZ, maxX, maxY, maxZ};
    return std::make_unique<NBTTagIntArray>(temp);
}

StructureBoundingBox StructureBoundingBox::getNewBoundingBox()
{
    return StructureBoundingBox(std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max(),
                                std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::min(),
                                std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::min());
}

StructureBoundingBox StructureBoundingBox::getComponentToAddBoundingBox(int32_t structureMinX, int32_t structureMinY,
                                                                        int32_t structureMinZ, int32_t xMin, int yMin,
                                                                        int32_t zMin, int32_t xMax, int32_t yMax,
                                                                        int32_t zMax, EnumFacing facing)
{
    switch (facing)
    {
    case EnumFacing::NORTH:
        return StructureBoundingBox(structureMinX + xMin, structureMinY + yMin, structureMinZ - zMax + 1 + zMin,
                                    structureMinX + xMax - 1 + xMin, structureMinY + yMax - 1 + yMin,
                                    structureMinZ + zMin);
    case EnumFacing::SOUTH:
        return StructureBoundingBox(structureMinX + xMin, structureMinY + yMin, structureMinZ + zMin,
                                    structureMinX + xMax - 1 + xMin, structureMinY + yMax - 1 + yMin,
                                    structureMinZ + zMax - 1 + zMin);
    case EnumFacing::WEST:
        return StructureBoundingBox(structureMinX - zMax + 1 + zMin, structureMinY + yMin, structureMinZ + xMin,
                                    structureMinX + zMin, structureMinY + yMax - 1 + yMin,
                                    structureMinZ + xMax - 1 + xMin);
    case EnumFacing::EAST:
        return StructureBoundingBox(structureMinX + zMin, structureMinY + yMin, structureMinZ + xMin,
                                    structureMinX + zMax - 1 + zMin, structureMinY + yMax - 1 + yMin,
                                    structureMinZ + xMax - 1 + xMin);
    default:
        return StructureBoundingBox(structureMinX + xMin, structureMinY + yMin, structureMinZ + zMin,
                                    structureMinX + xMax - 1 + xMin, structureMinY + yMax - 1 + yMin,
                                    structureMinZ + zMax - 1 + zMin);
    }
}

StructureBoundingBox StructureBoundingBox::createProper(int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2,
                                                        int32_t z2)
{
    return StructureBoundingBox(MathHelper::min(x1, x2), MathHelper::min(y1, y2), MathHelper::min(z1, z2),
                                MathHelper::max(x1, x2), MathHelper::max(y1, y2), MathHelper::max(z1, z2));
}
