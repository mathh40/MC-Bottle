#pragma once
#include "RayTraceResult.h"

#include <cstdint>
#include <optional>
#include <string>

class Vec3d;

class AxisAlignedBB
{
  public:
    AxisAlignedBB() = default;
    AxisAlignedBB(double x1, double y1, double z1, double x2, double y2, double z2);
    AxisAlignedBB(BlockPos pos);
    AxisAlignedBB(BlockPos pos1, BlockPos pos2);
    AxisAlignedBB(Vec3d min, Vec3d max);
    AxisAlignedBB setMaxY(double y2) const;

    AxisAlignedBB contract(double x, double y, double z) const;
    AxisAlignedBB expand(double x, double y, double z) const;
    AxisAlignedBB grow(double x, double y, double z) const;
    AxisAlignedBB grow(double value) const;
    AxisAlignedBB intersect(AxisAlignedBB other) const;
    AxisAlignedBB aunion(AxisAlignedBB other) const;
    AxisAlignedBB offset(double x, double y, double z) const;
    AxisAlignedBB offset(BlockPos pos) const;
    AxisAlignedBB offset(Vec3d vec) const;
    double calculateXOffset(AxisAlignedBB other, double offsetX) const;
    double calculateYOffset(AxisAlignedBB other, double offsetY) const;
    double calculateZOffset(AxisAlignedBB other, double offsetZ) const;
    bool intersects(AxisAlignedBB other) const;
    bool intersects(double x1, double y1, double z1, double x2, double y2, double z2) const;
    bool intersects(Vec3d min, Vec3d max) const;
    bool contains(Vec3d vec);
    double getAverageEdgeLength() const;
    AxisAlignedBB shrink(double value) const;
    std::optional<RayTraceResult> calculateIntercept(Vec3d vecA, Vec3d vecB);
    bool isClosest(Vec3d p_186661_1_, std::optional<Vec3d> p_186661_2_, Vec3d p_186661_3_);
    std::optional<Vec3d> collideWithXPlane(double p_186671_1_, Vec3d p_186671_3_, Vec3d p_186671_4_);
    std::optional<Vec3d> collideWithYPlane(double p_186671_1_, Vec3d p_186671_3_, Vec3d p_186671_4_);
    std::optional<Vec3d> collideWithZPlane(double p_186671_1_, Vec3d p_186671_3_, Vec3d p_186671_4_);

    bool intersectsWithYZ(Vec3d vec);
    bool intersectsWithXZ(Vec3d vec);
    bool intersectsWithXY(Vec3d vec);

    std::string to_string() const;
    bool hasNaN() const;
    Vec3d getCenter() const;

    friend bool operator==(const AxisAlignedBB &a, const AxisAlignedBB &b);
    double getminX() const;
    double getminY() const;
    double getminZ() const;
    double getmaxX() const;
    double getmaxY() const;
    double getmaxZ() const;

  private:
    double minX{};
    double minY{};
    double minZ{};
    double maxX{};
    double maxY{};
    double maxZ{};
};

namespace std
{
template <> struct hash<AxisAlignedBB>
{
    size_t operator()(const AxisAlignedBB &x) const noexcept
    {
        auto i = static_cast<int64_t>(x.getminX());
        auto j = i ^ i >> 32;
        i      = static_cast<int64_t>(x.getminY());
        j      = 31 * j + (i ^ i >> 32);
        i      = static_cast<int64_t>(x.getminZ());
        j      = 31 * j + (i ^ i >> 32);
        i      = static_cast<int64_t>(x.getmaxX());
        j      = 31 * j + (i ^ i >> 32);
        i      = static_cast<int64_t>(x.getmaxY());
        j      = 31 * j + (i ^ i >> 32);
        i      = static_cast<int64_t>(x.getmaxZ());
        j      = 31 * j + (i ^ i >> 32);
        return j;
    }
};
} // namespace std
