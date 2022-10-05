#pragma once
#include "MapGenStructure.h"
#include "biome/Biome.h"

class MapGenStronghold : public MapGenStructure
{
  public:
    MapGenStronghold();
    explicit MapGenStronghold(std::unordered_map<std::string, std::string> p_i2068_1_);
    std::string getStructureName() override;
    std::optional<BlockPos> getNearestStructurePos(World *worldIn, BlockPos &pos, bool findUnexplored) override;

    class Start : public StructureStart
    {
      public:
        Start() = default;
        Start(World *worldIn, pcg32 &random, int32_t chunkX, int32_t chunkZ);
    };

  protected:
    bool canSpawnStructureAtCoords(int32_t chunkX, int32_t chunkZ) override;
    StructureStart getStructureStart(int32_t chunkX, int32_t chunkZ) override;

  private:
    std::vector<Biome *> allowedBiomes;
    bool ranBiomeCheck;
    std::vector<ChunkPos> structureCoords;
    double distance;
    int32_t spread;

    void generatePositions();
};
