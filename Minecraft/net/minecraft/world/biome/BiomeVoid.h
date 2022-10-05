#pragma once
#include "Biome.h"

class BiomeVoid : public Biome
{
  public:
    BiomeVoid(BiomeProperties properties);
    bool ignorePlayerSpawnSuitability() const override;

  private:
};
