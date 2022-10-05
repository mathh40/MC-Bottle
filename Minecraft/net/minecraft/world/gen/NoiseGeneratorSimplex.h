#pragma once
#include "math/MathHelper.h"

class NoiseGeneratorSimplex
{
  public:
    static double SQRT_3;
    double xo;
    double yo;
    double zo;

    NoiseGeneratorSimplex();
    NoiseGeneratorSimplex(Random &seed);
    void init(Random &rand);
    double getValue(double p_151605_1_, double p_151605_3_);
    void add(std::vector<double> &p_151606_1_, double p_151606_2_, double p_151606_4_, int32_t p_151606_6_,
             int32_t p_151606_7_, double p_151606_8_, double p_151606_10_, double p_151606_12_);

  private:
    static std::array<Vec3i, 12> grad3;
    std::array<int32_t, 512> p;
    static double F2;
    static double G2;

    static int32_t fastFloor(double value);
    static double dot(std::array<Vec3i, 12> &p_151604_0_, double p_151604_1_, double p_151604_3_);
};
