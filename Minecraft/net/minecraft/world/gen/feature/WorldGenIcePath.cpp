#include "WorldGenIcePath.h"
#include "World.h"

WorldGenIcePath::WorldGenIcePath(int32_t basePathWidthIn)
	:block(Blocks::PACKED_ICE), basePathWidth(basePathWidthIn)
{

}

bool WorldGenIcePath::generate(World* worldIn, pcg32& rand, const BlockPos& positionIn)
{
	BlockPos position;
	while (worldIn->isAirBlock(position) && position.gety() > 2) 
	{
		position = positionIn.down();
	}

	if (worldIn->getBlockState(position)->getBlock() != Blocks::SNOW) 
	{
		return false;
	}
	else 
	{
		auto i = rand(basePathWidth - 2) + 2;

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
						auto block = worldIn->getBlockState(blockpos)->getBlock();
						if (block == Blocks::DIRT || block == Blocks::SNOW || block == Blocks::ICE) 
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
