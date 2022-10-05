#pragma once
#include "GenLayer.h"

class GenLayerDeepOcean : public GenLayer
{
  public:
    GenLayerDeepOcean(int64_t p_i45472_1_, std::shared_ptr<GenLayer> p_i45472_3_);
    std::vector<int32_t> getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight) override;

  private:
};
