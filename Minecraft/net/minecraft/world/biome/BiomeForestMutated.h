#pragma once
#include "BiomeForest.h"

class BiomeForestMutated : public BiomeForest
{
  public:
    BiomeForestMutated(BiomeProperties properties);
    WorldGenAbstractTree getRandomTreeFeature(pcg32 &rand) override;

  private:
};
