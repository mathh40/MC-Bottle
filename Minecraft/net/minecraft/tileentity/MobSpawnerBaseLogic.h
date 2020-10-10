#include "EntitySelectors.h"
#include "WeightedSpawnerEntity.h"
#include "ResourceLocation.h"
#include "../world/World.h"

class MobSpawnerBaseLogic
{
public:
	void setEntityId(std::optional<ResourceLocation> id);
	void updateSpawner();
	void readFromNBT(NBTTagCompound* nbt);
	NBTTagCompound* writeToNBT(NBTTagCompound* p_189530_1_);
	Entity* getCachedEntity();
	bool setDelayToMin(int32_t delay);
	void setNextSpawnData(WeightedSpawnerEntity p_184993_1_);
	double getMobRotation() const;
	double getPrevMobRotation() const;
	
	virtual void broadcastEvent(std::byte var1) = 0;
	virtual World* getSpawnerWorld() const = 0;
	virtual BlockPos getSpawnerPosition() const = 0;
	
private:
	std::optional<ResourceLocation> getEntityId();
	bool isActivated();
	void resetTimer();
	
	int32_t spawnDelay = 20;
	std::vector<WeightedSpawnerEntity> potentialSpawns;
	WeightedSpawnerEntity spawnData;
	double mobRotation;
	double prevMobRotation;
	int32_t minSpawnDelay = 200;
	int32_t maxSpawnDelay = 800;
	int32_t spawnCount = 4;
	Entity* cachedEntity;
	int32_t maxNearbyEntities = 6;
	int32_t activatingRangeFromPlayer = 16;
	int32_t spawnRange = 4;
};
