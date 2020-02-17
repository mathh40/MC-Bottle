#include "WorldGenWaterlily.h"
#include "math/BlockPos.h"

bool WorldGenWaterlily::generate(World* worldIn, pcg32& rand, const BlockPos& position)
{
	for (auto i = 0; i < 10; ++i) 
	{
		auto j = position.getx() + rand(8) - rand(8);
		auto k = position.gety() + rand(4) - rand(4);
		auto l = position.getz() + rand(8) - rand(8);
		BlockPos pos(j, k, l);
		if (worldIn->isAirBlock(pos) && Blocks::WATERLILY.canPlaceBlockAt(worldIn, pos))
		{
			worldIn->setBlockState(pos, Blocks::WATERLILY.getDefaultState(), 2);
		}
	}

	return true;
}
}
