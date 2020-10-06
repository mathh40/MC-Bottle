#pragma once
#include "Entity.h"
#include "../../../../spdlog/include/spdlog/logger.h"

#include <memory>

class EntityTrackerEntry {
public:
    EntityTrackerEntry(Entity* entityIn, int32_t rangeIn, int32_t maxRangeIn, int32_t updateFrequencyIn, bool sendVelocityUpdatesIn);
    void updatePlayerList(const std::vector<EntityPlayerMP*>& players);
    void sendPacketToTrackedPlayers(Packet packetIn);
    void sendToTrackingAndSelf(Packet packetIn);
    void sendDestroyEntityPacketToTrackedPlayers();
    void removeFromTrackedPlayers(EntityPlayerMP* playerMP);
    void updatePlayerEntity(EntityPlayerMP* playerMP);
    bool isVisibleTo(EntityPlayerMP* playerMP) const;
    void updatePlayerEntities(std::span<EntityPlayer*> players);
    void removeTrackedPlayerSymmetric(EntityPlayerMP* playerMP);
    Entity* getTrackedEntity() const;
    void setMaxRange(int32_t maxRangeIn);
    void resetPlayerVisibility();


    friend bool operator==(const EntityTrackerEntry& lhs, const EntityTrackerEntry& rhs);

    int32_t updateCounter;
    bool playerEntitiesUpdated;
private:
    void sendMetadata();
    bool isPlayerWatchingThisChunk(EntityPlayerMP* playerMP) const;
    Packet createSpawnPacket();



   static std::shared_ptr<spdlog::logger> LOGGER;
   Entity* trackedEntity;
   int32_t range;
   int32_t maxRange;
   int32_t updateFrequency;
   int64_t encodedPosX;
   int64_t encodedPosY;
   int64_t encodedPosZ;
   int32_t encodedRotationYaw;
   int32_t encodedRotationPitch;
   int32_t lastHeadMotion;
   double lastTrackedEntityMotionX;
   double lastTrackedEntityMotionY;
   double motionZ;
   double lastTrackedEntityPosX;
   double lastTrackedEntityPosY;
   double lastTrackedEntityPosZ;
   bool updatedPlayerVisibility;
   bool sendVelocityUpdates;
   int32_t ticksSinceLastForcedTeleport;
   std::vector<Entity*> passengers;
   bool ridingEntity;
   bool onGround;
   std::unordered_set<EntityPlayerMP*> trackingPlayers;
};

namespace std
{
    template<> struct hash<EntityTrackerEntry>
    {
        std::size_t operator()(EntityTrackerEntry const& s) const noexcept
        {
            return s.trackedEntity->getEntityId();
        }
    };
}
