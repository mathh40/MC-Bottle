#include "WorldGenMinable.h"
#include "math/MathHelper.h"
#include "math/BlockPos.h"
#include "World.h"

WorldGenMinable::WorldGenMinable(IBlockState* state, int32_t blockCount)
	:WorldGenMinable(state, blockCount, StonePredicate)
{
}

WorldGenMinable::WorldGenMinable(IBlockState* state, int32_t blockCount, std::function<bool(IBlockState*)> p_i45631_3_)
	: oreBlock(state), numberOfBlocks(blockCount), predicate(p_i45631_3_)
{
}

bool WorldGenMinable::generate(World* worldIn, pcg32& rand, const BlockPos& position)
{	
	auto f = MathHelper::nextFloat(rand) * 3.1415927F;
	auto d0 = (double)((float)(position.getx() + 8) + MathHelper::sin(f) * (float)numberOfBlocks / 8.0F);
	auto d1 = (double)((float)(position.getx() + 8) - MathHelper::sin(f) * (float)numberOfBlocks / 8.0F);
	auto d2 = (double)((float)(position.getz() + 8) + MathHelper::cos(f) * (float)numberOfBlocks / 8.0F);
	auto d3 = (double)((float)(position.getz() + 8) - MathHelper::cos(f) * (float)numberOfBlocks / 8.0F);
	auto d4 = (double)(position.gety() + rand(3) - 2);
	auto d5 = (double)(position.gety() + rand(3) - 2);

	for (int i = 0; i < numberOfBlocks; ++i) 
	{
		auto f1 = (float)i / (float)numberOfBlocks;
		auto d6 = d0 + (d1 - d0) * (double)f1;
		auto d7 = d4 + (d5 - d4) * (double)f1;
		auto d8 = d2 + (d3 - d2) * (double)f1;
		auto d9 = MathHelper::nextDouble(rand) * (double)numberOfBlocks / 16.0;
		auto d10 = (double)(MathHelper::sin(3.1415927F * f1) + 1.0F) * d9 + 1.0;
		auto d11 = (double)(MathHelper::sin(3.1415927F * f1) + 1.0F) * d9 + 1.0;
		auto j = MathHelper::floor(d6 - d10 / 2.0);
		auto k = MathHelper::floor(d7 - d11 / 2.0);
		auto l = MathHelper::floor(d8 - d10 / 2.0);
		auto i1 = MathHelper::floor(d6 + d10 / 2.0);
		auto j1 = MathHelper::floor(d7 + d11 / 2.0);
		auto k1 = MathHelper::floor(d8 + d10 / 2.0);

		for (auto l1 = j; l1 <= i1; ++l1)
		{
			auto d12 = ((double)l1 + 0.5 - d6) / (d10 / 2.0);
			if (d12 * d12 < 1.0) 
			{
				for (auto i2 = k; i2 <= j1; ++i2)
				{
					auto d13 = ((double)i2 + 0.5 - d7) / (d11 / 2.0);
					if (d12 * d12 + d13 * d13 < 1.0) 
					{
						for (auto j2 = l; j2 <= k1; ++j2)
						{
							auto d14 = ((double)j2 + 0.5 - d8) / (d10 / 2.0);
							if (d12 * d12 + d13 * d13 + d14 * d14 < 1.0) 
							{
								BlockPos blockpos(l1, i2, j2);
								if (predicate(worldIn->getBlockState(blockpos))) 
								{
									worldIn->setBlockState(blockpos, oreBlock, 2);
								}
							}
						}
					}
				}
			}
		}
	}

	return true;
}

