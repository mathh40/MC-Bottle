#pragma once
#include "Vec3i.h"
#include <vector>

class Rotation;
class Entity;
class EnumFacing;
class Vec3d;

class BlockPos : public Vec3i
{
public:
	static BlockPos ORIGIN;
	BlockPos();
	BlockPos(int32_t x, int32_t y, int32_t z);
	BlockPos(double x, double y, double z);
	BlockPos(const Entity* source);
	explicit BlockPos(const Vec3d& vec);
	explicit BlockPos(Vec3i source);
	BlockPos(const BlockPos& lhs) = default;
	BlockPos& operator=(const BlockPos& lhs) = default;
	BlockPos(uint32_t i2, uint32_t j4, uint32_t k2);

	BlockPos add(double x, double y, double z) const;
	BlockPos add(int32_t x, int32_t y, int32_t z) const;
	BlockPos add(Vec3i vec) const;
	BlockPos subtract(Vec3i vec) const;
	BlockPos up() const;
	BlockPos up(int32_t n) const;
	BlockPos down() const;
	BlockPos down(int32_t n) const;
	BlockPos north() const;
	BlockPos north(int32_t n) const;
	BlockPos south() const;
	BlockPos south(int32_t n) const;
	BlockPos west() const;
	BlockPos west(int32_t n) const;
	BlockPos east() const;
	BlockPos east(int32_t n) const;

	BlockPos offset(EnumFacing facing) const;
	BlockPos offset(EnumFacing facing, int n) const;

	BlockPos rotate(Rotation rotationIn) const;
	BlockPos crossProduct(Vec3i vec) const;
	int64_t toLong() const;
	static BlockPos fromLong(long serialized);

	BlockPos setPos(int xIn, int yIn, int zIn);
	BlockPos setPos(double xIn, double yIn, double zIn);
	BlockPos setPos(Vec3i vec);
	BlockPos move(EnumFacing facing);
	BlockPos move(EnumFacing facing, int n);
	void setY(int32_t yIn);
	void setX(int32_t int32);
	void setZ(int32_t int32);
	
};

using MutableBlockPos = BlockPos;

std::vector<MutableBlockPos> getAllInBoxMutable(int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2, int32_t z2);
std::vector<MutableBlockPos> getAllInBoxMutable(const BlockPos& from, const BlockPos& to);
