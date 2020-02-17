#include "GenLayerShore.h"
#include "IntCache.h"
#include "biome/Biome.h"
#include "biome/BiomeJungle.h"
#include "biome/BiomeMesa.h"
#include "Util.h"

GenLayerShore::GenLayerShore(int64_t p_i2130_1_, std::shared_ptr<GenLayer> p_i2130_3_)
	:GenLayer(p_i2130_1_)
{
	parent = p_i2130_3_;
}

std::vector<int32_t> GenLayerShore::getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight)
{
	auto aint = parent->getInts(areaX - 1, areaY - 1, areaWidth + 2, areaHeight + 2);
	auto aint1 = IntCache::getIntCache(areaWidth * areaHeight);

	for (auto i = 0; i < areaHeight; ++i) 
	{
		for (auto j = 0; j < areaWidth; ++j) 
		{
			initChunkSeed((int64_t)(j + areaX), (int64_t)(i + areaY));
			auto k = aint[j + 1 + (i + 1) * (areaWidth + 2)];
			Biome* biome = Biome::getBiome(k);
			if (k == Biome::getIdForBiome(Biomes::MUSHROOM_ISLAND))
			{
				auto l = aint[j + 1 + (i + 1 - 1) * (areaWidth + 2)];
				auto i1 = aint[j + 1 + 1 + (i + 1) * (areaWidth + 2)];
				auto j1 = aint[j + 1 - 1 + (i + 1) * (areaWidth + 2)];
				auto k1 = aint[j + 1 + (i + 1 + 1) * (areaWidth + 2)];
				if (l != Biome::getIdForBiome(Biomes::OCEAN) && i1 != Biome::getIdForBiome(Biomes::OCEAN) && j1 != Biome::getIdForBiome(Biomes::OCEAN) && k1 != Biome::getIdForBiome(Biomes::OCEAN)) {
					aint1[j + i * areaWidth] = k;
				}
				else 
				{
					aint1[j + i * areaWidth] = Biome::getIdForBiome(Biomes::MUSHROOM_ISLAND_SHORE);
				}
			}
			else if (biome != nullptr && biome::getBiomeClass() == BiomeJungle.class) 
			{
				auto l = aint[j + 1 + (i + 1 - 1) * (areaWidth + 2)];
				auto i1 = aint[j + 1 + 1 + (i + 1) * (areaWidth + 2)];
				auto j1 = aint[j + 1 - 1 + (i + 1) * (areaWidth + 2)];
				auto k1 = aint[j + 1 + (i + 1 + 1) * (areaWidth + 2)];
				if (isJungleCompatible(l) && isJungleCompatible(i1) && isJungleCompatible(j1) && isJungleCompatible(k1)) 
				{
					if (!isBiomeOceanic(l) && !isBiomeOceanic(i1) && !isBiomeOceanic(j1) && !isBiomeOceanic(k1)) 
					{
						aint1[j + i * areaWidth] = k;
					}
					else {
						aint1[j + i * areaWidth] = Biome::getIdForBiome(Biomes::BEACH);
					}
				}
				else {
					aint1[j + i * areaWidth] = Biome::getIdForBiome(Biomes::JUNGLE_EDGE);
				}
			}
			else if (k != Biome::getIdForBiome(Biomes::EXTREME_HILLS) && k != Biome::getIdForBiome(Biomes::EXTREME_HILLS_WITH_TREES) && k != Biome::getIdForBiome(Biomes::EXTREME_HILLS_EDGE)) {
				if (biome != nullptr && biome->isSnowyBiome()) 
				{
					replaceIfNeighborOcean(aint, aint1, j, i, areaWidth, k, Biome::getIdForBiome(Biomes::COLD_BEACH));
				}
				else if (k != Biome::getIdForBiome(Biomes::MESA) && k != Biome::getIdForBiome(Biomes::MESA_ROCK)) 
				{
					if (k != Biome::getIdForBiome(Biomes::OCEAN) && k != Biome::getIdForBiome(Biomes::DEEP_OCEAN) && k != Biome::getIdForBiome(Biomes::RIVER) && k != Biome::getIdForBiome(Biomes::SWAMPLAND)) {
						auto l = aint[j + 1 + (i + 1 - 1) * (areaWidth + 2)];
						auto i1 = aint[j + 1 + 1 + (i + 1) * (areaWidth + 2)];
						auto j1 = aint[j + 1 - 1 + (i + 1) * (areaWidth + 2)];
						auto k1 = aint[j + 1 + (i + 1 + 1) * (areaWidth + 2)];
						if (!isBiomeOceanic(l) && !isBiomeOceanic(i1) && !isBiomeOceanic(j1) && !isBiomeOceanic(k1)) 
						{
							aint1[j + i * areaWidth] = k;
						}
						else 
						{
							aint1[j + i * areaWidth] = Biome::getIdForBiome(Biomes::BEACH);
						}
					}
					else 
					{
						aint1[j + i * areaWidth] = k;
					}
				}
				else 
				{
					auto l = aint[j + 1 + (i + 1 - 1) * (areaWidth + 2)];
					auto i1 = aint[j + 1 + 1 + (i + 1) * (areaWidth + 2)];
					auto j1 = aint[j + 1 - 1 + (i + 1) * (areaWidth + 2)];
					auto k1 = aint[j + 1 + (i + 1 + 1) * (areaWidth + 2)];
					if (!isBiomeOceanic(l) && !isBiomeOceanic(i1) && !isBiomeOceanic(j1) && !isBiomeOceanic(k1)) 
					{
						if (isMesa(l) && isMesa(i1) && isMesa(j1) && isMesa(k1)) 
						{
							aint1[j + i * areaWidth] = k;
						}
						else 
						{
							aint1[j + i * areaWidth] = Biome::getIdForBiome(Biomes::DESERT);
						}
					}
					else 
					{
						aint1[j + i * areaWidth] = k;
					}
				}
			}
			else 
			{
				replaceIfNeighborOcean(aint, aint1, j, i, areaWidth, k, Biome::getIdForBiome(Biomes::STONE_BEACH));
			}
		}
	}

	return aint1;
}

void GenLayerShore::replaceIfNeighborOcean(const std::vector<int32_t> p_151632_1_,
	std::vector<int32_t>& p_151632_2_, int32_t p_151632_3_, int32_t p_151632_4_, int32_t p_151632_5_,
	int32_t p_151632_6_, int32_t p_151632_7_)
{
	if (isBiomeOceanic(p_151632_6_)) 
	{
		p_151632_2_[p_151632_3_ + p_151632_4_ * p_151632_5_] = p_151632_6_;
	}
	else {
		auto i = p_151632_1_[p_151632_3_ + 1 + (p_151632_4_ + 1 - 1) * (p_151632_5_ + 2)];
		auto j = p_151632_1_[p_151632_3_ + 1 + 1 + (p_151632_4_ + 1) * (p_151632_5_ + 2)];
		auto k = p_151632_1_[p_151632_3_ + 1 - 1 + (p_151632_4_ + 1) * (p_151632_5_ + 2)];
		auto l = p_151632_1_[p_151632_3_ + 1 + (p_151632_4_ + 1 + 1) * (p_151632_5_ + 2)];
		if (!isBiomeOceanic(i) && !isBiomeOceanic(j) && !isBiomeOceanic(k) && !isBiomeOceanic(l)) 
		{
			p_151632_2_[p_151632_3_ + p_151632_4_ * p_151632_5_] = p_151632_6_;
		}
		else 
		{
			p_151632_2_[p_151632_3_ + p_151632_4_ * p_151632_5_] = p_151632_7_;
		}
	}
}

bool GenLayerShore::isJungleCompatible(int32_t p_151631_1_)
{
	if (Biome::getBiome(p_151631_1_) != nullptr && Biome::getBiome(p_151631_1_).getBiomeClass() == BiomeJungle.class) 
	{
		return true;
	}
	else 
	{
		return p_151631_1_ == Biome::getIdForBiome(Biomes::JUNGLE_EDGE) || p_151631_1_ == Biome::getIdForBiome(Biomes::JUNGLE) || p_151631_1_ == Biome::getIdForBiome(Biomes::JUNGLE_HILLS) || p_151631_1_ == Biome::getIdForBiome(Biomes::FOREST) || p_151631_1_ == Biome::getIdForBiome(Biomes::TAIGA) || isBiomeOceanic(p_151631_1_);
	}
}

bool GenLayerShore::isMesa(int32_t p_151633_1_) const
{
	return Util::instanceof <BiomeMesa>(Biome::getBiome(p_151633_1_));
}
