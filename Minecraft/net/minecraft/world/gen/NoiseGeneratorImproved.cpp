#include "NoiseGeneratorImproved.h"
#include "../../../../../pcg-cpp/pcg_random.hpp"
#include "math/MathHelper.h"

std::array<double, 16> NoiseGeneratorImproved::GRAD_X = { 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0 };
std::array<double, 16> NoiseGeneratorImproved::GRAD_Y = { 1.0, 1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0 };
std::array<double, 16> NoiseGeneratorImproved::GRAD_Z = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 0.0, 1.0, 0.0, -1.0 };
std::array<double, 16> NoiseGeneratorImproved::GRAD_2X = { 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0 };
std::array<double, 16> NoiseGeneratorImproved::GRAD_2Z = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 0.0, 1.0, 0.0, -1.0 };

NoiseGeneratorImproved::NoiseGeneratorImproved()
{
    pcg32 rand;
	init(rand);
}

NoiseGeneratorImproved::NoiseGeneratorImproved(pcg32& p_i45469_1_)
{
	init(p_i45469_1_);
}

double NoiseGeneratorImproved::lerp(double p_76311_1_, double p_76311_3_, double p_76311_5_)
{
	return p_76311_3_ + p_76311_1_ * (p_76311_5_ - p_76311_3_);
}

double NoiseGeneratorImproved::grad2(int32_t p_76309_1_, double p_76309_2_, double p_76309_4_)
{
	auto i = p_76309_1_ & 15;
	return GRAD_2X[i] * p_76309_2_ + GRAD_2Z[i] * p_76309_4_;
}

double NoiseGeneratorImproved::grad(int32_t p_76310_1_, double p_76310_2_, double p_76310_4_, double p_76310_6_)
{
	auto i = p_76310_1_ & 15;
	return GRAD_X[i] * p_76310_2_ + GRAD_Y[i] * p_76310_4_ + GRAD_Z[i] * p_76310_6_;
}

void NoiseGeneratorImproved::populateNoiseArray(std::vector<double>& noiseArray, double xOffset, double yOffset,
	double zOffset, int32_t xSize, int32_t ySize, int32_t zSize, double xScale, double yScale, double zScale,
	double noiseScale)
{
	if (ySize == 1) 
	{
		auto l1 = 0;
		auto d16 = 1.0 / noiseScale;

		for (auto j2 = 0; j2 < xSize; ++j2) 
		{
			auto d2 = xOffset + (double)j2 * xScale + xCoord;
			auto i6 = (int)d2;
			if (d2 < (double)i6) 
			{
				--i6;
			}

			auto k2 = i6 & 255;
			d2 -= (double)i6;
			auto d4 = d2 * d2 * d2 * (d2 * (d2 * 6.0 - 15.0) + 10.0);

			for (auto j6 = 0; j6 < zSize; ++j6) 
			{
				auto d5 = zOffset + (double)j6 * zScale + zCoord;
				auto k6 = (int)d5;
				if (d5 < (double)k6) 
				{
					--k6;
				}

				auto j3 = k6 & 255;
				d5 -= (double)k6;
				auto d6 = d5 * d5 * d5 * (d5 * (d5 * 6.0 - 15.0) + 10.0);
				auto i5 = permutations[k2] + 0;
				auto j5 = permutations[i5] + j3;
				auto j = permutations[k2 + 1] + 0;
				auto k = permutations[j] + j3;
				auto d14 = lerp(d4, grad2(permutations[j5], d2, d5), grad(permutations[k], d2 - 1.0, 0.0, d5));
				auto d15 = lerp(d4, grad(permutations[j5 + 1], d2, 0.0, d5 - 1.0), grad(permutations[k + 1], d2 - 1.0, 0.0, d5 - 1.0));
				auto d21 = lerp(d6, d14, d15);
				auto i7 = l1++;
				noiseArray[i7] += d21 * d16;
			}
		}
	}
	else {
		auto i5 = 0;
		double d0 = 1.0 / noiseScale;
		auto k = -1;
		auto d1 = 0.0;
		auto d2 = 0.0;
		auto d3 = 0.0;
		auto d4 = 0.0;

		for (auto j6 = 0; j6 < xSize; ++j6) 
		{
			auto d5 = xOffset + (double)j6 * xScale + xCoord;
			auto k6 = (int)d5;
			if (d5 < (double)k6) 
			{
				--k6;
			}

			auto j3 = k6 & 255;
			d5 -= (double)k6;
			auto d6 = d5 * d5 * d5 * (d5 * (d5 * 6.0 - 15.0) + 10.0);

			for (auto k3 = 0; k3 < zSize; ++k3) 
			{
				auto d7 = zOffset + (double)k3 * zScale + zCoord;
				auto l3 = (int)d7;
				if (d7 < (double)l3) 
				{
					--l3;
				}

				auto i4 = l3 & 255;
				d7 -= (double)l3;
				auto d8 = d7 * d7 * d7 * (d7 * (d7 * 6.0 - 15.0) + 10.0);

				for (auto j4 = 0; j4 < ySize; ++j4) 
				{
					auto d9 = yOffset + (double)j4 * yScale + yCoord;
					auto k4 = (int)d9;
					if (d9 < (double)k4) 
					{
						--k4;
					}

					auto l4 = k4 & 255;
					d9 -= (double)k4;
					auto d10 = d9 * d9 * d9 * (d9 * (d9 * 6.0 - 15.0) + 10.0);
					if (j4 == 0 || l4 != k) 
					{
						k = l4;
						auto l = permutations[j3] + l4;
						auto i1 = permutations[l] + i4;
						auto j1 = permutations[l + 1] + i4;
						auto k1 = permutations[j3 + 1] + l4;
						auto l1 = permutations[k1] + i4;
						auto i2 = permutations[k1 + 1] + i4;
						d1 = lerp(d6, grad(permutations[i1], d5, d9, d7), grad(permutations[l1], d5 - 1.0, d9, d7));
						d2 = lerp(d6, grad(permutations[j1], d5, d9 - 1.0, d7), grad(permutations[i2], d5 - 1.0, d9 - 1.0, d7));
						d3 = lerp(d6, grad(permutations[i1 + 1], d5, d9, d7 - 1.0), grad(permutations[l1 + 1], d5 - 1.0, d9, d7 - 1.0));
						d4 = lerp(d6, grad(permutations[j1 + 1], d5, d9 - 1.0, d7 - 1.0), grad(permutations[i2 + 1], d5 - 1.0, d9 - 1.0, d7 - 1.0));
					}

					auto d11 = lerp(d10, d1, d2);
					auto d12 = lerp(d10, d3, d4);
					auto d13 = lerp(d8, d11, d12);
					auto j7 = i5++;
					noiseArray[j7] += d13 * d0;
				}
			}
		}
	}
}

void NoiseGeneratorImproved::init(pcg32& rand)
{
	xCoord = MathHelper::nextDouble(rand) * 256.0;
	yCoord = MathHelper::nextDouble(rand) * 256.0;
	zCoord = MathHelper::nextDouble(rand) * 256.0;

	for (auto l = 0; l < 256; permutations[l] = l++) 
	{
	}

	for (auto l = 0; l < 256; ++l) 
	{
		auto j = rand(256 - l) + l;
		auto k = permutations[l];
		permutations[l] = permutations[j];
		permutations[j] = k;
		permutations[l + 256] = permutations[l];
	}
}
