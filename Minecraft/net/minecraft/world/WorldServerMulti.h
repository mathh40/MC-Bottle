#include "WorldServer.h"

class WorldServerMulti :public WorldServer
{
public:
	WorldServerMulti(MinecraftServer* server, ISaveHandler saveHandlerIn, int32_t dimensionId, WorldServer* delegate, Profiler profilerIn);
	World* init() override;
	void saveAdditionalData();
protected:
	void saveLevel() override;
private: WorldServer* delegate;
};
