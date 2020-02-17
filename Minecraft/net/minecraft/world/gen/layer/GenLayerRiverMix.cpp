#include "GenLayerRiverMix.h"
#include "IntCache.h"
#include "biome/Biome.h"

GenLayerRiverMix::GenLayerRiverMix(int64_t p_i2129_1_, std::shared_ptr<GenLayer> p_i2129_3_, std::shared_ptr<GenLayer> p_i2129_4_)
	:GenLayer(p_i2129_1_)
{	
	biomePatternGeneratorChain = p_i2129_3_;
	riverPatternGeneratorChain = p_i2129_4_;
}

void GenLayerRiverMix::initWorldGenSeed(int64_t seed)
{
	biomePatternGeneratorChain->initWorldGenSeed(seed);
	riverPatternGeneratorChain->initWorldGenSeed(seed);
	GenLayer::initWorldGenSeed(seed);
}

std::vector<int32_t> GenLayerRiverMix::getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight)
{
	auto aint = biomePatternGeneratorChain->getInts(areaX, areaY, areaWidth, areaHeight);
	auto aint1 = riverPatternGeneratorChain->getInts(areaX, areaY, areaWidth, areaHeight);
	auto aint2 = IntCache::getIntCache(areaWidth * areaHeight);

	for (auto i = 0; i < areaWidth * areaHeight; ++i) 
	{
		if (aint[i] != Biome::getIdForBiome(Biomes::OCEAN) && aint[i] != Biome::getIdForBiome(Biomes::DEEP_OCEAN)) {
			if (aint1[i] == Biome::getIdForBiome(Biomes::RIVER))
			{
				if (aint[i] == Biome::getIdForBiome(Biomes::ICE_PLAINS))
				{
					aint2[i] = Biome::getIdForBiome(Biomes::FROZEN_RIVER);
				}
				else if (aint[i] != Biome::getIdForBiome(Biomes::MUSHROOM_ISLAND) && aint[i] != Biome::getIdForBiome(Biomes::MUSHROOM_ISLAND_SHORE)) {
					aint2[i] = aint1[i] & 255;
				}
				else {
					aint2[i] = Biome::getIdForBiome(Biomes::MUSHROOM_ISLAND_SHORE);
				}
			}
			else {
				aint2[i] = aint[i];
			}
		}
		else {
			aint2[i] = aint[i];
		}
	}

	return aint2;
}
