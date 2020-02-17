#include "WorldGenClay.h"
#include "World.h"

WorldGenClay::WorldGenClay(int32_t p_i2011_1_)
	: block(Blocks::CLAY), numberOfBlocks(p_i2011_1_)
{
}

bool WorldGenClay::generate(World* worldIn, pcg32& rand, const BlockPos& position)
{
	if (worldIn->getBlockState(position).getMaterial() != Material::WATER) 
	{
		return false;
	}
	else 
	{
		auto i = rand(numberOfBlocks - 2) + 2;

		for (auto k = position.getx() - i; k <= position.getx() + i; ++k)
		{
			for (auto l = position.getz() - i; l <= position.getz() + i; ++l)
			{
				auto i1 = k - position.getx();
				auto j1 = l - position.getz();
				if (i1 * i1 + j1 * j1 <= i * i) 
				{
					for (auto k1 = position.gety() - 1; k1 <= position.gety() + 1; ++k1)
					{
						BlockPos blockpos(k, k1, l);
						auto block = worldIn->getBlockState(blockpos).getBlock();
						if (block == Blocks::DIRT || block == Blocks::CLAY) 
						{
							worldIn->setBlockState(blockpos, block.getDefaultState(), 2);
						}
					}
				}
			}
		}

		return true;
	}
}
