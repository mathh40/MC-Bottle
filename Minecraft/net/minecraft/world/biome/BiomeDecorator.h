#pragma once
#include "../../../../../pcg-cpp/pcg_random.hpp"
#include "../../util/math/BlockPos.h"
#include "../gen/ChunkGeneratorSettings.h"

class Biome;
class World;
class WorldGenerator;
class WorldGenFlowers;

class BiomeDecorator
{
public:
	bool generateFalls;
	BiomeDecorator();
	virtual void decorate(World* worldIn, pcg32& random, Biome* biome, BlockPos& pos);
protected:
	bool decorating;
	BlockPos chunkPos;
	ChunkGeneratorSettings chunkProviderSettings;
	WorldGenerator* clayGen;
	WorldGenerator* sandGen;
	WorldGenerator* gravelGen;
	WorldGenerator* dirtGen;
	WorldGenerator* gravelOreGen;
	WorldGenerator* graniteGen;
	WorldGenerator* dioriteGen;
	WorldGenerator* andesiteGen;
	WorldGenerator* coalGen;
	WorldGenerator* ironGen;
	WorldGenerator* goldGen;
	WorldGenerator* redstoneGen;
	WorldGenerator* diamondGen;
	WorldGenerator* lapisGen;
	WorldGenFlowers* flowerGen;
	WorldGenerator* mushroomBrownGen;
	WorldGenerator* mushroomRedGen;
	WorldGenerator* bigMushroomGen;
	WorldGenerator* reedGen;
	WorldGenerator* cactusGen;
	WorldGenerator* waterlilyGen;
	int32_t waterlilyPerChunk;
	int32_t treesPerChunk;
	float extraTreeChance;
	int32_t flowersPerChunk;
	int32_t grassPerChunk;
	int32_t deadBushPerChunk;
	int32_t mushroomsPerChunk;
	int32_t reedsPerChunk;
	int32_t cactiPerChunk;
	int32_t gravelPatchesPerChunk;
	int32_t sandPatchesPerChunk;
	int32_t clayPerChunk;
	int32_t bigMushroomsPerChunk;

	virtual void genDecorations(Biome* biomeIn, World* worldIn, pcg32& random);
	virtual void generateOres(World* worldIn, pcg32& random);
	virtual void genStandardOre1(World* worldIn, pcg32& random, int32_t blockCount, WorldGenerator* generator, int32_t minHeight, int32_t maxHeight);
	virtual void genStandardOre2(World* worldIn, pcg32& random, int32_t blockCount, WorldGenerator* generator, int32_t centerHeight, int32_t spread);
private:
};
