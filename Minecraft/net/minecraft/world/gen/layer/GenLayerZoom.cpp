#include "GenLayerZoom.h"
#include "IntCache.h"

GenLayerZoom::GenLayerZoom(int64_t p_i2134_1_, std::shared_ptr<GenLayer> p_i2134_3_)
	:GenLayer(p_i2134_1_)
{
	parent = p_i2134_3_;
}

std::vector<int32_t> GenLayerZoom::getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight)
{
	auto i = areaX >> 1;
	auto j = areaY >> 1;
	auto k = (areaWidth >> 1) + 2;
	auto l = (areaHeight >> 1) + 2;
	auto aint = parent->getInts(i, j, k, l);
	auto i1 = k - 1 << 1;
	auto j1 = l - 1 << 1;
	auto aint1 = IntCache::getIntCache(i1 * j1);

	for (auto k1 = 0; k1 < l - 1; ++k1) 
	{
		auto j3 = (k1 << 1) * i1;
		auto i2 = 0;
		auto j2 = aint[i2 + 0 + (k1 + 0) * k];

		for (auto k2 = aint[i2 + 0 + (k1 + 1) * k]; i2 < k - 1; ++i2) 
		{
			initChunkSeed((int64_t)(i2 + i << 1), (int64_t)(k1 + j << 1));
			auto l2 = aint[i2 + 1 + (k1 + 0) * k];
			auto i3 = aint[i2 + 1 + (k1 + 1) * k];
			aint1[j3] = j2;
			aint1[j3++ + i1] = selectRandom({j2, k2});
			aint1[j3] = selectRandom({j2, l2});
			aint1[j3++ + i1] = selectModeOrRandom(j2, l2, k2, i3);
			j2 = l2;
			k2 = i3;
		}
	}

	auto aint2 = IntCache::getIntCache(areaWidth * areaHeight);

	for (auto j3 = 0; j3 < areaHeight; ++j3) 
	{
		auto src = aint1.begin() + (j3 + (areaY & 1)) * i1 + (areaX & 1);
		auto dest = aint2.begin() + j3 * areaWidth;
		std::copy_n(src, areaWidth, dest);
	}

	return aint2;
}

std::shared_ptr<GenLayer> GenLayerZoom::magnify(int64_t p_75915_0_, std::shared_ptr<GenLayer> p_75915_2_, int32_t p_75915_3_)
{
	auto genlayer = p_75915_2_;

	for (auto i = 0; i < p_75915_3_; ++i) {
		genlayer = std::make_shared<GenLayerZoom>(p_75915_0_ + (int64_t)i, genlayer);
	}

	return genlayer;
}
