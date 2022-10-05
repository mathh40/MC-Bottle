#include "NoiseGeneratorSimplex.h"

double NoiseGeneratorSimplex::SQRT_3 = MathHelper::sqrt(3.0);
double NoiseGeneratorSimplex::F2     = 0.5 * (SQRT_3 - 1.0);
double NoiseGeneratorSimplex::G2     = (3.0 - SQRT_3) / 6.0;

std::array<Vec3i, 12> NoiseGeneratorSimplex::grad3 = {{1, 1, 0}, {-1, 1, 0}, {1, -1, 0}, {-1, -1, 0},
                                                      {1, 0, 1}, {-1, 0, 1}, {1, 0, -1}, {-1, 0, -1},
                                                      {0, 1, 1}, {0, -1, 1}, {0, 1, -1}, {0, -1, -1}};

NoiseGeneratorSimplex::NoiseGeneratorSimplex()
{
    pcg32 rand;
    init(rand);
}

NoiseGeneratorSimplex::NoiseGeneratorSimplex(pcg32 &seed)
{
    init(seed);
}

void NoiseGeneratorSimplex::init(pcg32 &rand)
{
    xo = MathHelper::nextDouble(rand) * 256.0;
    yo = MathHelper::nextDouble(rand) * 256.0;
    zo = MathHelper::nextDouble(rand) * 256.0;

    for (auto l = 0; l < 256; p[l] = l++)
    {
    }

    for (auto l = 0; l < 256; ++l)
    {
        auto j     = rand(256 - l) + l;
        auto k     = p[l];
        p[l]       = p[j];
        p[j]       = k;
        p[l + 256] = p[l];
    }
}

double NoiseGeneratorSimplex::getValue(double p_151605_1_, double p_151605_3_)
{
    auto d3  = 0.5 * (SQRT_3 - 1.0);
    auto d4  = (p_151605_1_ + p_151605_3_) * d3;
    auto i   = fastFloor(p_151605_1_ + d4);
    auto j   = fastFloor(p_151605_3_ + d4);
    auto d5  = (3.0 - SQRT_3) / 6.0;
    auto d6  = (double)(i + j) * d5;
    auto d7  = (double)i - d6;
    auto d8  = (double)j - d6;
    auto d9  = p_151605_1_ - d7;
    auto d10 = p_151605_3_ - d8;
    auto k   = 0;
    auto l   = 0;
    if (d9 > d10)
    {
        k = 1;
        l = 0;
    }
    else
    {
        k = 0;
        l = 1;
    }

    auto d11 = d9 - (double)k + d5;
    auto d12 = d10 - (double)l + d5;
    auto d13 = d9 - 1.0 + 2.0 * d5;
    auto d14 = d10 - 1.0 + 2.0 * d5;
    auto i1  = i & 255;
    auto j1  = j & 255;
    auto k1  = p[i1 + p[j1]] % 12;
    auto l1  = p[i1 + k + p[j1 + l]] % 12;
    auto i2  = p[i1 + 1 + p[j1 + 1]] % 12;
    auto d15 = 0.5 - d9 * d9 - d10 * d10;
    auto d0  = 0.0;
    if (d15 < 0.0)
    {
        d0 = 0.0;
    }
    else
    {
        d15 *= d15;
        d0 = d15 * d15 * dot(grad3[k1], d9, d10);
    }

    auto d16 = 0.5 - d11 * d11 - d12 * d12;
    auto d1  = 0.0;
    if (d16 < 0.0)
    {
        d1 = 0.0;
    }
    else
    {
        d16 *= d16;
        d1 = d16 * d16 * dot(grad3[l1], d11, d12);
    }

    auto d17 = 0.5 - d13 * d13 - d14 * d14;
    auto d2  = 0.0;
    if (d17 < 0.0)
    {
        d2 = 0.0;
    }
    else
    {
        d17 *= d17;
        d2 = d17 * d17 * dot(grad3[i2], d13, d14);
    }

    return 70.0 * (d0 + d1 + d2);
}

void NoiseGeneratorSimplex::add(std::vector<double> &p_151606_1_, double p_151606_2_, double p_151606_4_,
                                int32_t p_151606_6_, int32_t p_151606_7_, double p_151606_8_, double p_151606_10_,
                                double p_151606_12_)
{
    auto i = 0;

    for (auto j = 0; j < p_151606_7_; ++j)
    {
        auto d0 = (p_151606_4_ + (double)j) * p_151606_10_ + yo;

        for (auto k = 0; k < p_151606_6_; ++k)
        {
            auto d1  = (p_151606_2_ + (double)k) * p_151606_8_ + xo;
            auto d5  = (d1 + d0) * F2;
            auto l   = fastFloor(d1 + d5);
            auto i1  = fastFloor(d0 + d5);
            auto d6  = (double)(l + i1) * G2;
            auto d7  = (double)l - d6;
            auto d8  = (double)i1 - d6;
            auto d9  = d1 - d7;
            auto d10 = d0 - d8;
            auto j1  = 0;
            auto k1  = 0;
            if (d9 > d10)
            {
                j1 = 1;
                k1 = 0;
            }
            else
            {
                j1 = 0;
                k1 = 1;
            }

            auto d11 = d9 - (double)j1 + G2;
            auto d12 = d10 - (double)k1 + G2;
            auto d13 = d9 - 1.0 + 2.0 * G2;
            auto d14 = d10 - 1.0 + 2.0 * G2;
            auto l1  = l & 255;
            auto i2  = i1 & 255;
            auto j2  = p[l1 + p[i2]] % 12;
            auto k2  = p[l1 + j1 + p[i2 + k1]] % 12;
            auto l2  = p[l1 + 1 + p[i2 + 1]] % 12;
            auto d15 = 0.5 - d9 * d9 - d10 * d10;
            auto d2  = 0.0;
            if (d15 < 0.0)
            {
                d2 = 0.0;
            }
            else
            {
                d15 *= d15;
                d2 = d15 * d15 * dot(grad3[j2], d9, d10);
            }

            auto d16 = 0.5 - d11 * d11 - d12 * d12;
            auto d3  = 0.0;
            if (d16 < 0.0)
            {
                d3 = 0.0;
            }
            else
            {
                d16 *= d16;
                d3 = d16 * d16 * dot(grad3[k2], d11, d12);
            }

            auto d17 = 0.5 - d13 * d13 - d14 * d14;
            auto d4  = 0.0;
            if (d17 < 0.0)
            {
                d4 = 0.0;
            }
            else
            {
                d17 *= d17;
                d4 = d17 * d17 * dot(grad3[l2], d13, d14);
            }

            auto i3 = i++;
            p_151606_1_[i3] += 70.0 * (d2 + d3 + d4) * p_151606_12_;
        }
    }
}

int32_t NoiseGeneratorSimplex::fastFloor(double value)
{
    return value > 0.0 ? (int)value : (int)value - 1;
}

double NoiseGeneratorSimplex::dot(std::array<Vec3i, 12> &p_151604_0_, double p_151604_1_, double p_151604_3_)
{
    return (double)p_151604_0_[0] * p_151604_1_ + (double)p_151604_0_[1] * p_151604_3_;
}
