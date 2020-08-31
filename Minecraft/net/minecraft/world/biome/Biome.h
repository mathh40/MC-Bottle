#pragma once
#include "../../../../../pcg-cpp/pcg_random.hpp"
#include "../../block/state/IBlockState.h"
#include "../../util/ResourceLocation.h"
#include "../../util/WeightedRandom.h"
#include "../../util/registry/RegistryNamespaced.h"
#include "../chunk/ChunkPrimer.h"
#include "../gen/feature/WorldGenSwamp.h"
#include "../gen/feature/WorldGenTrees.h"
#include "BiomeDecorator.h"
#include "gen/NoiseGeneratorPerlin.h"
#include "gen/feature/WorldGenAbstractTree.h"
#include "gen/feature/WorldGenBigTree.h"
#include "gen/feature/WorldGenDoublePlant.h"

enum class TempCategory
{
	OCEAN,
	COLD,
	MEDIUM,
	WARM
};

class SpawnListEntry :public WeightedRandom::WeightedItem
{
public:
	Entity* entityClass;
	int32_t minGroupCount;
	int32_t maxGroupCount;

	SpawnListEntry(Entity* entityclassIn, int32_t weight, int32_t groupCountMin, int32_t groupCountMax)
		:WeightedItem(weight)
	{
		entityClass = entityclassIn;
		minGroupCount = groupCountMin;
		maxGroupCount = groupCountMax;
	}

	std::string toString() const
	{
		return entityClass->getSimpleName() + "*(" + minGroupCount + "-" + maxGroupCount + "):" + itemWeight;
	}
};


class BiomeProperties
{
public:
	explicit BiomeProperties(std::string_view nameIn);
protected:
	BiomeProperties& setTemperature(float temperatureIn);
	BiomeProperties& setRainfall(float rainfallIn);
	BiomeProperties& setBaseHeight(float baseHeightIn);
	BiomeProperties& setHeightVariation(float heightVariationIn);
	BiomeProperties& setRainDisabled();
	BiomeProperties& setSnowEnabled();
	BiomeProperties& setWaterColor(int64_t waterColorIn);
	BiomeProperties& setBaseBiome(std::string_view nameIn);
private:
	std::string biomeName;
	float baseHeight = 0.1F;
	float heightVariation = 0.2F;
	float temperature = 0.5F;
	float rainfall = 0.5F;
	int waterColor = 16777215;
	bool enableSnow;
	bool enableRain = true;
	std::optional<std::string> baseBiomeRegName;
};

class Biome
{
public:
	static std::unordered_map<int32_t,Biome*> MUTATION_TO_BASE_ID_MAP;
	static RegistryNamespaced<ResourceLocation, Biome*> REGISTRY;
	IBlockState* topBlock;
	IBlockState* fillerBlock;
	BiomeDecorator decorator;

	static int32_t getIdForBiome(Biome* biome);
	static Biome* getBiomeForId(int32_t id);
	static Biome* getMutationForBiome(Biome* biome);

	bool isMutation() const;
	virtual WorldGenAbstractTree getRandomTreeFeature(pcg32& rand);
	virtual WorldGenerator getRandomWorldGenForGrass(pcg32& rand);
	virtual BlockFlower::EnumFlowerType pickRandomFlower(pcg32& rand, BlockPos& pos);
	virtual int32_t getSkyColorByTemp(float currentTemperature);
	std::vector<SpawnListEntry> getSpawnableList(EnumCreatureType creatureType);
	bool getEnableSnow();
	bool canRain();
	bool isHighHumidity();
	virtual float getSpawningChance();
	float getTemperature(BlockPos& pos);
	virtual void decorate(World* worldIn, pcg32& rand, BlockPos& pos);
	virtual int32_t getGrassColorAtPos(BlockPos& pos);
	virtual int32_t getFoliageColorAtPos(BlockPos& pos);
	virtual void genTerrainBlocks(World* worldIn, pcg32& rand, ChunkPrimer chunkPrimerIn, int32_t x, int32_t z, double noiseVal);
	void generateBiomeTerrain(World* worldIn, pcg32& rand, ChunkPrimer chunkPrimerIn, int32_t x, int32_t z, double noiseVal);
	virtual TempCategory getTempCategory();
	static Biome* getBiome(int32_t id);
	static Biome* getBiome(int32_t biomeId, Biome* fallback);
	virtual bool ignorePlayerSpawnSuitability() const;
	float getBaseHeight() const;
	float getRainfall() const;
	std::string getBiomeName() const;
	float getHeightVariation() const;
	float getDefaultTemperature() const;
	int32_t getWaterColor() const;
	bool isSnowyBiome() const;
	void registerBiomes();

protected:
	static IBlockState* STONE;
	static IBlockState* AIR;
	static IBlockState* BEDROCK;
	static IBlockState* GRAVEL;
	static IBlockState* RED_SANDSTONE;
	static IBlockState* SANDSTONE;
	static IBlockState* ICE;
	static IBlockState* WATER;
	static NoiseGeneratorPerlin TEMPERATURE_NOISE;
	static NoiseGeneratorPerlin GRASS_COLOR_NOISE;
	static WorldGenDoublePlant DOUBLE_PLANT_GENERATOR;
	static WorldGenTrees TREE_FEATURE;
	static WorldGenBigTree BIG_TREE_FEATURE;
	static WorldGenSwamp SWAMP_FEATURE;
	std::vector<SpawnListEntry> spawnableMonsterList;
	std::vector<SpawnListEntry> spawnableCreatureList;
	std::vector<SpawnListEntry> spawnableWaterCreatureList;
	std::vector<SpawnListEntry> spawnableCaveCreatureList;

	explicit Biome(BiomeProperties properties);
	virtual BiomeDecorator createBiomeDecorator();
private:
	static std::shared_ptr<spdlog::logger> LOGGER;
	std::string biomeName;
	float baseHeight;
	float heightVariation;
	float temperature;
	float rainfall;
	int waterColor;
	bool enableSnow;
	bool enableRain;
	std::optional<std::string> baseBiomeRegName;

	void registerBiome(int id, std::string_view name, Biome* biome);
};
