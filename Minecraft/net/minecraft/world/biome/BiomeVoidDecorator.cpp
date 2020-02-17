#include "BiomeVoidDecorator.h"

void BiomeVoidDecorator::decorate(World* worldIn, pcg32& random, Biome* biome, BlockPos& pos)
{
	BlockPos blockpos = worldIn->getSpawnPoint();
	auto i = true;
	auto d0 = blockpos.distanceSq(pos.add(8, blockpos.gety(), 8));
	if (d0 <= 1024.0) 
	{
		BlockPos blockpos1(blockpos.getx() - 16, blockpos.gety() - 1, blockpos.getz() - 16);
		BlockPos blockpos2(blockpos.getx() + 16, blockpos.gety() - 1, blockpos.getz() + 16);
		auto blockpos$mutableblockpos = MutableBlockPos(blockpos1);

		for (int j = pos.getz(); j < pos.getz() + 16; ++j) 
		{
			for (int k = pos.getx(); k < pos.getx() + 16; ++k) 
			{
				if (j >= blockpos1.getz() && j <= blockpos2.getz() && k >= blockpos1.getx() && k <= blockpos2.getx()) 
				{
					blockpos$mutableblockpos.setPos(k, blockpos$mutableblockpos.gety(), j);
					if (blockpos.getx() == k && blockpos.getz() == j) 
					{
						worldIn->setBlockState(blockpos$mutableblockpos, Blocks.COBBLESTONE.getDefaultState(), 2);
					}
					else {
						worldIn->setBlockState(blockpos$mutableblockpos, Blocks.STONE.getDefaultState(), 2);
					}
				}
			}
		}
	}
}
