#pragma once
#include "IChunkGenerator.h"

class ChunkGeneratorFlat :public IChunkGenerator
{
public:
	ChunkGeneratorFlat(World* worldIn, int64_t seed, bool generateStructures, std::string_view flatGeneratorSettings);
	Chunk generateChunk(int32_t x, int32_t z) override;
	void populate(int32_t x, int32_t z) override;
	bool generateStructures(Chunk& chunkIn, int32_t x, int32_t z) override;
	std::vector<SpawnListEntry> getPossibleCreatures(EnumCreatureType var1, BlockPos& var2) override;
	std::optional<BlockPos> getNearestStructurePos(World* var1, std::string_view var2, BlockPos& var3, bool var4) override;
	bool isInsideStructure(World* worldIn, std::string_view structureName, BlockPos& pos) override;
	void recreateStructures(Chunk& chunkIn, int32_t x, int32_t z) override;
private:
	World* world;
	pcg32 random;
	std::array<IBlockState*, 256> cachedBlockIDs;
	FlatGeneratorInfo flatWorldGenInfo;
	std::unordered_map<std::string, MapGenStructure*> structureGenerators;
	bool hasDecoration;
	bool hasDungeons;
	WorldGenLakes waterLakeGenerator;
	WorldGenLakes lavaLakeGenerator;
};
