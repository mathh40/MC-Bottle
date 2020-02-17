#include "WorldGenMegaJungle.h"
#include "math/BlockPos.h"
#include "math/MathHelper.h"
#include "properties/PropertyBool.h"
#include "World.h"

WorldGenMegaJungle::WorldGenMegaJungle(bool notify, int32_t baseHeightIn, int32_t extraRandomHeightIn,
	IBlockState* woodMetadataIn, IBlockState* p_i46448_5_)
	:WorldGenHugeTrees(notify, baseHeightIn, extraRandomHeightIn, woodMetadataIn, p_i46448_5_)
{
}

bool WorldGenMegaJungle::generate(World* worldIn, pcg32& rand, const BlockPos& position)
{
	auto i = getHeight(rand);
	if (!ensureGrowable(worldIn, rand, position, i)) 
	{
		return false;
	}
	else 
	{
		createCrown(worldIn, position.up(i), 2);

		for (auto i2 = position.gety() + i - 2 - rand(4); i2 > position.gety() + i / 2; i2 -= 2 + rand(4)) 
		{	
			auto f = MathHelper::nextFloat(rand) * 6.2831855F;
			auto k = position.getx() + (int)(0.5F + MathHelper::cos(f) * 4.0F);
			auto l = position.getz() + (int)(0.5F + MathHelper::sin(f) * 4.0F);

			for (auto j2 = 0; j2 < 5; ++j2)
			{
				k = position.getx() + (int)(1.5F + MathHelper::cos(f) * (float)j2);
				l = position.getz() + (int)(1.5F + MathHelper::sin(f) * (float)j2);
				setBlockAndNotifyAdequately(worldIn, BlockPos(k, i2 - 3 + j2 / 2, l), woodMetadata);
			}

			auto j2 = 1 + rand(2);
			auto j1 = i2;

			for (auto k1 = i2 - j2; k1 <= j1; ++k1) 
			{
				auto l1 = k1 - j1;
				growLeavesLayer(worldIn BlockPos(k, k1, l), 1 - l1);
			}
		}

		for (auto i2 = 0; i2 < i; ++i2) 
		{
			auto blockpos = position.up(i2);
			if (canGrowInto(worldIn->getBlockState(blockpos).getBlock())) 
			{
				setBlockAndNotifyAdequately(worldIn, blockpos, woodMetadata);
				if (i2 > 0) 
				{
					placeVine(worldIn, rand, blockpos.west(), BlockVine::EAST);
					placeVine(worldIn, rand, blockpos.north(), BlockVine::SOUTH);
				}
			}

			if (i2 < i - 1) 
			{
				BlockPos blockpos1 = blockpos.east();
				if (canGrowInto(worldIn->getBlockState(blockpos1).getBlock())) 
				{
					setBlockAndNotifyAdequately(worldIn, blockpos1, woodMetadata);
					if (i2 > 0) 
					{
						placeVine(worldIn, rand, blockpos1.east(), BlockVine::WEST);
						placeVine(worldIn, rand, blockpos1.north(), BlockVine::SOUTH);
					}
				}

				BlockPos blockpos2 = blockpos.south().east();
				if (canGrowInto(worldIn->getBlockState(blockpos2).getBlock())) 
				{
					setBlockAndNotifyAdequately(worldIn, blockpos2, woodMetadata);
					if (i2 > 0) 
					{
						placeVine(worldIn, rand, blockpos2.east(), BlockVine::WEST);
						placeVine(worldIn, rand, blockpos2.south(), BlockVine::NORTH);
					}
				}

				BlockPos blockpos3 = blockpos.south();
				if (canGrowInto(worldIn->getBlockState(blockpos3).getBlock())) 
				{
					setBlockAndNotifyAdequately(worldIn, blockpos3, woodMetadata);
					if (i2 > 0) 
					{
						placeVine(worldIn, rand, blockpos3.west(), BlockVine::EAST);
						placeVine(worldIn, rand, blockpos3.south(), BlockVine::NORTH);
					}
				}
			}
		}

		return true;
	}
}

void WorldGenMegaJungle::placeVine(World* p_181632_1_, pcg32& p_181632_2_, const BlockPos& p_181632_3_,
	PropertyBool* p_181632_4_)
{
	if (p_181632_2_(3) > 0 && p_181632_1_->isAirBlock(p_181632_3_)) 
	{
		setBlockAndNotifyAdequately(p_181632_1_, p_181632_3_, Blocks::VINE.getDefaultState().withProperty(p_181632_4_, true));
	}
}

void WorldGenMegaJungle::createCrown(World* worldIn, const BlockPos& p_175930_2_, int32_t p_175930_3_)
{
	for (auto j = -2; j <= 0; ++j) 
	{
		growLeavesLayerStrict(worldIn, p_175930_2_.up(j), p_175930_3_ + 1 - j);
	}
}
