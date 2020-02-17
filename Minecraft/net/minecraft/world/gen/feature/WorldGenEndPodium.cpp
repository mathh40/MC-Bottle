#include "WorldGenEndPodium.h"
#include "math/BlockPos.h"

BlockPos WorldGenEndPodium::END_PODIUM_LOCATION = BlockPos::ORIGIN;
BlockPos WorldGenEndPodium::END_PODIUM_CHUNK_POS = BlockPos(END_PODIUM_LOCATION.getx() - 4 & -16, 0, END_PODIUM_LOCATION.getz() - 4 & -16);

WorldGenEndPodium::WorldGenEndPodium(bool activePortalIn)
	: activePortal(activePortalIn)
{
}

bool WorldGenEndPodium::generate(World* worldIn, pcg32& rand, const BlockPos& position)
{
	for(auto blockpos$mutableblockpos : getAllInBoxMutable(BlockPos(position.getx() - 4, position.gety() - 1, position.getz() - 4), BlockPos(position.getx() + 4, position.gety() + 32, position.getz() + 4)))
	{
		auto d0 = blockpos$mutableblockpos.getDistance(position.getx(), blockpos$mutableblockpos.gety(), position.getz());
		if (d0 <= 3.5) 
		{
			if (blockpos$mutableblockpos.gety() < position.gety()) 
			{
				if (d0 <= 2.5) 
				{
					setBlockAndNotifyAdequately(worldIn, blockpos$mutableblockpos, Blocks.BEDROCK.getDefaultState());
				}
				else if (blockpos$mutableblockpos.gety() < position.gety()) 
				{
					setBlockAndNotifyAdequately(worldIn, blockpos$mutableblockpos, Blocks.END_STONE.getDefaultState());
				}
			}
			else if (blockpos$mutableblockpos.gety() > position.gety()) 
			{
				setBlockAndNotifyAdequately(worldIn, blockpos$mutableblockpos, Blocks.AIR.getDefaultState());
			}
			else if (d0 > 2.5) 
			{
				setBlockAndNotifyAdequately(worldIn, blockpos$mutableblockpos, Blocks.BEDROCK.getDefaultState());
			}
			else if (activePortal) 
			{
				setBlockAndNotifyAdequately(worldIn, BlockPos(blockpos$mutableblockpos), Blocks.END_PORTAL.getDefaultState());
			}
			else 
			{
				setBlockAndNotifyAdequately(worldIn, BlockPos(blockpos$mutableblockpos), Blocks.AIR.getDefaultState());
			}
		}
	}

	for (auto i = 0; i < 4; ++i) 
	{
		setBlockAndNotifyAdequately(worldIn, position.up(i), Blocks.BEDROCK.getDefaultState());
	}

	BlockPos blockpos = position.up(2);

	for(auto enumfacing : EnumFacing.Plane.HORIZONTAL)
	{
		setBlockAndNotifyAdequately(worldIn, blockpos.offset(enumfacing), Blocks.TORCH.getDefaultState().withProperty(BlockTorch.FACING, enumfacing));
	}

	return true;
}
