#include "NoiseGeneratorPerlin.h"

NoiseGeneratorPerlin::NoiseGeneratorPerlin(pcg32 &seed, int levelsIn) : levels(levelsIn), noiseLevels(levelsIn)
{
    for (auto i = 0; i < levelsIn; ++i)
    {
        noiseLevels[i] = NoiseGeneratorSimplex(seed);
    }
}

double NoiseGeneratorPerlin::getValue(double p_151601_1_, double p_151601_3_)
{
    auto d0 = 0.0;
    auto d1 = 1.0;

    for (auto i = 0; i < levels; ++i)
    {
        d0 += noiseLevels[i].getValue(p_151601_1_ * d1, p_151601_3_ * d1) / d1;
        d1 /= 2.0;
    }

    return d0;
}

std::vector<double> NoiseGeneratorPerlin::getRegion(std::vector<double> &p_151599_1_, double p_151599_2_,
                                                    double p_151599_4_, int32_t p_151599_6_, int32_t p_151599_7_,
                                                    double p_151599_8_, double p_151599_10_, double p_151599_12_)
{
    return getRegion(p_151599_1_, p_151599_2_, p_151599_4_, p_151599_6_, p_151599_7_, p_151599_8_, p_151599_10_,
                     p_151599_12_, 0.5);
}

std::vector<double> NoiseGeneratorPerlin::getRegion(std::vector<double> &p_151600_1_, double p_151600_2_,
                                                    double p_151600_4_, int32_t p_151600_6_, int32_t p_151600_7_,
                                                    double p_151600_8_, double p_151600_10_, double p_151600_12_,
                                                    double p_151600_14_)
{
    if (!p_151600_1_.empty() && p_151600_1_.size() >= p_151600_6_ * p_151600_7_)
    {
        for (auto i = 0; i < p_151600_1_.size(); ++i)
        {
            p_151600_1_[i] = 0.0;
        }
    }
    else
    {
        p_151600_1_.resize(p_151600_6_ * p_151600_7_);
    }

    double d1 = 1.0D;
    double d0 = 1.0D;

    for (int j = 0; j < this.levels; ++j)
    {
        this.noiseLevels[j].add(p_151600_1_, p_151600_2_, p_151600_4_, p_151600_6_, p_151600_7_, p_151600_8_ * d0 * d1,
                                p_151600_10_ * d0 * d1, 0.55D / d1);
        d0 *= p_151600_12_;
        d1 *= p_151600_14_;
    }

    return p_151600_1_;
}
