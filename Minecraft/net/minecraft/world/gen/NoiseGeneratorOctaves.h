#pragma once
#include "NoiseGenerator.h"
#include "NoiseGeneratorImproved.h"

class NoiseGeneratorOctaves : public NoiseGenerator
{
  public:
    NoiseGeneratorOctaves(pcg32 &seed, int32_t octavesIn);
    std::vector<double> generateNoiseOctaves(std::vector<double> &noiseArray, int32_t xOffset, int32_t yOffset,
                                             int32_t zOffset, int32_t xSize, int32_t ySize, int32_t zSize,
                                             double xScale, double yScale, double zScale);
    std::vector<double> generateNoiseOctaves(std::vector<double> &noiseArray, int32_t xOffset, int32_t zOffset,
                                             int32_t xSize, int32_t zSize, double xScale, double zScale,
                                             double p_76305_10_);

  private:
    std::vector<NoiseGeneratorImproved> generatorCollection;
    int32_t octaves;
};
