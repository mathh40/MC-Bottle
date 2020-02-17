#pragma once
#include "IBlockAccess.h"
#include "../util/multivec.h"
#include "biome/Biome.h"
#include "WorldType.h"
#include "chunk/Chunk.h"

enum class EnumSkyBlock;

class ChunkCache : public IBlockAccess
{
public:
	ChunkCache(World* worldIn, BlockPos& posFromIn, BlockPos& posToIn, int32_t subIn);
	bool isEmpty();
	TileEntity* getTileEntity(BlockPos& pos) override;
	TileEntity* getTileEntity(BlockPos& pos, Chunk::EnumCreateEntityType createType);
	int32_t getCombinedLight(BlockPos& pos, int32_t lightValue);
	IBlockState* getBlockState(BlockPos& pos) override;
	Biome& getBiome(BlockPos& pos) override;
	bool isAirBlock(BlockPos& pos) override;
	int32_t getLightFor(EnumSkyBlock type, BlockPos& pos);
	int32_t getStrongPower(BlockPos& pos, EnumFacing direction);
	WorldType& getWorldType() override;
protected:
	int32_t chunkX;
	int32_t chunkZ;
	multivec<Chunk> chunkArray;
	bool empty;
	World* world;
private:
	int32_t getLightForExt(EnumSkyBlock type, BlockPos& pos);
};
