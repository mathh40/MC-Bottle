#pragma once
#include "NoiseGenerator.h"
#include <array>
#include <vector>
#include "../../../../../pcg-cpp/pcg_random.hpp"

class NoiseGeneratorImproved :public NoiseGenerator
{
public:
	double xCoord;
	double yCoord;
	double zCoord;

	NoiseGeneratorImproved();
	explicit NoiseGeneratorImproved(pcg32& p_i45469_1_);

	static double lerp(double p_76311_1_, double p_76311_3_, double p_76311_5_);
	static double grad2(int32_t p_76309_1_, double p_76309_2_, double p_76309_4_);
	static double grad(int32_t p_76310_1_, double p_76310_2_, double p_76310_4_, double p_76310_6_);

	void populateNoiseArray(std::vector<double>& noiseArray, double xOffset, double yOffset, double zOffset, int32_t xSize, int32_t ySize, int32_t zSize, double xScale, double yScale, double zScale, double noiseScale);
protected:
private:
	std::array<int32_t, 512> permutations;
	static std::array<double,16> GRAD_X;
	static std::array<double, 16> GRAD_Y;
	static std::array<double, 16> GRAD_Z;
	static std::array<double, 16> GRAD_2X;
	static std::array<double, 16> GRAD_2Z;
	void init(pcg32& rand);
};
