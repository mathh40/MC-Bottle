#include "WorldGenTaiga2.h"
#include "material/Material.h"
#include "math/BlockPos.h"
#include "math/MathHelper.h"
#include "World.h"

IBlockState* WorldGenTaiga2::TRUNK = Blocks::LOG.getDefaultState().withProperty(BlockOldLog.VARIANT, BlockPlanks.EnumType.SPRUCE);
IBlockState* WorldGenTaiga2::LEAF = Blocks::LEAVES.getDefaultState().withProperty(BlockOldLeaf.VARIANT, BlockPlanks.EnumType.SPRUCE).withProperty(BlockLeaves.CHECK_DECAY, false);

WorldGenTaiga2::WorldGenTaiga2(bool notif)
	:WorldGenAbstractTree(notif)
{
}

bool WorldGenTaiga2::generate(World* worldIn, pcg32& rand, const BlockPos& position)
{
	auto i = rand(4) + 6;
	auto j = 1 + rand(2);
	auto k = i - j;
	auto l = 2 + rand(2);
	auto flag = true;
	if (position.gety() >= 1 && position.gety() + i + 1 <= 256) 
	{
		for (auto i1 = position.gety(); i1 <= position.gety() + 1 + i && flag; ++i1)
		{
			auto i3 = 0;
			if (i1 - position.gety() < j) 
			{
				i3 = 0;
			}
			else {
				i3 = l;
			}

			MutableBlockPos blockpos$mutableblockpos;

			for (auto k1 = position.getx() - i3; k1 <= position.getx() + i3 && flag; ++k1) 
			{
				for (auto l1 = position.getz() - i3; l1 <= position.getz() + i3 && flag; ++l1) 
				{
					if (i1 >= 0 && i1 < 256) 
					{
						Material material = worldIn->getBlockState(blockpos$mutableblockpos.setPos(k1, i1, l1)).getMaterial();
						if (material != Material::AIR && material != Material::LEAVES) 
						{
							flag = false;
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
			auto block = worldIn->getBlockState(position.down()).getBlock();
			if ((block == Blocks::GRASS || block == Blocks::DIRT || block == Blocks::FARMLAND) && position.gety() < 256 - i - 1) 
			{
				setDirtAt(worldIn, position.down());
				auto i3 = rand(2);
				auto j3 = 1;
				auto k3 = 0;
				for (auto l1 = 0; l1 <= k; ++l1) 
				{
					auto j4 = position.gety() + i - l1;

					for (auto i2 = position.getx() - i3; i2 <= position.getx() + i3; ++i2)
					{
						auto j2 = i2 - position.getx();

						for (auto k2 = position.getz() - i3; k2 <= position.getz() + i3; ++k2)
						{
							auto l2 = k2 - position.getz();
							if (MathHelper::abs(j2) != i3 || MathHelper::abs(l2) != i3 || i3 <= 0) 
							{
								BlockPos blockpos(i2, j4, k2);
								if (!worldIn->getBlockState(blockpos).isFullBlock()) 
								{
									setBlockAndNotifyAdequately(worldIn, blockpos, LEAF);
								}
							}
						}
					}

					if (i3 >= j3) 
					{
						i3 = k3;
						k3 = 1;
						++j3;
						if (j3 > l) 
						{
							j3 = l;
						}
					}
					else 
					{
						++i3;
					}
				}

				auto l1 = rand(3);

				for (auto j4 = 0; j4 < i - l1; ++j4) 
				{
					Material material1 = worldIn->getBlockState(position.up(j4)).getMaterial();
					if (material1 == Material::AIR || material1 == Material::LEAVES) 
					{
						setBlockAndNotifyAdequately(worldIn, position.up(j4), TRUNK);
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
