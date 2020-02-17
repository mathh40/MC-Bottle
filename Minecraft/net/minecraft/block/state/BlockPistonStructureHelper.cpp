#include "BlockPistonStructureHelper.h"
#include "material/Material.h"

BlockPistonStructureHelper::BlockPistonStructureHelper(World* worldIn, BlockPos posIn, EnumFacing pistonFacing,
                                                       bool extending)
	:world(worldIn), pistonPos(posIn), moveDirection(extending ? pistonFacing : pistonFacing.getOpposite())
	 , blockToMove(extending ? posIn.offset(pistonFacing) : posIn.offset(pistonFacing, 2))
{
}

bool BlockPistonStructureHelper::canMove()
{
	toMove.clear();
	toDestroy.clear();
	auto iblockstate = world->getBlockState(blockToMove);
	if (!BlockPistonBase.canPush(iblockstate, world, blockToMove, moveDirection, false, moveDirection)) {
		if (iblockstate.getPushReaction() == EnumPushReaction::DESTROY) 
		{
			toDestroy.emplace_back(blockToMove);
			return true;
		}
		else {
			return false;
		}
	}
	else if (!addBlockLine(blockToMove, moveDirection)) {
		return false;
	}
	else {
		for (int i = 0; i < toMove.size(); ++i) {
			BlockPos blockpos = toMove[i];
			if (world->getBlockState(blockpos).getBlock() == Blocks::SLIME_BLOCK && !addBranchingBlocks(blockpos)) 
			{
				return false;
			}
		}

		return true;
	}
}

std::vector<BlockPos> BlockPistonStructureHelper::getBlocksToMove() const
{
	return toMove;
}

std::vector<BlockPos> BlockPistonStructureHelper::getBlocksToDestroy() const
{
	return toDestroy;
}

bool BlockPistonStructureHelper::addBlockLine(BlockPos origin, EnumFacing p_177251_2_)
{
	auto iblockstate = world->getBlockState(origin);
	auto block = iblockstate.getBlock();
	if (iblockstate.getMaterial() == Material::AIR) 
	{
		return true;
	}
	else if (!BlockPistonBase.canPush(iblockstate, world, origin, moveDirection, false, p_177251_2_)) 
	{
		return true;
	}
	else if (origin == pistonPos)
	{
		return true;
	}
	else if (std::find(toMove.begin(), toMove.end(), origin) != toMove.end())
	{
		return true;
	}
	else {
		int i = 1;
		if (i + toMove.size() > 12) 
		{
			return false;
		}
		else {
			while (block == Blocks::SLIME_BLOCK) 
			{
				BlockPos blockpos = origin.offset(moveDirection.getOpposite(), i);
				iblockstate = world->getBlockState(blockpos);
				block = iblockstate.getBlock();
				if (iblockstate.getMaterial() == Material::AIR || !BlockPistonBase.canPush(iblockstate, world, blockpos, moveDirection, false, moveDirection.getOpposite()) || blockpos == pistonPos)
				{
					break;
				}

				++i;
				if (i + toMove.size() > 12) 
				{
					return false;
				}
			}

			int i1 = 0;

			int j1;
			for (j1 = i - 1; j1 >= 0; --j1) {
				toMove.emplace_back(origin.offset(moveDirection.getOpposite(), j1));
				++i1;
			}

			j1 = 1;

			while (true) {
				auto blockpos1 = origin.offset(moveDirection, j1);
				int k = toMove.indexOf(blockpos1);
				if (k > -1)
				{
					reorderListAtCollision(i1, k);

					for (int l = 0; l <= k + i1; ++l) {
						auto blockpos2 = toMove[1];
						if (world->getBlockState(blockpos2).getBlock() == Blocks::SLIME_BLOCK && !addBranchingBlocks(blockpos2)) 
						{
							return false;
						}
					}

					return true;
				}

				iblockstate = world->getBlockState(blockpos1);
				if (iblockstate.getMaterial() == Material::AIR) 
				{
					return true;
				}

				if (!BlockPistonBase.canPush(iblockstate, world, blockpos1, moveDirection, true, moveDirection) || blockpos1 == pistonPos)
				{
					return false;
				}

				if (iblockstate.getPushReaction() == EnumPushReaction::DESTROY) {
					toDestroy.emplace_back(blockpos1);
					return true;
				}

				if (toMove.size() >= 12) 
				{
					return false;
				}

				toMove.emplace_back(blockpos1);
				++i1;
				++j1;
			}
		}
	}
}

void BlockPistonStructureHelper::reorderListAtCollision(int32_t p_177255_1_, int32_t p_177255_2_)
{
	std::vector<BlockPos> list;
	std::vector<BlockPos> list1;
	std::vector<BlockPos> list2;
	list.assign(toMove.begin(), toMove.begin() + p_177255_2_);
	list1.assign(toMove.end() - p_177255_1_, toMove.end());
	list2.assign(toMove.begin() + p_177255_2_, toMove.end() - p_177255_1_);
	toMove.clear();
	toMove.assign(list.begin(), list.end());
	toMove.assign(list1.begin(), list1.end());
	toMove.assign(list2.begin(), list2.end());
}

bool BlockPistonStructureHelper::addBranchingBlocks(BlockPos fromPos)
{
	auto var2 = EnumFacing::values();

	for (auto enumfacing : var2) 
	{
		if (enumfacing.getAxis() != moveDirection.getAxis() && !addBlockLine(fromPos.offset(enumfacing), enumfacing)) 
		{
			return false;
		}
	}

	return true;
}
