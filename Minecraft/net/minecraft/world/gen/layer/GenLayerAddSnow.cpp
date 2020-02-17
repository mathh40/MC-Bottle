#include "GenLayerAddSnow.h"
#include "IntCache.h"

GenLayerAddSnow::GenLayerAddSnow(int64_t p_i2121_1_, std::shared_ptr<GenLayer> p_i2121_3_)
	:GenLayer(p_i2121_1_)
{
	parent = p_i2121_3_;
}

std::vector<int32_t> GenLayerAddSnow::getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight)
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
			auto k1 = aint[j1 + 1 + (i1 + 1) * k];
			initChunkSeed((int64_t)(j1 + areaX), (int64_t)(i1 + areaY));
			if (k1 == 0) 
			{
				aint1[j1 + i1 * areaWidth] = 0;
			}
			else 
			{
				auto l1 = nextInt(6);
				if (l1 == 0) 
				{
					l1 = 4;
				}
				else if (l1 <= 1) 
				{
					l1 = 3;
				}
				else 
				{
					l1 = 1;
				}

				aint1[j1 + i1 * areaWidth] = l1;
			}
		}
	}

	return aint1;
}
