#include "GenLayerRiver.h"
#include "IntCache.h"
#include "biome/Biome.h"

GenLayerRiver::GenLayerRiver(int64_t p_i2128_1_, std::shared_ptr<GenLayer> p_i2128_3_)
	:GenLayer(p_i2128_1_)
{
	parent = p_i2128_3_;
}

std::vector<int32_t> GenLayerRiver::getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight)
{
	auto i = areaX - 1;
	auto j = areaY - 1;
	auto k = areaWidth + 2;
	auto l = areaHeight + 2;
	auto aint = parent->getInts(i, j, k, l);
	auto aint1 = IntCache::getIntCache(areaWidth * areaHeight);

	for (auto i1 = 0; i1 < areaHeight; ++i1) {
		for (auto j1 = 0; j1 < areaWidth; ++j1) {
			auto k1 = riverFilter(aint[j1 + 0 + (i1 + 1) * k]);
			auto l1 = riverFilter(aint[j1 + 2 + (i1 + 1) * k]);
			auto i2 = riverFilter(aint[j1 + 1 + (i1 + 0) * k]);
			auto j2 = riverFilter(aint[j1 + 1 + (i1 + 2) * k]);
			auto k2 = riverFilter(aint[j1 + 1 + (i1 + 1) * k]);
			if (k2 == k1 && k2 == i2 && k2 == l1 && k2 == j2) {
				aint1[j1 + i1 * areaWidth] = -1;
			}
			else {
				aint1[j1 + i1 * areaWidth] = Biome::getIdForBiome(Biomes::RIVER);
			}
		}
	}

	return aint1;
}

int32_t GenLayerRiver::riverFilter(int32_t p_151630_1_)
{
	return p_151630_1_ >= 2 ? 2 + (p_151630_1_ & 1) : p_151630_1_;
}
