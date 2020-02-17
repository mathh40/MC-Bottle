#include "GenLayerBiomeEdge.h"
#include "IntCache.h"
#include "biome/Biome.h"

GenLayerBiomeEdge::GenLayerBiomeEdge(int64_t p_i45475_1_, std::shared_ptr<GenLayer> p_i45475_3_)
	:GenLayer(p_i45475_1_)
{
	parent = p_i45475_3_;
}

std::vector<int32_t> GenLayerBiomeEdge::getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight)
{
	auto aint = parent->getInts(areaX - 1, areaY - 1, areaWidth + 2, areaHeight + 2);
	auto aint1 = IntCache::getIntCache(areaWidth * areaHeight);

	for (auto i = 0; i < areaHeight; ++i) 
	{
		for (auto j = 0; j < areaWidth; ++j) 
		{
			initChunkSeed((int64_t)(j + areaX), (int64_t)(i + areaY));
			auto k = aint[j + 1 + (i + 1) * (areaWidth + 2)];
			if (!replaceBiomeEdgeIfNecessary(aint, aint1, j, i, areaWidth, k, Biome::getIdForBiome(Biomes::EXTREME_HILLS), Biome::getIdForBiome(Biomes::EXTREME_HILLS_EDGE)) && !replaceBiomeEdge(aint, aint1, j, i, areaWidth, k, Biome::getIdForBiome(Biomes::MESA_ROCK), Biome::getIdForBiome(Biomes::MESA)) && !replaceBiomeEdge(aint, aint1, j, i, areaWidth, k, Biome::getIdForBiome(Biomes::MESA_CLEAR_ROCK), Biome::getIdForBiome(Biomes::MESA)) && !replaceBiomeEdge(aint, aint1, j, i, areaWidth, k, Biome::getIdForBiome(Biomes::REDWOOD_TAIGA), Biome::getIdForBiome(Biomes::TAIGA))) 
			{
				if (k == Biome::getIdForBiome(Biomes::DESERT)) 
				{
					auto l = aint[j + 1 + (i + 1 - 1) * (areaWidth + 2)];
					auto i1 = aint[j + 1 + 1 + (i + 1) * (areaWidth + 2)];
					auto j1 = aint[j + 1 - 1 + (i + 1) * (areaWidth + 2)];
					auto k1 = aint[j + 1 + (i + 1 + 1) * (areaWidth + 2)];
					if (l != Biome::getIdForBiome(Biomes::ICE_PLAINS) && i1 != Biome::getIdForBiome(Biomes::ICE_PLAINS) && j1 != Biome::getIdForBiome(Biomes::ICE_PLAINS) && k1 != Biome::getIdForBiome(Biomes::ICE_PLAINS)) {
						aint1[j + i * areaWidth] = k;
					}
					else 
					{
						aint1[j + i * areaWidth] = Biome::getIdForBiome(Biomes::EXTREME_HILLS_WITH_TREES);
					}
				}
				else if (k == Biome::getIdForBiome(Biomes::SWAMPLAND)) 
				{
					auto l = aint[j + 1 + (i + 1 - 1) * (areaWidth + 2)];
					auto i1 = aint[j + 1 + 1 + (i + 1) * (areaWidth + 2)];
					auto j1 = aint[j + 1 - 1 + (i + 1) * (areaWidth + 2)];
					auto k1 = aint[j + 1 + (i + 1 + 1) * (areaWidth + 2)];
					if (l != Biome::getIdForBiome(Biomes::DESERT) && i1 != Biome::getIdForBiome(Biomes::DESERT) && j1 != Biome::getIdForBiome(Biomes::DESERT) && k1 != Biome::getIdForBiome(Biomes::DESERT) && l != Biome::getIdForBiome(Biomes::COLD_TAIGA) && i1 != Biome::getIdForBiome(Biomes::COLD_TAIGA) && j1 != Biome::getIdForBiome(Biomes::COLD_TAIGA) && k1 != Biome::getIdForBiome(Biomes::COLD_TAIGA) && l != Biome::getIdForBiome(Biomes::ICE_PLAINS) && i1 != Biome::getIdForBiome(Biomes::ICE_PLAINS) && j1 != Biome::getIdForBiome(Biomes::ICE_PLAINS) && k1 != Biome::getIdForBiome(Biomes::ICE_PLAINS)) 
					{
						if (l != Biome::getIdForBiome(Biomes::JUNGLE) && k1 != Biome::getIdForBiome(Biomes::JUNGLE) && i1 != Biome::getIdForBiome(Biomes::JUNGLE) && j1 != Biome::getIdForBiome(Biomes::JUNGLE)) 
						{
							aint1[j + i * areaWidth] = k;
						}
						else 
						{
							aint1[j + i * areaWidth] = Biome::getIdForBiome(Biomes::JUNGLE_EDGE);
						}
					}
					else 
					{
						aint1[j + i * areaWidth] = Biome::getIdForBiome(Biomes::PLAINS);
					}
				}
				else 
				{
					aint1[j + i * areaWidth] = k;
				}
			}
		}
	}

	return aint1;
}

bool GenLayerBiomeEdge::replaceBiomeEdge(const std::vector<int32_t>& p_151635_1_,
	std::vector<int32_t>& p_151635_2_, int32_t p_151635_3_, int32_t p_151635_4_, int32_t p_151635_5_,
	int32_t p_151635_6_, int32_t p_151635_7_, int32_t p_151635_8_)
{
	if (p_151635_6_ != p_151635_7_) 
	{
		return false;
	}
	else 
	{
		auto i = p_151635_1_[p_151635_3_ + 1 + (p_151635_4_ + 1 - 1) * (p_151635_5_ + 2)];
		auto j = p_151635_1_[p_151635_3_ + 1 + 1 + (p_151635_4_ + 1) * (p_151635_5_ + 2)];
		auto k = p_151635_1_[p_151635_3_ + 1 - 1 + (p_151635_4_ + 1) * (p_151635_5_ + 2)];
		auto l = p_151635_1_[p_151635_3_ + 1 + (p_151635_4_ + 1 + 1) * (p_151635_5_ + 2)];
		if (biomesEqualOrMesaPlateau(i, p_151635_7_) && biomesEqualOrMesaPlateau(j, p_151635_7_) && biomesEqualOrMesaPlateau(k, p_151635_7_) && biomesEqualOrMesaPlateau(l, p_151635_7_)) 
		{
			p_151635_2_[p_151635_3_ + p_151635_4_ * p_151635_5_] = p_151635_6_;
		}
		else {
			p_151635_2_[p_151635_3_ + p_151635_4_ * p_151635_5_] = p_151635_8_;
		}

		return true;
	}
}

bool GenLayerBiomeEdge::canBiomesBeNeighbors(int32_t p_151634_1_, int32_t p_151634_2_)
{
	if (biomesEqualOrMesaPlateau(p_151634_1_, p_151634_2_)) {
		return true;
	}
	else 
	{
		auto biome = Biome::getBiome(p_151634_1_);
		auto biome1 = Biome::getBiome(p_151634_2_);
		if (biome != nullptr && biome1 != nullptr) 
		{
			Biome::TempCategory biome$tempcategory = biome->getTempCategory();
			Biome::TempCategory biome$tempcategory1 = biome1->getTempCategory();
			return biome$tempcategory == biome$tempcategory1 || biome$tempcategory == Biome::TempCategory::MEDIUM || biome$tempcategory1 == Biome::TempCategory::MEDIUM;
		}
		else 
		{
			return false;
		}
	}
}

bool GenLayerBiomeEdge::replaceBiomeEdgeIfNecessary(const std::vector<int32_t>& p_151636_1_, std::vector<int32_t>& p_151636_2_,
	int32_t p_151636_3_, int32_t p_151636_4_, int32_t p_151636_5_, int32_t p_151636_6_, int32_t p_151636_7_,
	int32_t p_151636_8_)
{
	if (!biomesEqualOrMesaPlateau(p_151636_6_, p_151636_7_)) {
		return false;
	}
	else {
		auto i = p_151636_1_[p_151636_3_ + 1 + (p_151636_4_ + 1 - 1) * (p_151636_5_ + 2)];
		auto j = p_151636_1_[p_151636_3_ + 1 + 1 + (p_151636_4_ + 1) * (p_151636_5_ + 2)];
		auto k = p_151636_1_[p_151636_3_ + 1 - 1 + (p_151636_4_ + 1) * (p_151636_5_ + 2)];
		auto l = p_151636_1_[p_151636_3_ + 1 + (p_151636_4_ + 1 + 1) * (p_151636_5_ + 2)];
		if (canBiomesBeNeighbors(i, p_151636_7_) && canBiomesBeNeighbors(j, p_151636_7_) && canBiomesBeNeighbors(k, p_151636_7_) && canBiomesBeNeighbors(l, p_151636_7_)) 
		{
			p_151636_2_[p_151636_3_ + p_151636_4_ * p_151636_5_] = p_151636_6_;
		}
		else 
		{
			p_151636_2_[p_151636_3_ + p_151636_4_ * p_151636_5_] = p_151636_8_;
		}

		return true;
	}
}
