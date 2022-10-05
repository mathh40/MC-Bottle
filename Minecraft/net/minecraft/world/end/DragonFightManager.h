#pragma once
#include "BossInfoServer.h"
#include "DragonSpawnManager.h"
#include "WorldServer.h"
#include "math/BlockPos.h"

class EntityEnderCrystal;
class EntityDragon;
class DragonSpawnManager;

namespace DamageSource
{
class DamageSource;
}

class DragonFightManager
{
  public:
    DragonFightManager(WorldServer *worldIn, NBTTagCompound *compound);
    std::unique_ptr<NBTTagCompound> getCompound();

    void tick();
    void processDragonDeath(EntityDragon *dragon);
    void dragonUpdate(EntityDragon *dragonIn);
    int32_t getNumAliveCrystals() const;
    void onCrystalDestroyed(EntityEnderCrystal *crystal, DamageSource::DamageSource &dmgSrc);
    bool hasPreviouslyKilledDragon() const;
    void respawnDragon();
    void resetSpikeCrystals();
    void setRespawnState(DragonSpawnManager state);

  private:
    static std::shared_ptr<spdlog::logger> LOGGER;
    BossInfoServer bossInfo;
    WorldServer *world;
    std::vector<int32_t> gateways;
    BlockPattern portalPattern;
    int ticksSinceDragonSeen;
    int aliveCrystals;
    int ticksSinceCrystalsScanned;
    int ticksSinceLastPlayerScan;
    bool dragonKilled;
    bool previouslyKilled;
    xg::Guid dragonUniqueId;
    bool scanForLegacyFight;
    std::optional<BlockPos> exitPortalLocation;
    DragonSpawnManager respawnState;
    int32_t respawnStateTicks;
    std::vector<Entity *> crystals;

    bool hasDragonBeenKilled();
    BlockPattern::PatternHelper findExitPortal();
    void loadChunks() const;
    void updatePlayers();
    void findAliveCrystals();
    void spawnNewGateway();
    void generateGateway(BlockPos &pos);
    void generatePortal(bool active);
    std::unique_ptr<EntityDragon> createNewDragon();
    void respawnDragon(std::vector<Entity *> crystalsIn);
};
