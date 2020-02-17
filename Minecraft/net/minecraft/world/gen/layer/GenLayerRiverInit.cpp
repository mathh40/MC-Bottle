#include "GenLayerRiverInit.h"
#include "IntCache.h"

GenLayerRiverInit::GenLayerRiverInit(int64_t p_i2127_1_, std::shared_ptr<GenLayer> p_i2127_3_)
	:GenLayer(p_i2127_1_)
{
	parent = p_i2127_3_;
}

std::vector<int32_t> GenLayerRiverInit::getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight)
{
	auto aint = parent->getInts(areaX, areaY, areaWidth, areaHeight);
	auto aint1 = IntCache::getIntCache(areaWidth * areaHeight);

	for (auto i = 0; i < areaHeight; ++i) 
	{
		for (auto j = 0; j < areaWidth; ++j) 
		{
			initChunkSeed((int64_t)(j + areaX), (int64_t)(i + areaY));
			aint1[j + i * areaWidth] = aint[j + i * areaWidth] > 0 ? nextInt(299999) + 2 : 0;
		}
	}

	return aint1;
}
