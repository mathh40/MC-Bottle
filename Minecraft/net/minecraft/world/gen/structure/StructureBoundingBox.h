#pragma once
#include "NBTTagIntArray.h"

#include <cstdint>
#include <string>
#include <vector>

class Vec3i;
class EnumFacing;

class StructureBoundingBox
{
  public:
    int32_t minX;
    int32_t minY;
    int32_t minZ;
    int32_t maxX;
    int32_t maxY;
    int32_t maxZ;

    StructureBoundingBox() = default;
    explicit StructureBoundingBox(std::vector<int32_t> coords);
    StructureBoundingBox(int32_t xMin, int32_t yMin, int32_t zMin, int32_t xMax, int32_t yMax, int32_t zMax);
    StructureBoundingBox(const Vec3i &vec1, const Vec3i &vec2);
    StructureBoundingBox(int32_t xMin, int32_t zMin, int32_t xMax, int32_t zMax);

    bool intersectsWith(const StructureBoundingBox &structurebb) const;
    bool intersectsWith(int32_t minXIn, int32_t minZIn, int32_t maxXIn, int32_t maxZIn) const;
    void expandTo(const StructureBoundingBox &sbb);
    void offset(int32_t x, int32_t y, int32_t z);
    bool isVecInside(const Vec3i &vec) const;
    Vec3i getLength() const;
    int32_t getXSize() const;
    int32_t getYSize() const;
    int32_t getZSize() const;

    std::string toString();
    std::unique_ptr<NBTTagIntArray> toNBTTagIntArray() const;

    static StructureBoundingBox getNewBoundingBox();
    static StructureBoundingBox getComponentToAddBoundingBox(int32_t structureMinX, int32_t structureMinY,
                                                             int32_t structureMinZ, int32_t xMin, int yMin,
                                                             int32_t zMin, int32_t xMax, int32_t yMax, int32_t zMax,
                                                             EnumFacing facing);
    static StructureBoundingBox createProper(int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2, int32_t z2);

  private:
};
