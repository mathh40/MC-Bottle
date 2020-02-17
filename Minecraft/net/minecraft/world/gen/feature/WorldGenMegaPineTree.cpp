#include "WorldGenMegaPineTree.h"
#include "math/BlockPos.h"
#include "material/Material.h"
#include "World.h"

WorldGenMegaPineTree::WorldGenMegaPineTree(bool notify, bool useBaseHeight)
	:WorldGenHugeTrees(notify, 13, 15, TRUNK, LEAF), useBaseHeight(p_i45457_2_)
{
}

bool WorldGenMegaPineTree::generate(World* worldIn, pcg32& rand, const BlockPos& position)
{
	auto i = getHeight(rand);
	if (!ensureGrowable(worldIn, rand, position, i)) 
	{
		return false;
	}
	else 
	{
		createCrown(worldIn, position.getx(), position.getz(), position.gety() + i, 0, rand);

		for (auto j = 0; j < i; ++j) 
		{
			auto iblockstate = worldIn->getBlockState(position.up(j));
			if (iblockstate->getMaterial() == Material::AIR || iblockstate->getMaterial() == Material::LEAVES) 
			{
				setBlockAndNotifyAdequately(worldIn, position.up(j), woodMetadata);
			}

			if (j < i - 1) 
			{
				iblockstate = worldIn->getBlockState(position.add(1, j, 0));
				if (iblockstate->getMaterial() == Material::AIR || iblockstate->getMaterial() == Material::LEAVES)
				{
					setBlockAndNotifyAdequately(worldIn, position.add(1, j, 0), woodMetadata);
				}

				iblockstate = worldIn->getBlockState(position.add(1, j, 1));
				if (iblockstate->getMaterial() == Material::AIR || iblockstate->getMaterial() == Material::LEAVES)
				{
					setBlockAndNotifyAdequately(worldIn, position.add(1, j, 1), woodMetadata);
				}

				iblockstate = worldIn->getBlockState(position.add(0, j, 1));
				if (iblockstate->getMaterial() == Material::AIR || iblockstate->getMaterial() == Material::LEAVES)
				{
					setBlockAndNotifyAdequately(worldIn, position.add(0, j, 1), woodMetadata);
				}
			}
		}

		return true;
	}
}

void WorldGenMegaPineTree::generateSaplings(World* worldIn, pcg32& random, const BlockPos& pos)
{
	placePodzolCircle(worldIn, pos.west().north());
	placePodzolCircle(worldIn, pos.east(2).north());
	placePodzolCircle(worldIn, pos.west().south(2));
	placePodzolCircle(worldIn, pos.east(2).south(2));

	for (auto i = 0; i < 5; ++i) 
	{
		auto j = random(64);
		auto k = j % 8;
		auto l = j / 8;
		if (k == 0 || k == 7 || l == 0 || l == 7) {
			placePodzolCircle(worldIn, pos.add(-3 + k, 0, -3 + l));
		}
	}
}

void WorldGenMegaPineTree::createCrown(World* worldIn, int32_t x, int32_t z, int32_t y, int32_t p_150541_5_,
	pcg32& rand)
{
	auto i = rand(5) + (useBaseHeight ? baseHeight : 3);
	auto j = 0;

	for (auto k = y - i; k <= y; ++k)
	{
		auto l = y - k;
		auto i1 = p_150541_5_ + MathHelper::floor((float)l / (float)i * 3.5F);
		growLeavesLayerStrict(worldIn, BlockPos(x, k, z), i1 + (l > 0 && i1 == j && (k & 1) == 0 ? 1 : 0));
		j = i1;
	}
}

void WorldGenMegaPineTree::placePodzolCircle(World* worldIn, const BlockPos& center)
{
	for (auto i = -2; i <= 2; ++i) 
	{
		for (auto j = -2; j <= 2; ++j) 
		{
			if (MathHelper::abs(i) != 2 || MathHelper::abs(j) != 2) {
				placePodzolAt(worldIn, center.add(i, 0, j));
			}
		}
	}
}

void WorldGenMegaPineTree::placePodzolAt(World* worldIn, const BlockPos& pos)
{
	for (auto i = 2; i >= -3; --i) 
	{
		auto blockpos = pos.up(i);
		auto iblockstate = worldIn->getBlockState(blockpos);
		auto block = iblockstate->getBlock();
		if (block == Blocks::GRASS || block == Blocks::DIRT) 
		{
			setBlockAndNotifyAdequately(worldIn, blockpos, PODZOL);
			break;
		}

		if (iblockstate->getMaterial() != Material::AIR && i < 0) 
		{
			break;
		}
	}
}
