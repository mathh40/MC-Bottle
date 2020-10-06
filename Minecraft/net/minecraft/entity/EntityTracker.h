#pragma once
#include "spdlog/logger.h"


#include <cstdint>
#include <memory>
#include <unordered_map>
#include <unordered_set>

class Packet;
class Chunk;
class EntityPlayerMP;
class Entity;
class WorldServer;
class EntityTrackerEntry;
class EntityTracker {

public:
    EntityTracker(WorldServer* theWorldIn);
    static int64_t getPositionLong(double value);
    static void updateServerPosition(Entity* entityIn, double x, double y, double z);
    void track(Entity* entityIn);
    void track(Entity* entityIn, int32_t trackingRange, int32_t updateFrequency);
    void track(Entity* entityIn, int32_t trackingRange, int32_t updateFrequency, bool sendVelocityUpdates);
    void untrack(Entity* entityIn);
    void tick();
    void updateVisibility(EntityPlayerMP* player);
    void sendToTracking(Entity* entityIn, Packet packetIn);
    void sendToTrackingAndSelf(Entity* entityIn, Packet packetIn);
    void removePlayerFromTrackers(EntityPlayerMP* player);
    void sendLeashedEntitiesInChunk(EntityPlayerMP* player, const Chunk& chunkIn);
    void setViewDistance(int32_t distance);

private:
    static std::shared_ptr<spdlog::logger> LOGGER;
    WorldServer* world;
    std::unordered_set<EntityTrackerEntry> entries;
    std::unordered_map<int32_t,EntityTrackerEntry> trackedEntityHashTable;
    int32_t maxTrackingDistanceThreshold;
};
