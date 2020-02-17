#include "GenLayerDeepOcean.h"
#include "IntCache.h"
#include "biome/Biome.h"

GenLayerDeepOcean::GenLayerDeepOcean(int64_t p_i45472_1_, std::shared_ptr<GenLayer> p_i45472_3_)
	:GenLayer(p_i45472_1_)
{
	parent = p_i45472_3_;
}

std::vector<int32_t> GenLayerDeepOcean::getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight)
{
	auto i = areaX - 1;
	auto j = areaY - 1;
	auto k = areaWidth + 2;
	auto l = areaHeight + 2;
	auto aint = parent->getInts(i, j, k, l);
	auto aint1 = IntCache::getIntCache(areaWidth * areaHeight);

	for (auto i1 = 0; i1 < areaHeight; ++i1)
	{
		for (auto j1 = 0; j1 < areaWidth; ++j1) 
		{
			auto k1 = aint[j1 + 1 + (i1 + 1 - 1) * (areaWidth + 2)];
			auto l1 = aint[j1 + 1 + 1 + (i1 + 1) * (areaWidth + 2)];
			auto i2 = aint[j1 + 1 - 1 + (i1 + 1) * (areaWidth + 2)];
			auto j2 = aint[j1 + 1 + (i1 + 1 + 1) * (areaWidth + 2)];
			auto k2 = aint[j1 + 1 + (i1 + 1) * k];
			auto l2 = 0;
			if (k1 == 0) 
			{
				++l2;
			}

			if (l1 == 0) 
			{
				++l2;
			}

			if (i2 == 0) 
			{
				++l2;
			}

			if (j2 == 0) 
			{
				++l2;
			}

			if (k2 == 0 && l2 > 3) 
			{
				aint1[j1 + i1 * areaWidth] = Biome::getIdForBiome(Biomes::DEEP_OCEAN);
			}
			else 
			{
				aint1[j1 + i1 * areaWidth] = k2;
			}
		}
	}

	return aint1;
}
