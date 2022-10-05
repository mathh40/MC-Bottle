#pragma once
#include "GenLayer.h"

class GenLayerAddIsland : public GenLayer
{
  public:
    GenLayerAddIsland(int64_t p_i2119_1_, std::shared_ptr<GenLayer> p_i2119_3_);
    std::vector<int32_t> getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight) override;

  private:
};
