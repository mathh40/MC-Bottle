#pragma once
#include "GenLayer.h"

class GenLayerRiver : public GenLayer
{
  public:
    GenLayerRiver(int64_t p_i2128_1_, std::shared_ptr<GenLayer> p_i2128_3_);
    std::vector<int32_t> getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight) override;

  private:
    int32_t riverFilter(int32_t p_151630_1_);
};
