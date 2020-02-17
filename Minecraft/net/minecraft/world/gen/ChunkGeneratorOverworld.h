#pragma once
#include "IChunkGenerator.h"
#include "WorldType.h"
#include "NoiseGeneratorOctaves.h"
#include "NoiseGeneratorPerlin.h"
#include "ChunkGeneratorSettings.h"
#include "MapGenBase.h"

class ChunkPrimer;

class ChunkGeneratorOverworld :public IChunkGenerator
{
public:
	NoiseGeneratorOctaves* scaleNoise;
	NoiseGeneratorOctaves* depthNoise;
	NoiseGeneratorOctaves* forestNoise;

	ChunkGeneratorOverworld(World* worldIn, int64_t seed, bool mapFeaturesEnabledIn, std::string_view generatorOptions);
	void setBlocksInChunk(int32_t x, int32_t z, ChunkPrimer& primer);
	void replaceBiomeBlocks(int32_t x, int32_t z, ChunkPrimer& primer, std::vector<Biome*> biomesIn);
	Chunk generateChunk(int32_t x, int32_t z) override;
	void populate(int32_t x, int32_t z) override;
	bool generateStructures(Chunk& chunkIn, int32_t x, int32_t z) override;
	std::vector<SpawnListEntry> getPossibleCreatures(EnumCreatureType creatureType, BlockPos& pos) override;
	bool isInsideStructure(World* worldIn, std::string_view structureName, BlockPos& pos) override;
	std::optional<BlockPos> getNearestStructurePos(World* worldIn, std::string_view structureName, BlockPos& position, bool findUnexplored) override;
	void recreateStructures(Chunk& chunkIn, int32_t x, int32_t z) override;
protected:
	static IBlockState* STONE;
private:
	pcg32 rand;
	NoiseGeneratorOctaves* minLimitPerlinNoise;
	NoiseGeneratorOctaves* maxLimitPerlinNoise;
	NoiseGeneratorOctaves* mainPerlinNoise;
	NoiseGeneratorPerlin* surfaceNoise;

	World* world;
	bool mapFeaturesEnabled;
	WorldType terrainType;
	std::array<double, 825> heightMap;
	std::array<float, 25> biomeWeights;
	ChunkGeneratorSettings settings;
	IBlockState* oceanBlock;
	std::array<double, 256> depthBuffer;

	MapGenBase caveGenerator;
	MapGenStronghold strongholdGenerator;
	MapGenVillage villageGenerator;
	MapGenMineshaft mineshaftGenerator;
	MapGenScatteredFeature scatteredFeatureGenerator;
	MapGenBase ravineGenerator;
	StructureOceanMonument oceanMonumentGenerator;
	WoodlandMansion woodlandMansionGenerator;
	std::vector<Biome*> biomesForGeneration;
	std::vector<double> mainNoiseRegion;
	std::vector<double>minLimitRegion;
	std::vector<double> maxLimitRegion;
	std::vector<double> depthRegion;

	void generateHeightmap(int32_t x, int32_t y, int32_t z);
};
