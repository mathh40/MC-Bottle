#pragma once
#include "Biome.h"

class BiomeOcean : public Biome
{
  public:
    BiomeOcean(BiomeProperties properties);
    TempCategory getTempCategory() override;
};
