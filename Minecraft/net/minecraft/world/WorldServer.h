#pragma once
#include "../entity/EnumCreatureType.h"
#include "../util/IThreadListener.h"
#include "NextTickListEntry.h"
#include "Teleporter.h"
#include "World.h"
#include "WorldEntitySpawner.h"

class IProgressUpdate;
class EntityPlayerMP;

class WorldServer : public World, virtual public IThreadListener
{
  public:
    bool disableLevelSaving;

    WorldServer(MinecraftServer *server, ISaveHandler *saveHandlerIn, WorldInfo &info, int dimensionId,
                Profiler &profilerIn);
    World *init() override;
    void tick() override;
    std::optional<SpawnListEntry> getSpawnListEntryForTypeAt(EnumCreatureType creatureType, BlockPos &pos);
    bool canCreatureTypeSpawnHere(EnumCreatureType creatureType, SpawnListEntry spawnListEntry, BlockPos &pos);
    void updateAllPlayersSleepingFlag() override;
    bool areAllPlayersAsleep();
    void setInitialSpawnLocation() override;
    bool isBlockTickPending(BlockPos &pos, Block *blockType) const override;
    bool isUpdateScheduled(BlockPos &pos, Block *blk);
    void scheduleUpdate(BlockPos &pos, Block *blockIn, int32_t delay);
    void updateBlockTick(BlockPos &pos, Block *blockIn, int32_t delay, int32_t priority);
    void scheduleBlockUpdate(BlockPos &pos, Block *blockIn, int32_t delay, int32_t priority) override;
    void updateEntities() override;
    void resetUpdateEntityTick();
    bool tickUpdates(bool runAllPending) override;
    std::optional<> getPendingBlockUpdates(Chunk &chunkIn, bool remove) override;
    std::optional<> getPendingBlockUpdates(StructureBoundingBox &structureBB, bool remove) override;
    void updateEntityWithOptionalForce(Entity *entityIn, bool forceUpdate) override;
    bool isBlockModifiable(EntityPlayer *player, BlockPos &pos) override;
    void initialize(WorldSettings &settings);
    bool spawnEntity(Entity *entityIn) override;
    void loadEntities(std::initializer_list<Entity *> entityCollection) override;
    bool canAddEntity(Entity *entityIn);
    bool addWeatherEffect(Entity *entityIn) override;
    void setEntityState(Entity *entityIn, std::byte state) override;
    ChunkProviderServer getChunkProvider() override;
    Explosion newExplosion(Entity *entityIn, double x, double y, double z, float strength, bool causesFire,
                           bool damagesTerrain) override;
    void addBlockEvent(BlockPos &pos, Block *blockIn, int32_t eventID, int32_t eventParam) override;
    void flush();

    MinecraftServer *getMinecraftServer() override;
    EntityTracker *getEntityTracker();
    PlayerChunkMap *getPlayerChunkMap();
    Teleporter *getDefaultTeleporter();
    TemplateManager getStructureTemplateManager();
    void spawnParticle(EnumParticleTypes particleType, double xCoord, double yCoord, double zCoord,
                       int32_t numberOfParticles, double xOffset, double yOffset, double zOffset, double particleSpeed,
                       std::initializer_list<int32_t> particleArguments);
    void spawnParticle(EnumParticleTypes particleType, bool longDistance, double xCoord, double yCoord, double zCoord,
                       int32_t numberOfParticles, double xOffset, double yOffset, double zOffset, double particleSpeed,
                       std::initializer_list<int32_t> particleArguments);
    void spawnParticle(EntityPlayerMP *player, EnumParticleTypes particle, bool longDistance, double x, double y,
                       double z, int32_t count, double xOffset, double yOffset, double zOffset, double speed,
                       std::initializer_list<int32_t> particleArguments);
    Entity *getEntityFromUuid(xg::Guid uuid);
    ListenableFuture addScheduledTask(Runnable runnableToSchedule);
    bool isCallingFromMinecraftThread() override;
    BlockPos findNearestStructure(std::string_view structureName, BlockPos &position, bool findUnexplored);
    AdvancementManager getAdvancementManager();
    FunctionManager getFunctionManager();

  protected:
    VillageSiege villageSiege = new VillageSiege(this);
    void wakeAllPlayers();
    bool isChunkLoaded(int32_t x, int32_t z, bool allowEmpty) override;
    void playerCheckLight();
    void updateBlocks() override;
    BlockPos adjustPosToNearbyEntity(BlockPos &pos);
    void tickPlayers() override;
    IChunkProvider createChunkProvider() override;
    void createBonusChest();
    std::optional<BlockPos> getSpawnCoordinate() const;
    void saveAllChunks(bool all, IProgressUpdate *progressCallback);
    void flushToDisk();
    virtual void saveLevel();
    void onEntityAdded(Entity *entityIn) override;
    void onEntityRemoved(Entity *entityIn) override;
    void updateWeather() override;

  private:
    static std::shared_ptr<spdlog::logger> LOGGER;
    MinecraftServer server;
    EntityTracker entityTracker;
    PlayerChunkMap playerChunkMap;
    std::unordered_set<NextTickListEntry> pendingTickListEntriesHashSet;
    std::set<NextTickListEntry> pendingTickListEntriesTreeSet;
    std::unordered_map<xg::Guid, Entity *> entitiesByUuid;
    bool allPlayersSleeping;
    int32_t updateEntityTick;
    Teleporter worldTeleporter;
    WorldEntitySpawner entitySpawner;
    ServerBlockEventList[] blockEventQueue = new WorldServer.ServerBlockEventList[]{
        new WorldServer.ServerBlockEventList(), new WorldServer.ServerBlockEventList()};
    int32_t blockEventCacheIndex;
    std::vector<NextTickListEntry> pendingTickListEntriesThisTick;

    void resetRainAndThunder() const;
    bool canSpawnNPCs();
    bool canSpawnAnimals();
    void setDebugWorldSettings();
    void createSpawnPosition(WorldSettings &settings);
    void sendQueuedBlockEvents();
    bool fireBlockEvent(BlockEventData event);

    void sendPacketWithinDistance(EntityPlayerMP *player, bool longDistance, double x, double y, double z,
                                  Packet &packetIn);
};
