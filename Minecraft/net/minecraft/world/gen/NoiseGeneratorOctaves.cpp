#include "NoiseGeneratorOctaves.h"

#include "math/MathHelper.h"

NoiseGeneratorOctaves::NoiseGeneratorOctaves(pcg32 &seed, int32_t octavesIn)
    : octaves(octavesIn), generatorCollection(octavesIn)
{
    for (auto i = 0; i < octavesIn; ++i)
    {
        generatorCollection[i] = NoiseGeneratorImproved(seed);
    }
}

std::vector<double> NoiseGeneratorOctaves::generateNoiseOctaves(std::vector<double> &noiseArray, int32_t xOffset,
                                                                int32_t yOffset, int32_t zOffset, int32_t xSize,
                                                                int32_t ySize, int32_t zSize, double xScale,
                                                                double yScale, double zScale)
{
    if (noiseArray.empty())
    {
        noiseArray.resize(xSize * ySize * zSize);
    }
    else
    {
        for (auto i = 0; i < noiseArray.size(); ++i)
        {
            noiseArray[i] = 0.0;
        }
    }

    auto d3 = 1.0;

    for (auto j = 0; j < octaves; ++j)
    {
        auto d0 = (double)xOffset * d3 * xScale;
        auto d1 = (double)yOffset * d3 * yScale;
        auto d2 = (double)zOffset * d3 * zScale;
        auto k  = MathHelper::lfloor(d0);
        auto l  = MathHelper::lfloor(d2);
        d0 -= (double)k;
        d2 -= (double)l;
        k %= 16777216L;
        l %= 16777216L;
        d0 += (double)k;
        d2 += (double)l;
        generatorCollection[j].populateNoiseArray(noiseArray, d0, d1, d2, xSize, ySize, zSize, xScale * d3, yScale * d3,
                                                  zScale * d3, d3);
        d3 /= 2.0;
    }

    return noiseArray;
}

std::vector<double> NoiseGeneratorOctaves::generateNoiseOctaves(std::vector<double> &noiseArray, int32_t xOffset,
                                                                int32_t zOffset, int32_t xSize, int32_t zSize,
                                                                double xScale, double zScale, double p_76305_10_)
{
    return generateNoiseOctaves(noiseArray, xOffset, 10, zOffset, xSize, 1, zSize, xScale, 1.0, zScale);
}
