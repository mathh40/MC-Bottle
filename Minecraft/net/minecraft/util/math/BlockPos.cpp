#include "BlockPos.h"
#include "Vec3d.h"
#include "EnumFacing.h"
#include "MathHelper.h"
#include "Mirror.h"
#include <spdlog/fmt/bundled/format.h>
#include "../../world/gen/structure/ComponentScatteredFeaturePieces.h"

BlockPos BlockPos::ORIGIN = BlockPos(0, 0, 0);

constexpr auto NUM_X_BITS = 1 + MathHelper::log2(MathHelper::smallestEncompassingPowerOfTwo(30000000));
constexpr uint32_t NUM_Z_BITS = NUM_X_BITS;
constexpr uint32_t NUM_Y_BITS = 64 - NUM_X_BITS - NUM_Z_BITS;
constexpr uint32_t Y_SHIFT = 0 + NUM_Z_BITS;
constexpr uint32_t X_SHIFT = Y_SHIFT + NUM_Y_BITS;
constexpr int64_t X_MASK = (1L << NUM_X_BITS) - 1L;
constexpr int64_t Y_MASK = (1L << NUM_Y_BITS) - 1L;
constexpr int64_t Z_MASK = (1L << NUM_Z_BITS) - 1L;

BlockPos::BlockPos()
	:Vec3i(0, 0, 0)
{
}

BlockPos::BlockPos(int32_t x, int32_t y, int32_t z)
	:Vec3i(x,y,z)
{
}

BlockPos::BlockPos(double x, double y, double z)
	: Vec3i(x, y, z)
{
}

BlockPos::BlockPos(const Entity* source)
	: Vec3i(source->posX, source->posY, source->posZ)
{
	
}

BlockPos::BlockPos(const Vec3d& vec)
	: Vec3i(vec.getx(), vec.gety(), vec.getz())
{

}

BlockPos::BlockPos(Vec3i source)
	: Vec3i(source.getx(), source.gety(), source.getz())
{
}

BlockPos::BlockPos(uint32_t i2, uint32_t j4, uint32_t k2)
	: Vec3i(x, y, z)
{
}

BlockPos BlockPos::add(double x, double y, double z) const
{
	return x == 0.0 && y == 0.0 && z == 0.0 ? *this : BlockPos((double)getx() + x, (double)gety() + y, (double)getz() + z);
}

BlockPos BlockPos::add(int32_t x, int32_t y, int32_t z) const
{
	return x == 0 && y == 0 && z == 0 ? *this : BlockPos(getx() + x, gety() + y, getz() + z);
}

BlockPos BlockPos::add(Vec3i vec) const
{
	return add(vec.getx(), vec.gety(), vec.getz());
}

BlockPos BlockPos::subtract(Vec3i vec) const
{
	return add(-vec.getx(), -vec.gety(), -vec.getz());
}

BlockPos BlockPos::up() const
{
	return up(1);
}

BlockPos BlockPos::up(int32_t n) const
{
	return offset(EnumFacing::UP, n);
}

BlockPos BlockPos::down() const
{
	return down(1);
}

BlockPos BlockPos::down(int32_t n) const
{
	return offset(EnumFacing::DOWN, n);
}

BlockPos BlockPos::north() const
{
	return north(1);
}

BlockPos BlockPos::north(int32_t n) const
{
	return offset(EnumFacing::NORTH, n);
}

BlockPos BlockPos::south() const
{
	return south(1);
}

BlockPos BlockPos::south(int32_t n) const
{
	return offset(EnumFacing::SOUTH, n);
}

BlockPos BlockPos::west() const
{
	return west(1);
}

BlockPos BlockPos::west(int32_t n) const
{
	return offset(EnumFacing::WEST, n);
}

BlockPos BlockPos::east() const
{
	return east(1);
}

BlockPos BlockPos::east(int32_t n) const
{
	return offset(EnumFacing::EAST, n);
}

BlockPos BlockPos::offset(EnumFacing facing) const
{
	return offset(facing, 1);
}

BlockPos BlockPos::offset(EnumFacing facing, int n) const
{
	return n == 0 ? *this : BlockPos(getx() + facing.getXOffset() * n, gety() + facing.getYOffset() * n, getz() + facing.getZOffset() * n);
}

BlockPos BlockPos::rotate(Rotation rotationIn)
{
	switch (rotationIn) 
	{
	case Mirror::NONE:
	default:
		return *this;
	case Rotation::CLOCKWISE_90:
		return BlockPos(-getz(), gety(), getx());
	case Rotation::CLOCKWISE_180:
		return BlockPos(-getx(), gety(), -getz());
	case Rotation::COUNTERCLOCKWISE_90:
		return BlockPos(getz(), gety(), -getx());
	}
}

BlockPos BlockPos::crossProduct(Vec3i vec) const
{
	return BlockPos(gety() * vec.getz() - getz() * vec.gety(), getz() * vec.getx() - getx() * vec.getz(), getx() * vec.gety() - gety() * vec.getx());
}

int64_t BlockPos::toLong() const
{
	return ((long)getx() & X_MASK) << X_SHIFT | ((long)gety() & Y_MASK) << Y_SHIFT | ((long)getz() & Z_MASK) << 0;
}

BlockPos BlockPos::fromLong(long serialized)
{
	int i = (int)(serialized << 64 - X_SHIFT - NUM_X_BITS >> 64 - NUM_X_BITS);
	int j = (int)(serialized << 64 - Y_SHIFT - NUM_Y_BITS >> 64 - NUM_Y_BITS);
	int k = (int)(serialized << 64 - NUM_Z_BITS >> 64 - NUM_Z_BITS);
	return BlockPos(i, j, k);
}

BlockPos BlockPos::setPos(int xIn, int yIn, int zIn)
{
	x = xIn;
	y = yIn;
	z = zIn;
	return *this;
}

BlockPos BlockPos::setPos(double xIn, double yIn, double zIn)
{
	return setPos(MathHelper::floor(xIn), MathHelper::floor(yIn), MathHelper::floor(zIn));
}

BlockPos BlockPos::setPos(Vec3i vec)
{
	return setPos(vec.getx(), vec.gety(), vec.getz());
}

BlockPos BlockPos::move(EnumFacing facing)
{
	return move(facing, 1);
}

BlockPos BlockPos::move(EnumFacing facing, int n)
{
	return setPos(x + facing.getXOffset() * n, y + facing.getYOffset() * n, z + facing.getZOffset() * n);
}

void BlockPos::setY(int32_t yIn)
{
	y = yIn;
}

void BlockPos::setX(int32_t int32)
{
	x = int32;
}

void BlockPos::setZ(int32_t int32)
{
	z = int32;
}

std::vector<MutableBlockPos> getAllInBoxMutable(int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2, int32_t z2)
{
	std::vector<MutableBlockPos> position{MutableBlockPos(x1, y1, z1)};
	auto count = 0;

	while (true)
	{
		auto pos = position[count];
		if (pos.getx() == x2 && pos.gety() == y2 && pos.getz() == z2)
		{
			break;
		}
		else
		{
			if (pos.getx() < x2)
			{
				pos.setX(pos.getx() + 1);
			}
			else if (pos.gety() < y2)
			{
				pos.setX(x1);
				pos.setY(pos.gety() + 1);
			}
			else if (pos.getz() < z2)
			{
				pos.setX(x1);
				pos.setY(y1);
				pos.setZ(pos.getz() + 1);
			}

			++count;
			position.emplace_back(pos);
		}
	}
	return position;
}

std::vector<MutableBlockPos> getAllInBoxMutable(const BlockPos& from, const BlockPos& to)
{
	return getAllInBoxMutable(MathHelper::min(from.getx(), to.getx()), MathHelper::min(from.gety(), to.gety()), MathHelper::min(from.getz(), to.getz()), MathHelper::max(from.getx(), to.getx()), MathHelper::max(from.gety(), to.gety()), MathHelper::max(from.getz(), to.getz()));
}

PooledMutableBlockPos PooledMutableBlockPos::retain()
{
	return retain(0, 0, 0);
}

PooledMutableBlockPos PooledMutableBlockPos::retain(double xIn, double yIn, double zIn)
{
	return retain(MathHelper::floor(xIn), MathHelper::floor(yIn), MathHelper::floor(zIn));
}

PooledMutableBlockPos PooledMutableBlockPos::retain(int32_t xIn, int32_t yIn, int32_t zIn)
{
	{
		std::lock_guard<std::mutex> lock(mux);
		if (!POOL.empty()) 
		{
			auto pooledmutableblockpos = POOL.erase(POOL.end() - 1);
			if (pooledmutableblockpos != POOL.end() && pooledmutableblockpos->released) 
			{
				pooledmutableblockpos->released = false;
				pooledmutableblockpos->setPos(xIn, yIn, zIn);
				return *pooledmutableblockpos;
			}
		}
	}

	return PooledMutableBlockPos(xIn, yIn, zIn);
}

PooledMutableBlockPos PooledMutableBlockPos::retain(Vec3i& vec)
{
	return retain(vec.getx(), vec.gety(), vec.getz());
}

void PooledMutableBlockPos::release()
{
	std::lock_guard<std::mutex> lock(mux);
	if (POOL.size() < 100) 
	{
		POOL.emplace_back(*this);
	}

	released = true;
}

PooledMutableBlockPos::PooledMutableBlockPos(int32_t xIn, int32_t yIn, int32_t zIn)
	:MutableBlockPos(xIn, yIn, zIn)
{
}

PooledMutableBlockPos::PooledMutableBlockPos(const PooledMutableBlockPos& lhs)
{
}
