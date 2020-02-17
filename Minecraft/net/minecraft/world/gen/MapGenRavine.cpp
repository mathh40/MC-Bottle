#include "MapGenRavine.h"

IBlockState* MapGenRavine::FLOWING_LAVA = Blocks::FLOWING_LAVA.getDefaultState();
IBlockState* MapGenRavine::AIR = Blocks::AIR.getDefaultState();

void MapGenRavine::addTunnel(int64_t p_180707_1_, int32_t p_180707_3_, int32_t p_180707_4_, ChunkPrimer& p_180707_5_,
	double p_180707_6_, double p_180707_8_, double p_180707_10_, float p_180707_12_, float p_180707_13_,
	float p_180707_14_, int32_t p_180707_15_, int32_t p_180707_16_, double p_180707_17_)
{
	pcg32 random(p_180707_1_);
	auto d0 = (double)(p_180707_3_ * 16 + 8);
	auto d1 = (double)(p_180707_4_ * 16 + 8);
	auto f = 0.0F;
	auto f1 = 0.0F;
	if (p_180707_16_ <= 0) 
	{
		auto i = range * 16 - 16;
		p_180707_16_ = i - random(i / 4);
	}

	auto flag1 = false;
	if (p_180707_15_ == -1) 
	{
		p_180707_15_ = p_180707_16_ / 2;
		flag1 = true;
	}

	auto f2 = 1.0F;

	for (auto j = 0; j < 256; ++j) 
	{
		if (j == 0 || random(3) == 0) 
		{
			f2 = 1.0F + random() * MathHelper::nextFloat(random);
		}

		rs[j] = f2 * f2;
	}

	for (; p_180707_15_ < p_180707_16_; ++p_180707_15_) 
	{
		auto d9 = 1.5 + (double)(MathHelper::sin((float)p_180707_15_ * 3.1415927F / (float)p_180707_16_) * p_180707_12_);
		auto d2 = d9 * p_180707_17_;
		d9 *= (double)MathHelper::nextFloat(random) * 0.25 + 0.75;
		d2 *= (double)MathHelper::nextFloat(random) * 0.25 + 0.75;
		auto f3 = MathHelper::cos(p_180707_14_);
		auto f4 = MathHelper::sin(p_180707_14_);
		p_180707_6_ += (double)(MathHelper::cos(p_180707_13_) * f3);
		p_180707_8_ += (double)f4;
		p_180707_10_ += (double)(MathHelper::sin(p_180707_13_) * f3);
		p_180707_14_ *= 0.7F;
		p_180707_14_ += f1 * 0.05F;
		p_180707_13_ += f * 0.05F;
		f1 *= 0.8F;
		f *= 0.5F;
		f1 += (MathHelper::nextFloat(random) - MathHelper::nextFloat(random)) * MathHelper::nextFloat(random) * 2.0F;
		f += (MathHelper::nextFloat(random) - MathHelper::nextFloat(random)) * MathHelper::nextFloat(random) * 4.0F;
		if (flag1 || random(4) != 0) 
		{
			auto d3 = p_180707_6_ - d0;
			auto d4 = p_180707_10_ - d1;
			auto d5 = (double)(p_180707_16_ - p_180707_15_);
			auto d6 = (double)(p_180707_12_ + 2.0F + 16.0F);
			if (d3 * d3 + d4 * d4 - d5 * d5 > d6 * d6) 
			{
				return;
			}

			if (p_180707_6_ >= d0 - 16.0 - d9 * 2.0 && p_180707_10_ >= d1 - 16.0 - d9 * 2.0 && p_180707_6_ <= d0 + 16.0 + d9 * 2.0 && p_180707_10_ <= d1 + 16.0 + d9 * 2.0)
			{
				auto k2 = MathHelper::floor(p_180707_6_ - d9) - p_180707_3_ * 16 - 1;
				auto k = MathHelper::floor(p_180707_6_ + d9) - p_180707_3_ * 16 + 1;
				auto l2 = MathHelper::floor(p_180707_8_ - d2) - 1;
				auto l = MathHelper::floor(p_180707_8_ + d2) + 1;
				auto i3 = MathHelper::floor(p_180707_10_ - d9) - p_180707_4_ * 16 - 1;
				auto i1 = MathHelper::floor(p_180707_10_ + d9) - p_180707_4_ * 16 + 1;
				if (k2 < 0) 
				{
					k2 = 0;
				}

				if (k > 16) 
				{
					k = 16;
				}

				if (l2 < 1) 
				{
					l2 = 1;
				}

				if (l > 248) 
				{
					l = 248;
				}

				if (i3 < 0) 
				{
					i3 = 0;
				}

				if (i1 > 16) 
				{
					i1 = 16;
				}

				bool flag2 = false;

				for (auto j1 = k2; !flag2 && j1 < k; ++j1) 
				{
					for (auto j3 = i3; !flag2 && j3 < i1; ++j3) 
					{
						for (auto l1 = l + 1; !flag2 && l1 >= l2 - 1; --l1)
						{
							if (l1 >= 0 && l1 < 256) 
							{
								auto iblockstate = p_180707_5_->getBlockState(j1, l1, j3);
								if (iblockstate->getBlock() == Blocks::FLOWING_WATER || iblockstate.getBlock() == Blocks::WATER) 
								{
									flag2 = true;
								}

								if (l1 != l2 - 1 && j1 != k2 && j1 != k - 1 && j3 != i3 && j3 != i1 - 1) 
								{
									l1 = l2;
								}
							}
						}
					}
				}

				if (!flag2) 
				{
					MutableBlockPos blockpos$mutableblockpos;

					for (auto j3 = k2; j3 < k; ++j3) 
					{
						auto d10 = ((double)(j3 + p_180707_3_ * 16) + 0.5 - p_180707_6_) / d9;

						for (auto i2 = i3; i2 < i1; ++i2) 
						{
							auto d7 = ((double)(i2 + p_180707_4_ * 16) + 0.5 - p_180707_10_) / d9;
							auto flag = false;
							if (d10* d10 + d7 * d7 < 1.0) 
							{
								for (auto j2 = l; j2 > l2; --j2)
								{
									auto d8 = ((double)(j2 - 1) + 0.5 - p_180707_8_) / d2;
									if ((d10 * d10 + d7 * d7) * (double)rs[j2 - 1] + d8 * d8 / 6.0 < 1.0) 
									{
										auto iblockstate1 = p_180707_5_->getBlockState(j3, j2, i2);
										if (iblockstate1->getBlock() == Blocks::GRASS) 
										{
											flag = true;
										}

										if (iblockstate1->getBlock() == Blocks::STONE || iblockstate1.getBlock() == Blocks::DIRT || iblockstate1.getBlock() == Blocks::GRASS) 
										{
											if (j2 - 1 < 10) 
											{
												p_180707_5_->setBlockState(j3, j2, i2, FLOWING_LAVA);
											}
											else 
											{
												p_180707_5_->setBlockState(j3, j2, i2, AIR);
												if (flag && p_180707_5_->getBlockState(j3, j2 - 1, i2).getBlock() == Blocks::DIRT) 
												{
													blockpos$mutableblockpos.setPos(j3 + p_180707_3_ * 16, 0, i2 + p_180707_4_ * 16);
													p_180707_5_->setBlockState(j3, j2 - 1, i2, world->getBiome(blockpos$mutableblockpos).topBlock);
												}
											}
										}
									}
								}
							}
						}
					}

					if (flag1) 
					{
						break;
					}
				}
			}
		}
	}
}

void MapGenRavine::recursiveGenerate(World* worldIn, int32_t chunkX, int32_t chunkZ, int32_t originalX,
	int32_t originalZ, ChunkPrimer& chunkPrimerIn)
{
	if (rand(50) == 0) 
	{
		auto d0 = (double)(chunkX * 16 + rand(16));
		auto d1 = (double)(rand(rand(40) + 8) + 20);
		auto d2 = (double)(chunkZ * 16 + rand(16));

		for (auto j = 0; j < 1; ++j) 
		{
			auto f = MathHelper::nextFloat(rand) * 6.2831855F;
			auto f1 = (MathHelper::nextFloat(rand) - 0.5F) * 2.0F / 8.0F;
			auto f2 = (MathHelper::nextFloat(rand) * 2.0F + MathHelper::nextFloat(rand)) * 2.0F;
			addTunnel(MathHelper::nextLong(rand), originalX, originalZ, chunkPrimerIn, d0, d1, d2, f2, f, f1, 0, 0, 3.0);
		}
	}
}
