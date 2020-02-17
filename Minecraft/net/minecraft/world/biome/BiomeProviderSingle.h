#pragma once
#include "BiomeProvider.h"

class BiomeProviderSingle :public BiomeProvider
{
public:
	BiomeProviderSingle(Biome* biomeIn);
	Biome* getBiome(BlockPos& pos) const override;
	std::vector<Biome*> getBiomesForGeneration(std::vector<Biome*> biomes, int32_t x, int32_t z, int32_t width, int32_t height) override;
	std::vector<Biome*> getBiomes(std::vector<Biome*> oldBiomeList, int32_t x, int32_t z, int32_t width, int32_t depth) override;
	std::vector<Biome*> getBiomes(std::vector<Biome*> listToReuse, int32_t x, int32_t z, int32_t width, int32_t length, bool cacheFlag) override;
	std::optional<BlockPos> findBiomePosition(int32_t x, int32_t z, int32_t range, std::vector<Biome*> biomes, pcg32& random) override;
	bool areBiomesViable(int32_t x, int32_t z, int32_t radius, std::vector<Biome*> allowed) override;
	bool isFixedBiome() override;
	Biome* getFixedBiome() override;
private:
	Biome* biome;
};
