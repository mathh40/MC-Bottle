#include "WorldGenBirchTree.h"
#include "math/BlockPos.h"
#include "World.h"

IBlockState* WorldGenBirchTree::LOG = Blocks.LOG.getDefaultState().withProperty(BlockOldLog.VARIANT, BlockPlanks.EnumType.BIRCH);
IBlockState* WorldGenBirchTree::LEAF = Blocks.LEAVES.getDefaultState().withProperty(BlockOldLeaf.VARIANT, BlockPlanks.EnumType.BIRCH).withProperty(BlockOldLeaf.CHECK_DECAY, false);

WorldGenBirchTree::WorldGenBirchTree(bool notify, bool useExtraRandomHeightIn)
	:WorldGenAbstractTree(notify), useExtraRandomHeight(useExtraRandomHeightIn)
{

}

bool WorldGenBirchTree::generate(World* worldIn, pcg32& rand, const BlockPos& position)
{
	auto i = rand(3) + 5;
	if (useExtraRandomHeight) 
	{
		i += rand(7);
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
				k = 2;
			}

			MutableBlockPos blockpos$mutableblockpos;

			for (auto l2 = position.getx() - k; l2 <= position.getx() + k && flag; ++l2) 
			{
				for (auto i3 = position.getz() - k; i3 <= position.getz() + k && flag; ++i3) 
				{
					if (j >= 0 && j < 256) 
					{
						if (!canGrowInto(worldIn->getBlockState(blockpos$mutableblockpos.setPos(l2, j, i3)).getBlock())) 
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

				for (auto i2 = position.gety() - 3 + i; i2 <= position.gety() + i; ++i2) 
				{
					int k2 = i2 - (position.gety() + i);
					auto l2 = 1 - k2 / 2;

					for (auto i3 = position.getx() - l2; i3 <= position.getx() + l2; ++i3) 
					{
						auto j1 = i3 - position.getx();

						for (auto k1 = position.getz() - l2; k1 <= position.getz() + l2; ++k1)
						{
							auto l1 = k1 - position.getz();
							if (MathHelper::abs(j1) != l2 || MathHelper::abs(l1) != l2 || rand(2) != 0 && k2 != 0) 
							{
								BlockPos blockpos(i3, i2, k1);
								Material material = worldIn->getBlockState(blockpos).getMaterial();
								if (material == Material::AIR || material == Material::LEAVES) 
								{
									setBlockAndNotifyAdequately(worldIn, blockpos, LEAF);
								}
							}
						}
					}
				}

				for (auto i2 = 0; i2 < i; ++i2) 
				{
					Material material1 = worldIn->getBlockState(position.up(i2)).getMaterial();
					if (material1 == Material::AIR || material1 == Material::LEAVES) 
					{
						setBlockAndNotifyAdequately(worldIn, position.up(i2), LOG);
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
