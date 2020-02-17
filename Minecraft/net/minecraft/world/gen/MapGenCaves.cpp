#include "MapGenCaves.h"
#include "chunk/ChunkPrimer.h"

IBlockState* MapGenCaves::BLK_LAVA = Blocks::LAVA->getDefaultState();
IBlockState* MapGenCaves::BLK_AIR = Blocks::AIR->getDefaultState();
IBlockState* MapGenCaves::BLK_SANDSTONE = Blocks::SANDSTONE->getDefaultState();
IBlockState* MapGenCaves::BLK_RED_SANDSTONE = Blocks::RED_SANDSTONE->getDefaultState();

void MapGenCaves::addRoom(int64_t p_180703_1_, int32_t p_180703_3_, int32_t p_180703_4_, ChunkPrimer& p_180703_5_,
	double p_180703_6_, double p_180703_8_, double p_180703_10_)
{
	std::uniform_real_distribution<float> dist;
	addTunnel(p_180703_1_, p_180703_3_, p_180703_4_, p_180703_5_, p_180703_6_, p_180703_8_, p_180703_10_, 1.0F + dist(rand) * 6.0F, 0.0F, 0.0F, -1, -1, 0.5);
}

void MapGenCaves::addTunnel(int64_t p_180702_1_, int32_t p_180702_3_, int32_t p_180702_4_, ChunkPrimer& p_180702_5_,
	double p_180702_6_, double p_180702_8_, double p_180702_10_, float p_180702_12_, float p_180702_13_,
	float p_180702_14_, int32_t p_180702_15_, int32_t p_180702_16_, double p_180702_17_)
{
	auto d0 = (double)(p_180702_3_ * 16 + 8);
	auto d1 = (double)(p_180702_4_ * 16 + 8);
	auto f = 0.0F;
	auto f1 = 0.0F;
	pcg32 random(p_180702_1_);
	if (p_180702_16_ <= 0) 
	{
		auto i = range * 16 - 16;
		p_180702_16_ = i - random(i / 4);
	}

	bool flag2 = false;
	if (p_180702_15_ == -1) 
	{
		p_180702_15_ = p_180702_16_ / 2;
		flag2 = true;
	}

	auto j = random(p_180702_16_ / 2) + p_180702_16_ / 4;

	for (auto flag = random(6) == 0; p_180702_15_ < p_180702_16_; ++p_180702_15_) 
	{
		auto d2 = 1.5 + (double)(MathHelper::sin((float)p_180702_15_ * 3.1415927F / (float)p_180702_16_) * p_180702_12_);
		auto d3 = d2 * p_180702_17_;
		auto f2 = MathHelper::cos(p_180702_14_);
		auto f3 = MathHelper::sin(p_180702_14_);
		p_180702_6_ += (double)(MathHelper::cos(p_180702_13_) * f2);
		p_180702_8_ += (double)f3;
		p_180702_10_ += (double)(MathHelper::sin(p_180702_13_) * f2);
		if (flag) 
		{
			p_180702_14_ *= 0.92F;
		}
		else {
			p_180702_14_ *= 0.7F;
		}

		p_180702_14_ += f1 * 0.1F;
		p_180702_13_ += f * 0.1F;
		f1 *= 0.9F;
		f *= 0.75F;
		std::uniform_real_distribution<float> dist;
		f1 += (dist(random) - dist(random)) * dist(random) * 2.0F;
		f += (dist(random) - dist(random)) * dist(random) * 4.0F;
		if (!flag2 && p_180702_15_ == j && p_180702_12_ > 1.0F && p_180702_16_ > 0) 
		{
			addTunnel(MathHelper::nextLong(random), p_180702_3_, p_180702_4_, p_180702_5_, p_180702_6_, p_180702_8_, p_180702_10_, dist(random) * 0.5F + 0.5F, p_180702_13_ - 1.5707964F, p_180702_14_ / 3.0F, p_180702_15_, p_180702_16_, 1.0);
			addTunnel(MathHelper::nextLong(random), p_180702_3_, p_180702_4_, p_180702_5_, p_180702_6_, p_180702_8_, p_180702_10_, dist(random) * 0.5F + 0.5F, p_180702_13_ + 1.5707964F, p_180702_14_ / 3.0F, p_180702_15_, p_180702_16_, 1.0);
			return;
		}

		if (flag2 || random(4) != 0) 
		{
			auto d4 = p_180702_6_ - d0;
			auto d5 = p_180702_10_ - d1;
			auto d6 = (double)(p_180702_16_ - p_180702_15_);
			auto d7 = (double)(p_180702_12_ + 2.0F + 16.0F);
			if (d4 * d4 + d5 * d5 - d6 * d6 > d7 * d7) 
			{
				return;
			}

			if (p_180702_6_ >= d0 - 16.0 - d2 * 2.0 && p_180702_10_ >= d1 - 16.0 - d2 * 2.0 && p_180702_6_ <= d0 + 16.0 + d2 * 2.0 && p_180702_10_ <= d1 + 16.0 + d2 * 2.0) 
			{
				auto k2 = MathHelper::floor(p_180702_6_ - d2) - p_180702_3_ * 16 - 1;
				auto k = MathHelper::floor(p_180702_6_ + d2) - p_180702_3_ * 16 + 1;
				auto l2 = MathHelper::floor(p_180702_8_ - d3) - 1;
				auto l = MathHelper::floor(p_180702_8_ + d3) + 1;
				auto i3 = MathHelper::floor(p_180702_10_ - d2) - p_180702_4_ * 16 - 1;
				auto i1 = MathHelper::floor(p_180702_10_ + d2) - p_180702_4_ * 16 + 1;
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

				bool flag3 = false;

				for (auto j1 = k2; !flag3 && j1 < k; ++j1) 
				{
					for (auto j3 = i3; !flag3 && j3 < i1; ++j3)
					{
						for (auto l1 = l + 1; !flag3 && l1 >= l2 - 1; --l1) 
						{
							if (l1 >= 0 && l1 < 256) 
							{
								auto iblockstate = p_180702_5_.getBlockState(j1, l1, j3);
								if (iblockstate.getBlock() == Blocks.FLOWING_WATER || iblockstate.getBlock() == Blocks.WATER) 
								{
									flag3 = true;
								}

								if (l1 != l2 - 1 && j1 != k2 && j1 != k - 1 && j3 != i3 && j3 != i1 - 1) 
								{
									l1 = l2;
								}
							}
						}
					}
				}

				if (!flag3) 
				{
					MutableBlockPos blockpos$mutableblockpos;

					for (auto j3 = k2; j3 < k; ++j3) 
					{
						double d10 = ((double)(j3 + p_180702_3_ * 16) + 0.5 - p_180702_6_) / d2;

						for (auto i2 = i3; i2 < i1; ++i2) {
							auto d8 = ((double)(i2 + p_180702_4_ * 16) + 0.5 - p_180702_10_) / d2;
							auto flag1 = false;
							if (d10 * d10 + d8 * d8 < 1.0) 
							{
								for (auto j2 = l; j2 > l2; --j2)
								{
									auto d9 = ((double)(j2 - 1) + 0.5 - p_180702_8_) / d3;
									if (d9 > -0.7 && d10 * d10 + d9 * d9 + d8 * d8 < 1.0) 
									{
										auto iblockstate1 = p_180702_5_.getBlockState(j3, j2, i2);
										auto iblockstate2 = (IBlockState)MoreObjects.firstNonNull(p_180702_5_.getBlockState(j3, j2 + 1, i2), BLK_AIR);
										if (iblockstate1->getBlock() == Blocks.GRASS || iblockstate1->getBlock() == Blocks.MYCELIUM) 
										{
											flag1 = true;
										}

										if (canReplaceBlock(iblockstate1, iblockstate2)) 
										{
											if (j2 - 1 < 10) 
											{
												p_180702_5_.setBlockState(j3, j2, i2, BLK_LAVA);
											}
											else {
												p_180702_5_.setBlockState(j3, j2, i2, BLK_AIR);
												if (flag1 && p_180702_5_.getBlockState(j3, j2 - 1, i2).getBlock() == Blocks.DIRT) 
												{
													blockpos$mutableblockpos.setPos(j3 + p_180702_3_ * 16, 0, i2 + p_180702_4_ * 16);
													p_180702_5_.setBlockState(j3, j2 - 1, i2, world->getBiome(blockpos$mutableblockpos).topBlock.getBlock()->getDefaultState());
												}
											}
										}
									}
								}
							}
						}
					}

					if (flag2) 
					{
						break;
					}
				}
			}
		}
	}
}

bool MapGenCaves::canReplaceBlock(IBlockState* p_175793_1_, IBlockState* p_175793_2_)
{
	if (p_175793_1_->getBlock() == Blocks::STONE) 
	{
		return true;
	}
	else if (p_175793_1_->getBlock() == Blocks::DIRT) 
	{
		return true;
	}
	else if (p_175793_1_->getBlock() == Blocks::GRASS) 
	{
		return true;
	}
	else if (p_175793_1_->getBlock() == Blocks::HARDENED_CLAY) 
	{
		return true;
	}
	else if (p_175793_1_->getBlock() == Blocks::STAINED_HARDENED_CLAY) 
	{
		return true;
	}
	else if (p_175793_1_->getBlock() == Blocks::SANDSTONE) 
	{
		return true;
	}
	else if (p_175793_1_->getBlock() == Blocks::RED_SANDSTONE) 
	{
		return true;
	}
	else if (p_175793_1_->getBlock() == Blocks::MYCELIUM)
	{
		return true;
	}
	else if (p_175793_1_->getBlock() == Blocks::SNOW_LAYER) 
	{
		return true;
	}
	else {
		return (p_175793_1_->getBlock() == Blocks::SAND || p_175793_1_->getBlock() == Blocks::GRAVEL) && p_175793_2_->getMaterial() != Material::WATER;
	}
}

void MapGenCaves::recursiveGenerate(World* worldIn, int32_t chunkX, int32_t chunkZ, int32_t originalX,
	int32_t originalZ, ChunkPrimer& chunkPrimerIn)
{
	auto i = rand(rand(rand(15) + 1) + 1);
	if (rand(7) != 0) 
	{
		i = 0;
	}

	for (int j = 0; j < i; ++j) 
	{
		auto d0 = (double)(chunkX * 16 + rand(16));
		auto d1 = (double)rand(rand(120) + 8);
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
			if (MathHelper::nextFloat(rand) == 0) 
			{
				f2 *= MathHelper::nextFloat(rand) * MathHelper::nextFloat(rand) * 3.0F + 1.0F;
			}

			addTunnel(MathHelper::nextLong(rand), originalX, originalZ, chunkPrimerIn, d0, d1, d2, f2, f, f1, 0, 0, 1.0);
		}
	}
}
