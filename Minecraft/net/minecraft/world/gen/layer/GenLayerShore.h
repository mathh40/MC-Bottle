#pragma once
#include "GenLayer.h"

class GenLayerShore : public GenLayer
{
  public:
    GenLayerShore(int64_t p_i2130_1_, std::shared_ptr<GenLayer> p_i2130_3_);
    std::vector<int32_t> getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight) override;

  private:
    void replaceIfNeighborOcean(const std::vector<int32_t> p_151632_1_, std::vector<int32_t> &p_151632_2_,
                                int32_t p_151632_3_, int32_t p_151632_4_, int32_t p_151632_5_, int32_t p_151632_6_,
                                int32_t p_151632_7_);
    bool isJungleCompatible(int32_t p_151631_1_);
    bool isMesa(int32_t p_151633_1_) const;
};
