#include "GenLayerHills.h"
#include "spdlog/spdlog.h"
#include "IntCache.h"
#include "biome/Biome.h"

std::shared_ptr<spdlog::logger> GenLayerHills::LOGGER = spdlog::get("Minecraft")->clone("GenLayerHills");

GenLayerHills::GenLayerHills(int64_t p_i45479_1_, std::shared_ptr<GenLayer> parentIn, std::shared_ptr<GenLayer> riverLayerIn)
	:GenLayer(p_i45479_1_)
{
	parent = parentIn;
	riverLayer = riverLayerIn;
}

std::vector<int32_t> GenLayerHills::getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight)
{
	auto aint = parent.getInts(areaX - 1, areaY - 1, areaWidth + 2, areaHeight + 2);
	auto aint1 = riverLayer.getInts(areaX - 1, areaY - 1, areaWidth + 2, areaHeight + 2);
	auto aint2 = IntCache::getIntCache(areaWidth * areaHeight);

	for (auto i = 0; i < areaHeight; ++i) 
	{
		for (auto j = 0; j < areaWidth; ++j) 
		{
			initChunkSeed((int32_t)(j + areaX), (int32_t)(i + areaY));
			auto k = aint[j + 1 + (i + 1) * (areaWidth + 2)];
			auto l = aint1[j + 1 + (i + 1) * (areaWidth + 2)];
			bool flag = (l - 2) % 29 == 0;
			if (k > 255) 
			{
				LOGGER->debug("old! {}", k);
			}

			Biome* biome = Biome::getBiomeForId(k);
			bool flag1 = biome != nullptr && biome->isMutation();
			Biome* biome1;
			if (k != 0 && l >= 2 && (l - 2) % 29 == 1 && !flag1) 
			{
				biome1 = Biome::getMutationForBiome(biome);
				aint2[j + i * areaWidth] = biome1 == nullptr ? k : Biome::getIdForBiome(biome1);
			}
			else if (nextInt(3) != 0 && !flag) 
			{
				aint2[j + i * areaWidth] = k;
			}
			else 
			{
				biome1 = biome;
				int i1;
				if (biome == Biomes::DESERT) {
					biome1 = Biomes::DESERT_HILLS;
				}
				else if (biome == Biomes::FOREST) {
					biome1 = Biomes::FOREST_HILLS;
				}
				else if (biome == Biomes::BIRCH_FOREST) {
					biome1 = Biomes::BIRCH_FOREST_HILLS;
				}
				else if (biome == Biomes::ROOFED_FOREST) {
					biome1 = Biomes::PLAINS;
				}
				else if (biome == Biomes::TAIGA) {
					biome1 = Biomes::TAIGA_HILLS;
				}
				else if (biome == Biomes::REDWOOD_TAIGA) {
					biome1 = Biomes::REDWOOD_TAIGA_HILLS;
				}
				else if (biome == Biomes::COLD_TAIGA) {
					biome1 = Biomes::COLD_TAIGA_HILLS;
				}
				else if (biome == Biomes::PLAINS) {
					if (nextInt(3) == 0) {
						biome1 = Biomes::FOREST_HILLS;
					}
					else {
						biome1 = Biomes::FOREST;
					}
				}
				else if (biome == Biomes::ICE_PLAINS) {
					biome1 = Biomes::ICE_MOUNTAINS;
				}
				else if (biome == Biomes::JUNGLE) {
					biome1 = Biomes::JUNGLE_HILLS;
				}
				else if (biome == Biomes::OCEAN) {
					biome1 = Biomes::DEEP_OCEAN;
				}
				else if (biome == Biomes::EXTREME_HILLS) {
					biome1 = Biomes::EXTREME_HILLS_WITH_TREES;
				}
				else if (biome == Biomes::SAVANNA) {
					biome1 = Biomes::SAVANNA_PLATEAU;
				}
				else if (biomesEqualOrMesaPlateau(k, Biome::getIdForBiome(Biomes::MESA_ROCK))) {
					biome1 = Biomes::MESA;
				}
				else if (biome == Biomes::DEEP_OCEAN && nextInt(3) == 0) {
					i1 = nextInt(2);
					if (i1 == 0) {
						biome1 = Biomes::PLAINS;
					}
					else {
						biome1 = Biomes::FOREST;
					}
				}

				i1 = Biome::getIdForBiome(biome1);
				if (flag && i1 != k) {
					Biome* biome2 = Biome::getMutationForBiome(biome1);
					i1 = biome2 == nullptr ? k : Biome::getIdForBiome(biome2);
				}

				if (i1 == k) {
					aint2[j + i * areaWidth] = k;
				}
				else {
					auto k2 = aint[j + 1 + (i + 0) * (areaWidth + 2)];
					auto j1 = aint[j + 2 + (i + 1) * (areaWidth + 2)];
					auto k1 = aint[j + 0 + (i + 1) * (areaWidth + 2)];
					auto l1 = aint[j + 1 + (i + 2) * (areaWidth + 2)];
					auto i2 = 0;
					if (biomesEqualOrMesaPlateau(k2, k)) {
						++i2;
					}

					if (biomesEqualOrMesaPlateau(j1, k)) {
						++i2;
					}

					if (biomesEqualOrMesaPlateau(k1, k)) {
						++i2;
					}

					if (biomesEqualOrMesaPlateau(l1, k)) {
						++i2;
					}

					if (i2 >= 3) {
						aint2[j + i * areaWidth] = i1;
					}
					else {
						aint2[j + i * areaWidth] = k;
					}
				}
			}
		}
	}

	return aint2;
}
