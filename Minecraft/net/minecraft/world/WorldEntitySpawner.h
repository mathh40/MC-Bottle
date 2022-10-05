#pragma once
#include "../util/math/ChunkPos.h"
#include "../util/math/MathHelper.h"

#include <unordered_set>

class IBlockState;
class World;
class WorldServer;
class WorldEntitySpawner
{
  public:
    int32_t findChunksForSpawning(WorldServer &worldServerIn, bool spawnHostileMobs, bool spawnPeacefulMobs,
                                  bool spawnOnSetTickRate);
    bool isValidEmptySpawnBlock(IBlockState *state);
    bool canCreatureTypeSpawnAtLocation(SpawnPlacementType spawnPlacementTypeIn, World *worldIn, BlockPos pos);
    void performWorldGenSpawning(World *worldIn, Biome &biomeIn, int32_t centerX, int32_t centerZ, int32_t diameterX,
                                 int32_t diameterZ, pcg32 &randomIn);

  private:
    BlockPos getRandomChunkPosition(World *worldIn, int32_t x, int32_t z) const;

    static int32_t MOB_COUNT_DIV;
    std::unordered_set<ChunkPos> eligibleChunksForSpawning;
};
