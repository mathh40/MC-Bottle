#include "WorldServerDemo.h"

uint64_t WorldServerDemo::DEMO_WORLD_SEED = Util::getStringHash("North Carolina");

WorldServerDemo::WorldServerDemo(MinecraftServer *server, ISaveHandler saveHandlerIn, WorldInfo worldInfoIn,
                                 int32_t dimensionId, Profiler profilerIn)
    : WorldServer(server, saveHandlerIn, worldInfoIn, dimensionId, profilerIn)
{

    worldInfo.populateFromWorldSettings( : DEMO_WORLD_SETTINGS);
}
