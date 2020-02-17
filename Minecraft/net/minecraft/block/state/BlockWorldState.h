#pragma once
#include "Block.h"

class BlockWorldState
{
public:
	BlockWorldState(World& worldIn, BlockPos& posIn, bool forceLoadIn);
	IBlockState* getBlockState();
	std::optional<TileEntity> getTileEntity();
	BlockPos& getPos();
private:
	World& world;
	BlockPos pos;
	bool forceLoad;
	IBlockState* state;
	std::optional<TileEntity> tileEntity;
	bool tileEntityInitialized;
protected:
};
