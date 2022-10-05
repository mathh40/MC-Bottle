#include "ChunkPos.h"

#include "BlockPos.h"

ChunkPos::ChunkPos(int32_t x, int32_t z) : x(x), z(z)
{
}

ChunkPos::ChunkPos(const BlockPos &pos)
{
    x = pos.getx() >> 4;
    z = pos.gety() >> 4;
}

int64_t ChunkPos::asLong(int x, int z)
{
    return ((int64_t)x & 4294967295L) | ((int64_t)z & 4294967295L) << 32;
}

int32_t ChunkPos::getx() const
{
    return x;
}

int32_t ChunkPos::getz() const
{
    return z;
}

int32_t ChunkPos::getXStart() const
{
    return x << 4;
}

int32_t ChunkPos::getZStart() const
{
    return z << 4;
}

int32_t ChunkPos::getXEnd() const
{
    return (x << 4) + 15;
}

int32_t ChunkPos::getZEnd() const
{
    return (z << 4) + 15;
}

BlockPos ChunkPos::getBlock(int x, int y, int z)
{
    return BlockPos((x << 4) + x, y, (z << 4) + z);
}

bool operator==(const ChunkPos &a, const ChunkPos &b)
{
    return a.getx() == b.getx() && a.getz() == b.getz();
}

std::string to_string(const ChunkPos &a)
{
    return "[" + std::to_string(a.getx()) + ", " + std::to_string(a.getz()) + "]";
}

double ChunkPos::getDistanceSq(Entity *entityIn)
{
    double d0 = (double)(x * 16 + 8);
    double d1 = (double)(z * 16 + 8);
    double d2 = d0 - entityIn->posX;
    double d3 = d1 - entityIn->posZ;
    return d2 * d2 + d3 * d3;
}
