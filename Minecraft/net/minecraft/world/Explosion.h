#pragma once
#include "math/BlockPos.h"


#include <unordered_map>
#include <vector>

class EntityLivingBase;
class EntityPlayer;
class Entity;
class World;

class Explosion
{
public:
	Explosion(World* worldIn, Entity* entityIn, double x, double y, double z, float size, std::vector<BlockPos> affectedPositions);
	Explosion(World* worldIn, Entity* entityIn, double x, double y, double z, float size, bool causesFire, bool damagesTerrain, std::vector<BlockPos> affectedPositions);
	Explosion(World* worldIn, Entity* entityIn, double x, double y, double z, float size, bool causesFire, bool damagesTerrain);

	void doExplosionA();
	void doExplosionB(bool spawnParticles);
    std::unordered_map<EntityPlayer*,Vec3d>& getPlayerKnockbackMap();
	EntityLivingBase* getExplosivePlacedBy();
	void clearAffectedBlockPositions();
	std::vector<BlockPos> getAffectedBlockPositions() const;
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
    std::unordered_map<EntityPlayer*,Vec3d> playerKnockbackMap;
};
