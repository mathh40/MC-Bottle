#include "WorldGenFire.h"
#include "math/BlockPos.h"
#include "World.h"

bool WorldGenFire::generate(World* worldIn, pcg32& rand, const BlockPos& position)
{
	for (auto i = 0; i < 64; ++i) 
	{
		BlockPos blockpos = position.add((int32_t)(rand(8) - rand(8)), rand(4) - rand(4), rand(8) - rand(8));
		if (worldIn->isAirBlock(blockpos) && worldIn->getBlockState(blockpos.down()).getBlock() == Blocks::NETHERRACK) 
		{
			worldIn->setBlockState(blockpos, Blocks::FIRE.getDefaultState(), 2);
		}
	}

	return true;
}
