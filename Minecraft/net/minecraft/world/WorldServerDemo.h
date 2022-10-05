#pragma once
#include "WorldServer.h"

class WorldServerDemo : public WorldServer
{
  public:
    static WorldSettings DEMO_WORLD_SETTINGS;
    WorldServerDemo(MinecraftServer *server, ISaveHandler saveHandlerIn, WorldInfo worldInfoIn, int32_t dimensionId,
                    Profiler profilerIn);

  private:
    static uint64_t DEMO_WORLD_SEED;
};
