#pragma once
#include "IChunkGenerator.h"
#include "state/IBlockState.h"
#include "biome/Biome.h"

class ChunkGeneratorDebug :public IChunkGenerator
{
public:
	ChunkGeneratorDebug(World* worldIn);
	Chunk generateChunk(int32_t x, int32_t z) override;
	IBlockState* getBlockStateFor(int32_t p_177461_0_, int32_t p_177461_1_) const;
	void populate(int32_t x, int32_t z) override;
	bool generateStructures(Chunk& chunkIn, int32_t x, int32_t z) override;
	std::vector<SpawnListEntry> getPossibleCreatures(EnumCreatureType creatureType, BlockPos& pos) override;
	std::optional<BlockPos> getNearestStructurePos(World* worldIn, std::string_view structureName, BlockPos& position, bool findUnexplored) override;
	bool isInsideStructure(World* worldIn, std::string_view structureName, BlockPos& pos) override;
	void recreateStructures(Chunk& chunkIn, int32_t x, int32_t z) override;
protected:
	static IBlockState* AIR;
	static IBlockState* BARRIER;
private:
	static std::vector<IBlockState*> ALL_VALID_STATES;
	static int32_t GRID_WIDTH;
	static int32_t GRID_HEIGHT;
	World* world;
};
