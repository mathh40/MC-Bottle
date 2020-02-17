#include "WorldGenSwamp.h"
#include "math/BlockPos.h"
#include "World.h"

IBlockState* WorldGenSwamp::TRUNK = Blocks::LOG.getDefaultState().withProperty(BlockOldLog.VARIANT, BlockPlanks.EnumType.OAK);
IBlockState* WorldGenSwamp::LEAF = Blocks::LEAVES.getDefaultState().withProperty(BlockOldLeaf.VARIANT, BlockPlanks.EnumType.OAK).withProperty(BlockOldLeaf.CHECK_DECAY, false);

WorldGenSwamp::WorldGenSwamp()
	:WorldGenAbstractTree(false)
{
}

bool WorldGenSwamp::generate(World* worldIn, pcg32& rand, const BlockPos& positionIn)
{
	int32_t i = 0;
	BlockPos position = positionIn;
	for (i = rand(4) + 5; worldIn->getBlockState(position.down())->getMaterial() == Material::WATER; position = position.down()) 
	{
	}

	bool flag = true;
	if (position.gety() >= 1 && position.gety() + i + 1 <= 256) 
	{
		for (auto j = position.gety(); j <= position.gety() + 1 + i; ++j) 
		{
			auto k = 1;
			if (j == position.gety()) 
			{
				k = 0;
			}

			if (j >= position.gety() + 1 + i - 2) 
			{
				k = 3;
			}

			MutableBlockPos blockpos$mutableblockpos;

			for (auto i3 = position.getx() - k; i3 <= position.getx() + k && flag; ++i3) 
			{
				for (auto j3 = position.getz() - k; j3 <= position.getz() + k && flag; ++j3) 
				{
					if (j >= 0 && j < 256) 
					{
						auto iblockstate = worldIn->getBlockState(blockpos$mutableblockpos.setPos(i3, j, j3));
						auto block = iblockstate->getBlock();
						if (iblockstate->getMaterial() != Material::AIR && iblockstate->getMaterial() != Material::LEAVES) 
						{
							if (block != Blocks::WATER && block != Blocks::FLOWING_WATER) 
							{
								flag = false;
							}
							else if (j > position.gety()) 
							{
								flag = false;
							}
						}
					}
					else 
					{
						flag = false;
					}
				}
			}
		}

		if (!flag) 
		{
			return false;
		}
		else 
		{
			auto block1 = worldIn->getBlockState(position.down())->getBlock();
			if ((block1 == Blocks::GRASS || block1 == Blocks::DIRT) && position.gety() < 256 - i - 1) 
			{
				setDirtAt(worldIn, position.down());
				for (auto i2 = position.gety() - 3 + i; i2 <= position.gety() + i; ++i2) 
				{
					auto j2 = i2 - (position.gety() + i);
					auto i3 = 2 - j2 / 2;

					for (auto j3 = position.getx() - i3; j3 <= position.getx() + i3; ++j3) 
					{
						auto l3 = j3 - position.getx();

						for (auto j4 = position.getz() - i3; j4 <= position.getz() + i3; ++j4) 
						{
							auto j1 = j4 - position.getz();
							if (MathHelper::abs(l3) != i3 || MathHelper::abs(j1) != i3 || rand(2) != 0 && j2 != 0) 
							{
								BlockPos blockpos4(j3, i2, j4);
								if (!worldIn->getBlockState(blockpos4)->isFullBlock()) 
								{
									setBlockAndNotifyAdequately(worldIn, blockpos4, LEAF);
								}
							}
						}
					}
				}

				for (auto i2 = 0; i2 < i; ++i2) 
				{
					auto iblockstate1 = worldIn->getBlockState(position.up(i2));
					auto block2 = iblockstate1->getBlock();
					if (iblockstate1->getMaterial() == Material::AIR || iblockstate1->getMaterial() == Material::LEAVES || block2 == Blocks::FLOWING_WATER || block2 == Blocks::WATER) 
					{
						setBlockAndNotifyAdequately(worldIn, position.up(i2), TRUNK);
					}
				}

				for (auto i2 = position.gety() - 3 + i; i2 <= position.gety() + i; ++i2) 
				{
					auto j2 = i2 - (position.gety() + i);
					auto i3 = 2 - j2 / 2;
					MutableBlockPos blockpos$mutableblockpos1;

					for (auto l3 = position.getx() - i3; l3 <= position.getx() + i3; ++l3) 
					{
						for (auto j4 = position.getz() - i3; j4 <= position.getz() + i3; ++j4) 
						{
							blockpos$mutableblockpos1.setPos(l3, i2, j4);
							if (worldIn->getBlockState(blockpos$mutableblockpos1)->getMaterial() == Material::LEAVES) 
							{
								auto blockpos3 = blockpos$mutableblockpos1.west();
								auto blockpos4 = blockpos$mutableblockpos1.east();
								auto blockpos1 = blockpos$mutableblockpos1.north();
								auto blockpos2 = blockpos$mutableblockpos1.south();
								if (rand(4) == 0 && worldIn->getBlockState(blockpos3)->getMaterial() == Material::AIR)
								{
									addVine(worldIn, blockpos3, BlockVine::EAST);
								}

								if (rand(4) == 0 && worldIn->getBlockState(blockpos4)->getMaterial() == Material::AIR)
								{
									addVine(worldIn, blockpos4, BlockVine::WEST);
								}

								if (rand(4) == 0 && worldIn->getBlockState(blockpos1)->getMaterial() == Material::AIR)
								{
									addVine(worldIn, blockpos1, BlockVine::SOUTH);
								}

								if (rand(4) == 0 && worldIn->getBlockState(blockpos2)->getMaterial() == Material::AIR)
								{
									addVine(worldIn, blockpos2, BlockVine::NORTH);
								}
							}
						}
					}
				}

				return true;
			}
			else 
			{
				return false;
			}
		}
	}
	else 
	{
		return false;
	}
}

void WorldGenSwamp::addVine(World* worldIn, const BlockPos& pos, PropertyBool* prop)
{
	auto iblockstate = Blocks::VINE.getDefaultState().withProperty(prop, true);
	setBlockAndNotifyAdequately(worldIn, pos, iblockstate);
	auto i = 4;

	for (auto blockpos = pos.down(); worldIn->getBlockState(blockpos)->getMaterial() == Material::AIR && i > 0; --i) 
	{
		setBlockAndNotifyAdequately(worldIn, blockpos, iblockstate);
		blockpos = blockpos.down();
	}
}
