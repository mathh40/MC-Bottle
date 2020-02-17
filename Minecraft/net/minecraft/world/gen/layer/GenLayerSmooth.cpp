#include "GenLayerSmooth.h"
#include "IntCache.h"

GenLayerSmooth::GenLayerSmooth(int64_t p_i2131_1_, std::shared_ptr<GenLayer> p_i2131_3_)
	:GenLayer(p_i2131_1_)
{
	parent = p_i2131_3_;
}

std::vector<int32_t> GenLayerSmooth::getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight)
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
			auto k1 = aint[j1 + 0 + (i1 + 1) * k];
			auto l1 = aint[j1 + 2 + (i1 + 1) * k];
			auto i2 = aint[j1 + 1 + (i1 + 0) * k];
			auto j2 = aint[j1 + 1 + (i1 + 2) * k];
			auto k2 = aint[j1 + 1 + (i1 + 1) * k];
			if (k1 == l1 && i2 == j2) 
			{
				initChunkSeed((int32_t)(j1 + areaX), (int32_t)(i1 + areaY));
				if (nextInt(2) == 0) 
				{
					k2 = k1;
				}
				else 
				{
					k2 = i2;
				}
			}
			else {
				if (k1 == l1) 
				{
					k2 = k1;
				}

				if (i2 == j2) 
				{
					k2 = i2;
				}
			}

			aint1[j1 + i1 * areaWidth] = k2;
		}
	}

	return aint1;
}
