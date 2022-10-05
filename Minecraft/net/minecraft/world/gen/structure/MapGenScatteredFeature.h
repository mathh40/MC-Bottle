#pragma once
#include "MapGenStructure.h"

class MapGenScatteredFeature : public MapGenStructure
{
  private:
    static std::vector<Biome *> BIOMELIST;
    std::vector<> monsters{};
    int32_t maxDistanceBetweenScatteredFeatures;
    int32_t minDistanceBetweenScatteredFeatures;

  protected:
    bool canSpawnStructureAtCoords(int32_t chunkX, int32_t chunkZ) override;
    StructureStart getStructureStart(int32_t chunkX, int32_t chunkZ) override;

  public:
    MapGenScatteredFeature();
    explicit MapGenScatteredFeature(const std::unordered_map<std::string, std::string> &p_i2061_1_);
    std::string getStructureName() override;
    std::optional<BlockPos> getNearestStructurePos(World *worldIn, BlockPos &pos, bool findUnexplored) override;
    bool isSwampHut(BlockPos &pos);
    std::vector<> getMonsters();

    class Start : public StructureStart
    {
      public:
        Start() = default;
        Start(World *worldIn, pcg32 &random, int32_t chunkX, int32_t chunkZ);
        Start(World *worldIn, pcg32 &random, int32_t chunkX, int32_t chunkZ, Biome *biomeIn);
    };
};
