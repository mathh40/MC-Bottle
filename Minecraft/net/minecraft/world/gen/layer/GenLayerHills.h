#pragma once
#include "GenLayer.h"
#include "spdlog/logger.h"

#include <memory>

class GenLayerHills : public GenLayer
{
  public:
    GenLayerHills(int64_t p_i45479_1_, std::shared_ptr<GenLayer> parentIn, std::shared_ptr<GenLayer> riverLayerIn);
    std::vector<int32_t> getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight) override;

  private:
    static std::shared_ptr<spdlog::logger> LOGGER;
    std::shared_ptr<GenLayer> riverLayer;
};
