#pragma once
#include "IChunkGenerator.h"
#include "NoiseGeneratorOctaves.h"
#include "NoiseGeneratorSimplex.h"
#include "structure/MapGenEndCity.h"
#include "feature/WorldGenEndIsland.h"

class ChunkPrimer;

class ChunkGeneratorEnd :public IChunkGenerator
{
public:
	NoiseGeneratorOctaves* noiseGen5;
	NoiseGeneratorOctaves* noiseGen6;

	ChunkGeneratorEnd(World* p_i47241_1_, bool p_i47241_2_, int64_t p_i47241_3_, BlockPos& p_i47241_5_);
	void setBlocksInChunk(int32_t x, int32_t z, ChunkPrimer& primer);
	void buildSurfaces(ChunkPrimer& primer);
	Chunk generateChunk(int32_t x, int32_t z) override;
	bool isIslandChunk(int32_t p_185961_1_, int32_t p_185961_2_);
	void populate(int32_t x, int32_t z) override;
	bool generateStructures(Chunk& chunkIn, int32_t x, int32_t z) override;
	std::vector<SpawnListEntry> getPossibleCreatures(Chunk::EnumCreatureType creatureType, BlockPos& pos) override;
	std::optional<BlockPos> getNearestStructurePos(World* var1, std::string_view var2, BlockPos& var3, bool var4) override;
	bool isInsideStructure(World* worldIn, std::string_view structureName, BlockPos& pos) override;
	void recreateStructures(Chunk& chunkIn, int32_t x, int32_t z) override;
protected:
	static IBlockState* END_STONE;
	static IBlockState* AIR;
private:
	pcg32 rand;
	NoiseGeneratorOctaves* lperlinNoise1;
	NoiseGeneratorOctaves* lperlinNoise2;
	NoiseGeneratorOctaves* perlinNoise1;
	World* world;
	bool mapFeaturesEnabled;
	BlockPos spawnPoint;
	MapGenEndCity endCityGen;
	NoiseGeneratorSimplex* islandNoise;
	std::vector<double> buffer;
	std::vector<Biome*> biomesForGeneration;
	std::vector<double> pnr;
	std::vector<double> ar;
	std::vector<double> br;
	WorldGenEndIsland endIslands;

	float getIslandHeightValue(int32_t p_185960_1_, int32_t p_185960_2_, int32_t p_185960_3_, int32_t p_185960_4_);
	std::vector<double> getHeights(std::vector<double>& p_185963_1_, int32_t p_185963_2_, int32_t p_185963_3_, int32_t p_185963_4_, int32_t p_185963_5_, int32_t p_185963_6_, int32_t p_185963_7_);
};
