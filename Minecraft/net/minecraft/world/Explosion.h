#pragma once
#include "../util/EntitySelectors.h"
#include "World.h"

class Explosion
{
public:
	Explosion(World* worldIn, Entity* entityIn, double x, double y, double z, float size, std::vector<BlockPos> affectedPositions);
	Explosion(World* worldIn, Entity* entityIn, double x, double y, double z, float size, bool causesFire, bool damagesTerrain, std::vector<BlockPos> affectedPositions);
	Explosion(World* worldIn, Entity* entityIn, double x, double y, double z, float size, bool causesFire, bool damagesTerrain);

	void doExplosionA();
	void doExplosionB(bool spawnParticles);
	Map getPlayerKnockbackMap();
	EntityLivingBase* getExplosivePlacedBy();
	void clearAffectedBlockPositions();
	std::vector<BlockPos> getAffectedBlockPositions();
private:
	bool causesFire;
	bool damagesTerrain;
	World* world;
	double x;
	double y;
	double z;
	Entity* exploder;
	float size;
	std::vector<BlockPos> affectedBlockPositions;
	Map playerKnockbackMap;
};
