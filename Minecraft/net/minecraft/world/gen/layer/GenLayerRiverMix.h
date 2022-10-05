#pragma once
#include "GenLayer.h"

class GenLayerRiverMix : public GenLayer
{
  public:
    GenLayerRiverMix(int64_t p_i2129_1_, std::shared_ptr<GenLayer> p_i2129_3_, std::shared_ptr<GenLayer> p_i2129_4_);
    void initWorldGenSeed(int64_t seed) override;
    std::vector<int32_t> getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight) override;

  private:
    std::shared_ptr<GenLayer> biomePatternGeneratorChain;
    std::shared_ptr<GenLayer> riverPatternGeneratorChain;
};
