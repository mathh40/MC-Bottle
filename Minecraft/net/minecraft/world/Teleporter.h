#pragma once
#include "../../../../pcg-cpp/pcg_random.hpp"
#include <unordered_map>
#include "../util/math/BlockPos.h"

class Entity;

class PortalPosition :public BlockPos
{
public:
	int64_t lastUpdateTime;

	PortalPosition(BlockPos pos, int64_t lastUpdate)
		:BlockPos(pos)
	{
		lastUpdateTime = lastUpdate;
	}
};

class Teleporter
{
public:
	Teleporter(WorldServer& worldIn);
	void placeInPortal(Entity* entityIn, float rotationYaw);
	bool placeInExistingPortal(Entity* entityIn, float rotationYaw);
	bool makePortal(Entity* entityIn);
	void removeStalePortalLocations(int64_t worldTime);
private:
	WorldServer& world;
	pcg32 random;
	std::pmr::unordered_map<int64_t, PortalPosition> destinationCoordinateCache;
};
