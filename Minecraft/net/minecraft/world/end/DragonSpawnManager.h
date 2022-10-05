#pragma once
#include "DragonFightManager.h"
#include "WorldServer.h"

class DragonSpawnManager
{
  public:
    using processFunc = void(WorldServer *worldIn, DragonFightManager &manager,
                             std::vector<EntityEnderCrystal *> crystals, int32_t ticks, BlockPos &pos);
    void process(WorldServer *worldIn, DragonFightManager &manager, std::vector<EntityEnderCrystal *> crystals,
                 int32_t ticks, BlockPos &pos);
    static DragonSpawnManager START;
    static DragonSpawnManager PREPARING_TO_SUMMON_PILLARS;
    static DragonSpawnManager SUMMONING_PILLARS;
    static DragonSpawnManager SUMMONING_DRAGON;
    static DragonSpawnManager END;

  private:
    DragonSpawnManager(processFunc *prozessIn);
    processFunc *prozessfunc;
};
