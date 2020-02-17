#include "GenLayerRareBiome.h"
#include "IntCache.h"
#include "biome/Biome.h"

GenLayerRareBiome::GenLayerRareBiome(int64_t p_i45478_1_, std::shared_ptr<GenLayer> p_i45478_3_)
	:GenLayer(p_i45478_1_)
{
	parent = p_i45478_3_;
}

std::vector<int32_t> GenLayerRareBiome::getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight)
{
	auto aint = parent->getInts(areaX - 1, areaY - 1, areaWidth + 2, areaHeight + 2);
	auto aint1 = IntCache::getIntCache(areaWidth * areaHeight);

	for (auto i = 0; i < areaHeight; ++i) 
	{
		for (auto j = 0; j < areaWidth; ++j) 
		{
			initChunkSeed((int64_t)(j + areaX), (int64_t)(i + areaY));
			auto k = aint[j + 1 + (i + 1) * (areaWidth + 2)];
			if (nextInt(57) == 0) {
				if (k == Biome::getIdForBiome(Biomes::PLAINS)) {
					aint1[j + i * areaWidth] = Biome::getIdForBiome(Biomes::MUTATED_PLAINS);
				}
				else {
					aint1[j + i * areaWidth] = k;
				}
			}
			else {
				aint1[j + i * areaWidth] = k;
			}
		}
	}

	return aint1;
}
