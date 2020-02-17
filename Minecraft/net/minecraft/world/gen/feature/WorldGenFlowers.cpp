#include "WorldGenFlowers.h"
#include "math/BlockPos.h"
#include "World.h"
#include "WorldProvider.h"

WorldGenFlowers::WorldGenFlowers(BlockFlower* flowerIn, BlockFlower::EnumFlowerType type)
{
	setGeneratedBlock(flowerIn, type);
}

void WorldGenFlowers::setGeneratedBlock(BlockFlower* flowerIn, BlockFlower::EnumFlowerType typeIn)
{
	flower = flowerIn;
	state = flowerIn->getDefaultState().withProperty(flowerIn->getTypeProperty(), typeIn);
}

bool WorldGenFlowers::generate(World* worldIn, pcg32& rand, const BlockPos& position)
{
	for (auto i = 0; i < 64; ++i) 
	{
		BlockPos blockpos = position.add((int32_t)(rand(8) - rand(8)), rand(4) - rand(4), rand(8) - rand(8));
		if (worldIn->isAirBlock(blockpos) && (!worldIn->provider->isNether() || blockpos.gety() < 255) && flower->canBlockStay(worldIn, blockpos, state)) 
		{
			worldIn->setBlockState(blockpos, state, 2);
		}
	}

	return true;
}
