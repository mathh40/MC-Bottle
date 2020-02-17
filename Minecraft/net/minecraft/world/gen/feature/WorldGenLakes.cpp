#include "WorldGenLakes.h"
#include "World.h"

WorldGenLakes::WorldGenLakes(Block* blockIn)
	:block(blockIn)
{
}

bool WorldGenLakes::generate(World* worldIn, pcg32& rand, const BlockPos& positionIn)
{
	BlockPos position = positionIn;
	for (position = position.add(-8, 0, -8); position.gety() > 5 && worldIn->isAirBlock(position); position = position.down()) 
	{
	}

	if (position.gety() <= 4) 
	{
		return false;
	}
	else 
	{
		position = position.down(4);
		std::array<bool, 2048> aboolean;
		auto i = rand(4) + 4;

		for (auto k2 = 0; k2 < i; ++k2) 
		{
			auto d0 = MathHelper::nextDouble(rand) * 6.0 + 3.0;
			auto d1 = MathHelper::nextDouble(rand) * 4.0 + 2.0;
			auto d2 = MathHelper::nextDouble(rand) * 6.0 + 3.0;
			auto d3 = MathHelper::nextDouble(rand) * (16.0 - d0 - 2.0) + 1.0 + d0 / 2.0;
			auto d4 = MathHelper::nextDouble(rand) * (8.0 - d1 - 4.0) + 2.0 + d1 / 2.0;
			auto d5 = MathHelper::nextDouble(rand) * (16.0 - d2 - 2.0) + 1.0 + d2 / 2.0;

			for (auto l = 1; l < 15; ++l)
			{
				for (auto i1 = 1; i1 < 15; ++i1)
				{
					for (auto j1 = 1; j1 < 7; ++j1)
					{
						auto d6 = ((double)l - d3) / (d0 / 2.0);
						auto d7 = ((double)j1 - d4) / (d1 / 2.0);
						auto d8 = ((double)i1 - d5) / (d2 / 2.0);
						auto d9 = d6 * d6 + d7 * d7 + d8 * d8;
						if (d9 < 1.0) 
						{
							aboolean[(l * 16 + i1) * 8 + j1] = true;
						}
					}
				}
			}
		}

		for (auto k2 = 0; k2 < 16; ++k2) 
		{
			for (auto l3 = 0; l3 < 16; ++l3)
			{
				for (auto k4 = 0; k4 < 8; ++k4)
				{
					auto flag1 = !aboolean[(k2 * 16 + l3) * 8 + k4] && (k2 < 15 && aboolean[((k2 + 1) * 16 + l3) * 8 + k4] || k2 > 0 && aboolean[((k2 - 1) * 16 + l3) * 8 + k4] || l3 < 15 && aboolean[(k2 * 16 + l3 + 1) * 8 + k4] || l3 > 0 && aboolean[(k2 * 16 + (l3 - 1)) * 8 + k4] || k4 < 7 && aboolean[(k2 * 16 + l3) * 8 + k4 + 1] || k4 > 0 && aboolean[(k2 * 16 + l3) * 8 + (k4 - 1)]);
					if (flag1) 
					{
						auto material = worldIn->getBlockState(position.add(k2, k4, l3))->getMaterial();
						if (k4 >= 4 && material.isLiquid()) 
						{
							return false;
						}

						if (k4 < 4 && !material.isSolid() && worldIn->getBlockState(position.add(k2, k4, l3))->getBlock() != block) 
						{
							return false;
						}
					}
				}
			}
		}

		for (auto k2 = 0; k2 < 16; ++k2) 
		{
			for (auto l3 = 0; l3 < 16; ++l3)
			{
				for (auto k4 = 0; k4 < 8; ++k4)
				{
					if (aboolean[(k2 * 16 + l3) * 8 + k4]) 
					{
						worldIn->setBlockState(position.add(k2, k4, l3), k4 >= 4 ? Blocks::AIR.getDefaultState() : block->getDefaultState(), 2);
					}
				}
			}
		}

		for (auto k2 = 0; k2 < 16; ++k2) 
		{
			for (auto l3 = 0; l3 < 16; ++l3) 
			{
				for (auto k4 = 4; k4 < 8; ++k4) 
				{
					if (aboolean[(k2 * 16 + l3) * 8 + k4]) 
					{
						auto blockpos = position.add(k2, k4 - 1, l3);
						if (worldIn->getBlockState(blockpos).getBlock() == Blocks::DIRT && worldIn->getLightFor(EnumSkyBlock::SKY, position.add(k2, k4, l3)) > 0) 
						{
							auto biome = worldIn->getBiome(blockpos);
							if (biome->topBlock.getBlock() == Blocks::MYCELIUM) 
							{
								worldIn->setBlockState(blockpos, Blocks::MYCELIUM.getDefaultState(), 2);
							}
							else 
							{
								worldIn->setBlockState(blockpos, Blocks::GRASS.getDefaultState(), 2);
							}
						}
					}
				}
			}
		}

		if (block->getDefaultState()->getMaterial() == Material::LAVA) 
		{
			for (auto k2 = 0; k2 < 16; ++k2) 
			{
				for (auto l3 = 0; l3 < 16; ++l3) 
				{
					for (auto k4 = 0; k4 < 8; ++k4)
					{
						auto flag1 = !aboolean[(k2 * 16 + l3) * 8 + k4] && (k2 < 15 && aboolean[((k2 + 1) * 16 + l3) * 8 + k4] || k2 > 0 && aboolean[((k2 - 1) * 16 + l3) * 8 + k4] || l3 < 15 && aboolean[(k2 * 16 + l3 + 1) * 8 + k4] || l3 > 0 && aboolean[(k2 * 16 + (l3 - 1)) * 8 + k4] || k4 < 7 && aboolean[(k2 * 16 + l3) * 8 + k4 + 1] || k4 > 0 && aboolean[(k2 * 16 + l3) * 8 + (k4 - 1)]);
						if (flag1 && (k4 < 4 || rand(2) != 0) && worldIn->getBlockState(position.add(k2, k4, l3))->getMaterial().isSolid()) 
						{
							worldIn->setBlockState(position.add(k2, k4, l3), Blocks::STONE.getDefaultState(), 2);
						}
					}
				}
			}
		}

		if (block->getDefaultState()->getMaterial() == Material::WATER) 
		{
			for (auto k2 = 0; k2 < 16; ++k2) 
			{
				for (auto l3 = 0; l3 < 16; ++l3) 
				{
					int l4 = true;
					if (worldIn->canBlockFreezeWater(position.add(k2, 4, l3))) {
						worldIn->setBlockState(position.add(k2, 4, l3), Blocks::ICE.getDefaultState(), 2);
					}
				}
			}
		}

		return true;
	}
}
