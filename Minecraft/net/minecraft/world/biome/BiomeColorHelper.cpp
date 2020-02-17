#include "BiomeColorHelper.h"

namespace BiomeColorHelper
{
	int32_t getColorAtPos(IBlockAccess* blockAccess, BlockPos& pos, ColorResolver colorResolver) 
	{
		auto i = 0;
		auto j = 0;
		auto k = 0;

		int l;

		auto box = getAllInBoxMutable(pos.add(-1, 0, -1), pos.add(1, 0, 1));
		for(auto mutableblockpos : box)
		{
			l = colorResolver(blockAccess->getBiome(mutableblockpos), mutableblockpos);
			i += (l & 16711680) >> 16;
			j += (l & '\uff00') >> 8;
			k += l & 255;
		}

		return (i / 9 & 255) << 16 | (j / 9 & 255) << 8 | k / 9 & 255;
	}

	int32_t getGrassColorAtPos(IBlockAccess* blockAccess, BlockPos& pos)
	{
		return getColorAtPos(blockAccess, pos, GRASS_COLOR);
	}

	int32_t getFoliageColorAtPos(IBlockAccess* blockAccess, BlockPos& pos)
	{
		return getColorAtPos(blockAccess, pos, FOLIAGE_COLOR);
	}

	int32_t getWaterColorAtPos(IBlockAccess* blockAccess, BlockPos& pos)
	{
		return getColorAtPos(blockAccess, pos, WATER_COLOR);
	}
}
