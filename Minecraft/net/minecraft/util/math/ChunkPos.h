#pragma once
#include <cstdint>
#include "Vec3i.h"
#include "BlockPos.h"

class Entity;

class ChunkPos
{
public:
	ChunkPos(int32_t x, int32_t z);
	explicit ChunkPos(const BlockPos& pos);

	static int64_t asLong(int x, int z);

	double getDistanceSq(Entity* entityIn);
	int32_t getx() const;
	int32_t getz() const;

	int32_t getXStart() const;
	int32_t getZStart() const;
	int32_t getXEnd() const;
	int32_t getZEnd() const;

	BlockPos getBlock(int x, int y, int z);
private:
	int32_t x;
	int32_t z;
};

bool operator==(const ChunkPos& a, const ChunkPos& b);
std::string to_string(const ChunkPos& a);

namespace std {
	template <> struct hash<ChunkPos>
	{
		size_t operator()(const ChunkPos& pos) const noexcept
		{
			auto i = 1664525 * pos.getx() + 1013904223;
			auto j = 1664525 * (pos.getz() ^ -559038737) + 1013904223;
			return i ^ j;
		}
	};
}
