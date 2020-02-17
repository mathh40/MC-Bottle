#include "WorldGenSand.h"
#include "World.h"

WorldGenSand::WorldGenSand(Block* blockIn, int32_t radiusIn)
	:block(blockIn), radius(radiusIn)
{
}

bool WorldGenSand::generate(World* worldIn, pcg32& rand, const BlockPos& position)
{
	if (worldIn->getBlockState(position)->getMaterial() != Material::WATER) 
	{
		return false;
	}
	else 
	{
		auto i = rand(radius - 2) + 2;

		for (auto k = position.getx() - i; k <= position.getx() + i; ++k) 
		{
			for (auto l = position.getz() - i; l <= position.getz() + i; ++l) 
			{
				auto i1 = k - position.getx();
				auto j1 = l - position.getz();
				if (i1 * i1 + j1 * j1 <= i * i) 
				{
					for (auto k1 = position.gety() - 2; k1 <= position.gety() + 2; ++k1) 
					{
						BlockPos blockpos(k, k1, l);
						auto block = worldIn->getBlockState(blockpos)->getBlock();
						if (block == Blocks::DIRT || block == Blocks::GRASS) 
						{
							worldIn->setBlockState(blockpos, block->getDefaultState(), 2);
						}
					}
				}
			}
		}

		return true;
	}
}
