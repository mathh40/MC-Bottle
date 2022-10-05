#pragma once
#include "GenLayer.h"

class GenLayerRareBiome : public GenLayer
{
  public:
    GenLayerRareBiome(int64_t p_i45478_1_, std::shared_ptr<GenLayer> p_i45478_3_);
    std::vector<int32_t> getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight) override;
};
