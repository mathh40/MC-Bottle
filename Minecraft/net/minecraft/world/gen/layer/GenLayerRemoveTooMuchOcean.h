#pragma once
#include "GenLayer.h"

class GenLayerRemoveTooMuchOcean : public GenLayer
{
  public:
    GenLayerRemoveTooMuchOcean(int64_t p_i45480_1_, std::shared_ptr<GenLayer> p_i45480_3_);
    std::vector<int32_t> getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight) override;
};
