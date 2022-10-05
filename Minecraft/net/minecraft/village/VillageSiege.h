#include "Village.h"

class VillageSiege
{
  public:
    VillageSiege(World *worldIn);
    void tick();

  private:
    bool trySetupSiege();
    bool spawnZombie();
    std::optional<Vec3d> findRandomSpawnPos(BlockPos pos);

    World *world;
    bool hasSetupSiege;
    int32_t siegeState = -1;
    int32_t siegeCount;
    int32_t nextSpawnTime;
    Village village;
    int32_t spawnX;
    int32_t spawnY;
    int32_t spawnZ;
};
