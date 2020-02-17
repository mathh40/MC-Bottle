#include "WorldGenHellLava.h"
#include "World.h"

WorldGenHellLava::WorldGenHellLava(Block* blockIn, bool insideRockIn)
	:block(blockIn), insideRock(insideRockIn)
{
}

bool WorldGenHellLava::generate(World* worldIn, pcg32& rand, const BlockPos& position)
{
	if (worldIn->getBlockState(position.up()).getBlock() != Blocks::NETHERRACK) 
	{
		return false;
	}
	else if (worldIn->getBlockState(position).getMaterial() != Material::AIR && worldIn->getBlockState(position).getBlock() != Blocks::NETHERRACK) 
	{
		return false;
	}
	else 
	{
		auto i = 0;
		if (worldIn->getBlockState(position.west()).getBlock() == Blocks::NETHERRACK) 
		{
			++i;
		}

		if (worldIn->getBlockState(position.east()).getBlock() == Blocks::NETHERRACK) 
		{
			++i;
		}

		if (worldIn->getBlockState(position.north()).getBlock() == Blocks::NETHERRACK) 
		{
			++i;
		}

		if (worldIn->getBlockState(position.south()).getBlock() == Blocks::NETHERRACK) 
		{
			++i;
		}

		if (worldIn->getBlockState(position.down()).getBlock() == Blocks::NETHERRACK) 
		{
			++i;
		}

		auto j = 0;
		if (worldIn->isAirBlock(position.west()))
		{
			++j;
		}

		if (worldIn->isAirBlock(position.east())) 
		{
			++j;
		}

		if (worldIn->isAirBlock(position.north())) 
		{
			++j;
		}

		if (worldIn->isAirBlock(position.south()))
		{
			++j;
		}

		if (worldIn->isAirBlock(position.down())) 
		{
			++j;
		}

		if (!insideRock && i == 4 && j == 1 || i == 5) 
		{
			auto iblockstate = block->getDefaultState();
			worldIn->setBlockState(position, iblockstate, 2);
			worldIn->immediateBlockTick(position, iblockstate, rand);
		}

		return true;
	}
}
