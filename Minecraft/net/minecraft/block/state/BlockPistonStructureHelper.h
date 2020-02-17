#pragma once
#include "../../util/EnumFacing.h"
#include "BlockWorldState.h"

class BlockPistonStructureHelper
{
public:
	BlockPistonStructureHelper(World* worldIn, BlockPos posIn, EnumFacing pistonFacing, bool extending);
	bool canMove();
	std::vector<BlockPos> getBlocksToMove() const;
	std::vector<BlockPos> getBlocksToDestroy() const;
private:
	World* world{};
	BlockPos pistonPos;
	BlockPos blockToMove;
	EnumFacing moveDirection;
	std::vector<BlockPos> toMove{};
	std::vector<BlockPos> toDestroy{};

	bool addBlockLine(BlockPos origin, EnumFacing p_177251_2_);
	void reorderListAtCollision(int32_t p_177255_1_, int32_t p_177255_2_);
	bool addBranchingBlocks(BlockPos fromPos);
};
