#include "MapGenCavesHell.h"

IBlockState* MapGenCavesHell::AIR = Blocks::AIR.getDefaultState();

void MapGenCavesHell::addRoom(int64_t p_180705_1_, int32_t p_180705_3_, int32_t p_180705_4_, ChunkPrimer& p_180705_5_,
	double p_180705_6_, double p_180705_8_, double p_180705_10_)
{
	addTunnel(p_180705_1_, p_180705_3_, p_180705_4_, p_180705_5_, p_180705_6_, p_180705_8_, p_180705_10_, 1.0F + MathHelper::nextFloat(rand) * 6.0F, 0.0F, 0.0F, -1, -1, 0.5);
}

void MapGenCavesHell::addTunnel(int64_t p_180704_1_, int32_t p_180704_3_, int32_t p_180704_4_, ChunkPrimer& p_180704_5_, double p_180704_6_, double p_180704_8_, double p_180704_10_, float p_180704_12_, float p_180704_13_, float p_180704_14_, int32_t p_180704_15_, int32_t p_180704_16_, double p_180704_17_)
{
	auto d0 = (double)(p_180704_3_ * 16 + 8);
	auto d1 = (double)(p_180704_4_ * 16 + 8);
	auto f = 0.0F;
	auto f1 = 0.0F;
	pcg32 random(p_180704_1_);
	if (p_180704_16_ <= 0) 
	{
		auto i = range * 16 - 16;
		p_180704_16_ = i - random(i / 4);
	}

	auto flag1 = false;
	if (p_180704_15_ == -1) 
	{
		p_180704_15_ = p_180704_16_ / 2;
		flag1 = true;
	}

	auto j = random(p_180704_16_ / 2) + p_180704_16_ / 4;

	for (auto flag = random(6) == 0; p_180704_15_ < p_180704_16_; ++p_180704_15_) 
	{
		auto d2 = 1.5 + (double)(MathHelper::sin((float)p_180704_15_ * 3.1415927F / (float)p_180704_16_) * p_180704_12_);
		auto d3 = d2 * p_180704_17_;
		auto f2 = MathHelper::cos(p_180704_14_);
		auto f3 = MathHelper::sin(p_180704_14_);
		p_180704_6_ += (double)(MathHelper::cos(p_180704_13_) * f2);
		p_180704_8_ += (double)f3;
		p_180704_10_ += (double)(MathHelper::sin(p_180704_13_) * f2);
		if (flag) {
			p_180704_14_ *= 0.92F;
		}
		else {
			p_180704_14_ *= 0.7F;
		}

		p_180704_14_ += f1 * 0.1F;
		p_180704_13_ += f * 0.1F;
		f1 *= 0.9F;
		f *= 0.75F;

		f1 += (MathHelper::nextFloat(random) - MathHelper::nextFloat(random)) * MathHelper::nextFloat(random) * 2.0F;
		f += (MathHelper::nextFloat(random) - MathHelper::nextFloat(random)) * MathHelper::nextFloat(random) * 4.0F;
		if (!flag1 && p_180704_15_ == j && p_180704_12_ > 1.0F) {
			addTunnel(MathHelper::nextLong(random), p_180704_3_, p_180704_4_, p_180704_5_, p_180704_6_, p_180704_8_, p_180704_10_, MathHelper::nextLong(random) * 0.5F + 0.5F, p_180704_13_ - 1.5707964F, p_180704_14_ / 3.0F, p_180704_15_, p_180704_16_, 1.0);
			addTunnel(MathHelper::nextLong(random), p_180704_3_, p_180704_4_, p_180704_5_, p_180704_6_, p_180704_8_, p_180704_10_, MathHelper::nextLong(random) * 0.5F + 0.5F, p_180704_13_ + 1.5707964F, p_180704_14_ / 3.0F, p_180704_15_, p_180704_16_, 1.0);
			return;
		}

		if (flag1 || random(4) != 0) 
		{
			auto d4 = p_180704_6_ - d0;
			auto d5 = p_180704_10_ - d1;
			auto d6 = (double)(p_180704_16_ - p_180704_15_);
			auto d7 = (double)(p_180704_12_ + 2.0F + 16.0F);
			if (d4 * d4 + d5 * d5 - d6 * d6 > d7 * d7) 
			{
				return;
			}

			if (p_180704_6_ >= d0 - 16.0 - d2 * 2.0 && p_180704_10_ >= d1 - 16.0 - d2 * 2.0 && p_180704_6_ <= d0 + 16.0 + d2 * 2.0 && p_180704_10_ <= d1 + 16.0 + d2 * 2.0) 
			{
				auto j2 = MathHelper::floor(p_180704_6_ - d2) - p_180704_3_ * 16 - 1;
				auto k = MathHelper::floor(p_180704_6_ + d2) - p_180704_3_ * 16 + 1;
				auto k2 = MathHelper::floor(p_180704_8_ - d3) - 1;
				auto l = MathHelper::floor(p_180704_8_ + d3) + 1;
				auto l2 = MathHelper::floor(p_180704_10_ - d2) - p_180704_4_ * 16 - 1;
				auto i1 = MathHelper::floor(p_180704_10_ + d2) - p_180704_4_ * 16 + 1;
				if (j2 < 0) 
				{
					j2 = 0;
				}

				if (k > 16) 
				{
					k = 16;
				}

				if (k2 < 1) 
				{
					k2 = 1;
				}

				if (l > 120) 
				{
					l = 120;
				}

				if (l2 < 0) 
				{
					l2 = 0;
				}

				if (i1 > 16) 
				{
					i1 = 16;
				}

				auto flag2 = false;

				for (auto i3 = j2; !flag2 && i3 < k; ++i3) 
				{
					for (auto k1 = l2; !flag2 && k1 < i1; ++k1)
					{
						for (auto l1 = l + 1; !flag2 && l1 >= k2 - 1; --l1)
						{
							if (l1 >= 0 && l1 < 128) 
							{
								auto iblockstate = p_180704_5_->getBlockState(i3, l1, k1);
								if (iblockstate.getBlock() == Blocks::FLOWING_LAVA || iblockstate.getBlock() == Blocks::LAVA) 
								{
									flag2 = true;
								}

								if (l1 != k2 - 1 && i3 != j2 && i3 != k - 1 && k1 != l2 && k1 != i1 - 1) {
									l1 = k2;
								}
							}
						}
					}
				}

				if (!flag2) 
				{
					for (auto i3 = j2; i3 < k; ++i3)
					{
						auto d10 = ((double)(i3 + p_180704_3_ * 16) + 0.5 - p_180704_6_) / d2;

						for (auto j3 = l2; j3 < i1; ++j3)
						{
							auto d8 = ((double)(j3 + p_180704_4_ * 16) + 0.5 - p_180704_10_) / d2;

							for (int i2 = l; i2 > k2; --i2) 
							{
								auto d9 = ((double)(i2 - 1) + 0.5 - p_180704_8_) / d3;
								if (d9 > -0.7 && d10 * d10 + d9 * d9 + d8 * d8 < 1.0) 
								{
									auto iblockstate1 = p_180704_5_->getBlockState(i3, i2, j3);
									if (iblockstate1.getBlock() == Blocks::NETHERRACK || iblockstate1.getBlock() == Blocks::DIRT || iblockstate1.getBlock() == Blocks::GRASS) 
									{
										p_180704_5_->setBlockState(i3, i2, j3, AIR);
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

void MapGenCavesHell::recursiveGenerate(World* worldIn, int32_t chunkX, int32_t chunkZ, int32_t originalX, int32_t originalZ, ChunkPrimer& chunkPrimerIn)
{
	auto i = rand(rand(rand(10) + 1) + 1);
	if (rand(5) != 0) 
	{
		i = 0;
	}

	for (auto j = 0; j < i; ++j) 
	{
		auto d0 = (double)(chunkX * 16 + rand(16));
		auto d1 = (double)rand(128);
		auto d2 = (double)(chunkZ * 16 + rand(16));
		auto k = 1;
		if (rand(4) == 0) 
		{		
			addRoom(MathHelper::nextLong(rand), originalX, originalZ, chunkPrimerIn, d0, d1, d2);
			k += rand(4);
		}

		for (auto l = 0; l < k; ++l)
		{
			auto f = MathHelper::nextFloat(rand) * 6.2831855F;
			auto f1 = (MathHelper::nextFloat(rand) - 0.5F) * 2.0F / 8.0F;
			auto f2 = MathHelper::nextFloat(rand) * 2.0F + MathHelper::nextFloat(rand);
			addTunnel(MathHelper::nextLong(rand), originalX, originalZ, chunkPrimerIn, d0, d1, d2, f2 * 2.0F, f, f1, 0, 0, 0.5);
		}
	}
}