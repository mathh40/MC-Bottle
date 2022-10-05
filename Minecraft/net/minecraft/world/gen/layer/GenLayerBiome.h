#pragma once
#include "GenLayer.h"
#include "biome/Biome.h"
#include "gen/ChunkGeneratorSettings.h"

class WorldType;

class GenLayerBiome : public GenLayer
{
  public:
    GenLayerBiome(int64_t p_i45560_1_, std::shared_ptr<GenLayer> p_i45560_3_, WorldType p_i45560_4_,
                  ChunkGeneratorSettings p_i45560_5_);
    std::vector<int32_t> getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight) override;

  private:
    std::vector<Biome *> warmBiomes;
    std::vector<Biome *> mediumBiomes;
    std::vector<Biome *> coldBiomes;
    std::vector<Biome *> iceBiomes;
    std::optional<ChunkGeneratorSettings> settings;
};
