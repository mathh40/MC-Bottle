#include "GenLayerVoronoiZoom.h"
#include "IntCache.h"

GenLayerVoronoiZoom::GenLayerVoronoiZoom(int64_t p_i2133_1_, std::shared_ptr<GenLayer> p_i2133_3_)
	:GenLayer(p_i2133_1_)
{
	parent = p_i2133_3_;
}

std::vector<int32_t> GenLayerVoronoiZoom::getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight)
{
	areaX -= 2;
	areaY -= 2;
	auto i = areaX >> 2;
	auto j = areaY >> 2;
	auto k = (areaWidth >> 2) + 2;
	auto l = (areaHeight >> 2) + 2;
	auto aint = parent->getInts(i, j, k, l);
	auto i1 = k - 1 << 2;
	auto j1 = l - 1 << 2;
	auto aint1 = IntCache::getIntCache(i1 * j1);

	for (auto k1 = 0; k1 < l - 1; ++k1) 
	{
		auto l1 = 0;
		auto i2 = aint[l1 + 0 + (k1 + 0) * k];

		for (auto j2 = aint[l1 + 0 + (k1 + 1) * k]; l1 < k - 1; ++l1) 
		{
			auto d0 = 3.6;
			initChunkSeed((int64_t)(l1 + i << 2), (int64_t)(k1 + j << 2));
			auto d1 = ((double)nextInt(1024) / 1024.0 - 0.5) * 3.6;
			auto d2 = ((double)nextInt(1024) / 1024.0 - 0.5) * 3.6;
			initChunkSeed((int64_t)(l1 + i + 1 << 2), (int64_t)(k1 + j << 2));
			auto d3 = ((double)nextInt(1024) / 1024.0 - 0.5) * 3.6 + 4.0;
			auto d4 = ((double)nextInt(1024) / 1024.0 - 0.5) * 3.6;
			initChunkSeed((int64_t)(l1 + i << 2), (int64_t)(k1 + j + 1 << 2));
			auto d5 = ((double)nextInt(1024) / 1024.0 - 0.5) * 3.6;
			auto d6 = ((double)nextInt(1024) / 1024.0 - 0.5) * 3.6 + 4.0;
			initChunkSeed((int64_t)(l1 + i + 1 << 2), (int64_t)(k1 + j + 1 << 2));
			auto d7 = ((double)nextInt(1024) / 1024.0 - 0.5) * 3.6 + 4.0;
			auto d8 = ((double)nextInt(1024) / 1024.0 - 0.5) * 3.6 + 4.0;
			auto k2 = aint[l1 + 1 + (k1 + 0) * k] & 255;
			auto l2 = aint[l1 + 1 + (k1 + 1) * k] & 255;

			for (auto i3 = 0; i3 < 4; ++i3) 
			{
				auto j3 = ((k1 << 2) + i3) * i1 + (l1 << 2);

				for (auto k3 = 0; k3 < 4; ++k3)
				{
					auto d9 = ((double)i3 - d2) * ((double)i3 - d2) + ((double)k3 - d1) * ((double)k3 - d1);
					auto d10 = ((double)i3 - d4) * ((double)i3 - d4) + ((double)k3 - d3) * ((double)k3 - d3);
					auto d11 = ((double)i3 - d6) * ((double)i3 - d6) + ((double)k3 - d5) * ((double)k3 - d5);
					auto d12 = ((double)i3 - d8) * ((double)i3 - d8) + ((double)k3 - d7) * ((double)k3 - d7);
					if (d9 < d10 && d9 < d11 && d9 < d12) 
					{
						aint1[j3++] = i2;
					}
					else if (d10 < d9 && d10 < d11 && d10 < d12)
					{
						aint1[j3++] = k2;
					}
					else if (d11 < d9 && d11 < d10 && d11 < d12) 
					{
						aint1[j3++] = j2;
					}
					else 
					{
						aint1[j3++] = l2;
					}
				}
			}

			i2 = k2;
			j2 = l2;
		}
	}

	auto aint2 = IntCache::getIntCache(areaWidth * areaHeight);

	for (auto l1 = 0; l1 < areaHeight; ++l1)
	{
		std::copy(aint1.begin(), aint1.begin() + (l1 + (areaY & 3)) * i1 + (areaX & 3), aint2);
	}

	return aint2;
}
