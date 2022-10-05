#pragma once
#include "Biome.h"

class BiomeEnd : public Biome
{
  public:
    BiomeEnd(BiomeProperties properties);
    int32_t getSkyColorByTemp(float currentTemperature) override;
};
