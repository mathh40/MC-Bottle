#pragma once
#include "GenLayer.h"

class GenLayerIsland : public GenLayer
{
  public:
    explicit GenLayerIsland(int64_t p_i2124_1_);
    std::vector<int32_t> getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight) override;

  private:
};
