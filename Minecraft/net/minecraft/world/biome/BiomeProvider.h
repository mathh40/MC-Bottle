#pragma once
#include "BiomeCache.h"

class WorldInfo;
class WorldType;

class BiomeProvider
{
public:
	explicit BiomeProvider(WorldInfo info);
	std::vector<Biome*> getBiomesToSpawnIn() const;
	virtual Biome* getBiome(BlockPos& pos) const;
	virtual Biome* getBiome(BlockPos& pos, Biome* defaultBiome) const;
	float getTemperatureAtHeight(float p_76939_1_, int32_t p_76939_2_);
	virtual std::vector<Biome*> getBiomesForGeneration(std::vector<Biome*> biomes, int32_t x, int32_t z, int32_t width, int32_t height);
	virtual std::vector<Biome*> getBiomes(std::vector<Biome*> oldBiomeList, int32_t x, int32_t z, int32_t width, int32_t depth);
	virtual std::vector<Biome*> getBiomes(std::vector<Biome*> listToReuse, int32_t x, int32_t z, int32_t width, int32_t length, bool cacheFlag);
	virtual bool areBiomesViable(int32_t x, int32_t z, int32_t radius, std::vector<Biome*> allowed);
	virtual std::optional<BlockPos> findBiomePosition(int32_t x, int32_t z, int32_t range, std::vector<Biome*> biomes, pcg32& random);
	void cleanupCache();
	virtual bool isFixedBiome();
	virtual Biome* getFixedBiome();
protected:
	BiomeProvider();
private:
	ChunkGeneratorSettings settings;
	GenLayer genBiomes;
	GenLayer biomeIndexLayer;
	BiomeCache biomeCache;
	std::vector<Biome*> biomesToSpawnIn;

	BiomeProvider(int64_t seed, WorldType worldTypeIn, std::string_view options);
};
