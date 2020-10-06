#include "EntityTracker.h"

#include "Entity.h"
#include "EntityLiving.h"
#include "EntityTrackerEntry.h"
#include "ReportedException.h"

std::shared_ptr<spdlog::logger> EntityTracker::LOGGER = spdlog::get("Minecraft")->clone("EntityTracker");

class EntityTrackerCrashReportDetail: ICrashReportDetail {
    public:
        EntityTrackerCrashReportDetail(int32_t updateFrequencyIn)
    :updateFrequency(updateFrequencyIn){
            
        }

    std::string call(){
               std::string s = "Once per " + updateFrequency + " ticks";
               if (updateFrequency == std::numeric_limits<int32_t>::max()) {
                  s = "Maximum (" + s + ")";
               }

               return s;
            }

private:
    int32_t updateFrequency;
};


EntityTracker::EntityTracker(WorldServer *theWorldIn)
    :world(theWorldIn),maxTrackingDistanceThreshold(theWorldIn->getMinecraftServer()->getPlayerList().getEntityViewDistance())
{
}

int64_t EntityTracker::getPositionLong(double value) {
    return MathHelper::lfloor(value * 4096.0);
}

void EntityTracker::updateServerPosition(Entity *entityIn, double x, double y, double z) {
    entityIn->serverPosX = getPositionLong(x);
    entityIn->serverPosY = getPositionLong(y);
    entityIn->serverPosZ = getPositionLong(z);
}

void EntityTracker::track(Entity *entityIn) {
    if (Util::instanceof<EntityPlayerMP>(entityIn)) {
         track(entityIn, 512, 2);
         auto entityplayermp = (EntityPlayerMP*)entityIn;
         for(auto entitytrackerentry : entries){
            if (entitytrackerentry.getTrackedEntity() != entityplayermp) {
               entitytrackerentry.updatePlayerEntity(entityplayermp);
            }
         }
      } else if (entityIn instanceof EntityFishHook) {
         track(entityIn, 64, 5, true);
      } else if (entityIn instanceof EntityArrow) {
         track(entityIn, 64, 20, false);
      } else if (entityIn instanceof EntitySmallFireball) {
         track(entityIn, 64, 10, false);
      } else if (entityIn instanceof EntityFireball) {
         track(entityIn, 64, 10, true);
      } else if (entityIn instanceof EntitySnowball) {
         track(entityIn, 64, 10, true);
      } else if (entityIn instanceof EntityLlamaSpit) {
         track(entityIn, 64, 10, false);
      } else if (entityIn instanceof EntityEnderPearl) {
         track(entityIn, 64, 10, true);
      } else if (entityIn instanceof EntityEnderEye) {
         track(entityIn, 64, 4, true);
      } else if (entityIn instanceof EntityEgg) {
         track(entityIn, 64, 10, true);
      } else if (entityIn instanceof EntityPotion) {
         track(entityIn, 64, 10, true);
      } else if (entityIn instanceof EntityExpBottle) {
         track(entityIn, 64, 10, true);
      } else if (entityIn instanceof EntityFireworkRocket) {
         track(entityIn, 64, 10, true);
      } else if (entityIn instanceof EntityItem) {
         track(entityIn, 64, 20, true);
      } else if (entityIn instanceof EntityMinecart) {
         track(entityIn, 80, 3, true);
      } else if (entityIn instanceof EntityBoat) {
         track(entityIn, 80, 3, true);
      } else if (entityIn instanceof EntitySquid) {
         track(entityIn, 64, 3, true);
      } else if (entityIn instanceof EntityWither) {
         track(entityIn, 80, 3, false);
      } else if (entityIn instanceof EntityShulkerBullet) {
         track(entityIn, 80, 3, true);
      } else if (entityIn instanceof EntityBat) {
         track(entityIn, 80, 3, false);
      } else if (entityIn instanceof EntityDragon) {
         track(entityIn, 160, 3, true);
      } else if (entityIn instanceof IAnimals) {
         track(entityIn, 80, 3, true);
      } else if (entityIn instanceof EntityTNTPrimed) {
         track(entityIn, 160, 10, true);
      } else if (entityIn instanceof EntityFallingBlock) {
         track(entityIn, 160, 20, true);
      } else if (entityIn instanceof EntityHanging) {
         track(entityIn, 160, std::numeric_limits<int32_t>::max(), false);
      } else if (entityIn instanceof EntityArmorStand) {
         track(entityIn, 160, 3, true);
      } else if (entityIn instanceof EntityXPOrb) {
         track(entityIn, 160, 20, true);
      } else if (entityIn instanceof EntityAreaEffectCloud) {
         track(entityIn, 160, std::numeric_limits<int32_t>::max(), true);
      } else if (entityIn instanceof EntityEnderCrystal) {
         track(entityIn, 256, std::numeric_limits<int32_t>::max(), false);
      } else if (entityIn instanceof EntityEvokerFangs) {
         track(entityIn, 160, 2, false);
      }
}

void EntityTracker::track(Entity *entityIn, int32_t trackingRange, int32_t updateFrequency) {
    track(entityIn, trackingRange, updateFrequency, false);
}

void EntityTracker::track(Entity *entityIn, int32_t trackingRange, int32_t updateFrequency, bool sendVelocityUpdates) {
    try {
         if (trackedEntityHashTable.contains(entityIn->getEntityId())) {
            throw  std::logic_error("Entity is already tracked!");
         }

         EntityTrackerEntry entitytrackerentry(entityIn, trackingRange, maxTrackingDistanceThreshold, updateFrequency, sendVelocityUpdates);
         entries.emplace(entitytrackerentry);
         trackedEntityHashTable.emplace(entityIn->getEntityId(), entitytrackerentry);
         entitytrackerentry.updatePlayerEntities(world->playerEntities);
      } catch (std::exception& var10) {
         CrashReport crashreport = CrashReport.makeCrashReport(var10, "Adding entity to track");
         CrashReportCategory crashreportcategory = crashreport.makeCategory("Entity To Track");
         crashreportcategory.addCrashSection("Tracking range", trackingRange + " blocks");
         crashreportcategory.addDetail("Update interval", EntityTrackerCrashReportDetail(updateFrequency));
         entityIn->addEntityCrashInfo(crashreportcategory);
         (trackedEntityHashTable[entityIn->getEntityId()]).getTrackedEntity().addEntityCrashInfo(crashreport.makeCategory("Entity That Is Already Tracked"));

         try {
            throw ReportedException(crashreport);
         } catch (ReportedException& var9) {
            LOGGER->error("\"Silently\" catching entity tracking error.", var9);
         }
      }
}

void EntityTracker::untrack(Entity *entityIn) {
    if (Util::instanceof<EntityPlayerMP>(entityIn)) {
         auto entityplayermp = (EntityPlayerMP*)entityIn;
        for(auto entitytrackerentry : entries){
            entitytrackerentry.removeFromTrackedPlayers(entityplayermp);
         }
      }

    auto ite = trackedEntityHashTable.find(entityIn->getEntityId());
      auto& entitytrackerentry1 = ite->second;
      if (entitytrackerentry1 != nullptr) {
         entries.erase(entitytrackerentry1);
         entitytrackerentry1.sendDestroyEntityPacketToTrackedPlayers();
      }
    trackedEntityHashTable.erase(ite);
}

void EntityTracker::tick() {
    std::vector<EntityPlayerMP*> list;

    for(auto entitytrackerentry : entries){
         entitytrackerentry.updatePlayerList(world->playerEntities);
         if (entitytrackerentry.playerEntitiesUpdated) {
             std::vector<EntityPlayerMP *>::const_iterator entity = entitytrackerentry.getTrackedEntity();
            if (Util::instanceof<EntityPlayerMP>(entity)) {
               list.emplace(entity);
            }
         }
      }

    for(auto entityplayermp : list){
         for(auto entitytrackerentry1 : entries) {
            if (entitytrackerentry1.getTrackedEntity() != entityplayermp) {
               entitytrackerentry1.updatePlayerEntity(entityplayermp);
            }
         }
      }
}

void EntityTracker::updateVisibility(EntityPlayerMP *player) {
      for(auto entitytrackerentry : entries) {
         if (entitytrackerentry.getTrackedEntity() == player) {
            entitytrackerentry.updatePlayerEntities(world->playerEntities);
         } else {
            entitytrackerentry.updatePlayerEntity(player);
         }
      }
}

void EntityTracker::sendToTracking(Entity *entityIn, Packet packetIn) {
    auto& entitytrackerentry = trackedEntityHashTable.find(entityIn->getEntityId())->second;
      if (entitytrackerentry != nullptr) {
         entitytrackerentry.sendPacketToTrackedPlayers(packetIn);
      }
}

void EntityTracker::sendToTrackingAndSelf(Entity *entityIn, Packet packetIn) {
    auto& entitytrackerentry = trackedEntityHashTable.find(entityIn->getEntityId())->second;
      if (entitytrackerentry != nullptr) {
         entitytrackerentry.sendToTrackingAndSelf(packetIn);
      }
}

void EntityTracker::removePlayerFromTrackers(EntityPlayerMP *player) {
    for (auto entitytrackerentry : entries){
         entitytrackerentry.removeTrackedPlayerSymmetric(player);
      }
}

void EntityTracker::sendLeashedEntitiesInChunk(EntityPlayerMP *player, const Chunk &chunkIn) {
    std::vector<Entity*> list;
      std::vector<Entity*> list1;

    for(auto entitytrackerentry : entries){
         Entity* entity = entitytrackerentry.getTrackedEntity();
         if (entity != player && entity->chunkCoordX == chunkIn.x && entity->chunkCoordZ == chunkIn.z) {
            entitytrackerentry.updatePlayerEntity(player);
            if (Util::instanceof<EntityLiving>(entity) && ((EntityLiving*)entity)->getLeashHolder() != nullptr) {
               list.emplace(entity);
            }

            if (!entity->getPassengers().empty()) {
               list1.emplace(entity);
            }
         }
      }

      if (!list.empty()) {
         for(auto entity2 : list){
            player->connection.sendPacket(new SPacketEntityAttach(entity2, ((EntityLiving*)entity2)->getLeashHolder()));
         }
      }

      if (!list1.empty()) {
        for(auto entity2 : list1) {
            player->connection.sendPacket(new SPacketSetPassengers(entity2));
         }
      }
}

void EntityTracker::setViewDistance(int32_t distance) {
    maxTrackingDistanceThreshold = (distance - 1) * 16;
      for(auto entitytrackerentry : entries) {
         entitytrackerentry.setMaxRange(maxTrackingDistanceThreshold);
      }
}
