#include "WorldGenTallGrass.h"
#include "World.h"

WorldGenTallGrass::WorldGenTallGrass(BlockTallGrass::EnumType p_i45629_1_)
	:tallGrassState(Blocks::TALLGRASS.getDefaultState().withProperty(BlockTallGrass::TYPE, p_i45629_1_))
{
}

bool WorldGenTallGrass::generate(World* worldIn, pcg32& rand, const BlockPos& positionIn)
{
	BlockPos position = positionIn;
	for (IBlockState* iblockstate = worldIn->getBlockState(position); (iblockstate->getMaterial() == Material::AIR ||
		     iblockstate->getMaterial() == Material::LEAVES) && position.gety() > 0; iblockstate = worldIn->getBlockState(position)) 
	{
		position = position.down();
	}

	for (auto i = 0; i < 128; ++i) 
	{
		BlockPos blockpos = position.add((int32_t)(rand(8) - rand(8)), rand(4) - rand(4), rand(8) - rand(8));
		if (worldIn->isAirBlock(blockpos) && Blocks::TALLGRASS.canBlockStay(worldIn, blockpos, tallGrassState)) 
		{
			worldIn->setBlockState(blockpos, tallGrassState, 2);
		}
	}

	return true;
}
