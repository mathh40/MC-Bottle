#pragma once
#include "biome/Biome.h"

#include <cstdint>
#include <optional>
class BlockPos;
class Chunk;
class World;

class IChunkGenerator
{
  public:
    virtual ~IChunkGenerator()                              = default;
    virtual Chunk generateChunk(int32_t var1, int32_t var2) = 0;

    virtual void populate(int32_t var1, int32_t var2) = 0;

    virtual bool generateStructures(Chunk &var1, int32_t var2, int32_t var3) = 0;

    virtual std::vector<SpawnListEntry> getPossibleCreatures(EnumCreatureType var1, BlockPos &var2) = 0;

    virtual std::optional<BlockPos> getNearestStructurePos(World *var1, std::string_view var2, BlockPos &var3,
                                                           bool var4) = 0;

    virtual void recreateStructures(Chunk &var1, int32_t var2, int32_t var3) = 0;

    virtual bool isInsideStructure(World *var1, std::string_view var2, BlockPos &var3) = 0;

  private:
};
