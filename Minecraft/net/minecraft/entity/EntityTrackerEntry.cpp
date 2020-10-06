#include "EntityTrackerEntry.h"


#include "EntityAgeable.h"
#include "EntityLeashKnot.h"
#include "EntityTracker.h"
#include "../item/ItemMap.h"
#include "ai/attributes/AttributeMap.h"
#include "storage/MapData.h"

std::shared_ptr<spdlog::logger> EntityTrackerEntry::LOGGER = spdlog::get("Minecraft")->clone("EntityTrackerEntry");

EntityTrackerEntry::EntityTrackerEntry(Entity *entityIn, int32_t rangeIn, int32_t maxRangeIn, int32_t updateFrequencyIn,
    bool sendVelocityUpdatesIn):
    trackedEntity(entityIn), range(rangeIn), maxRange(maxRangeIn), updateFrequency(updateFrequencyIn), sendVelocityUpdates(sendVelocityUpdatesIn),
    encodedPosX(EntityTracker::getPositionLong(entityIn->posX)), encodedPosY(EntityTracker::getPositionLong(entityIn->posY)), encodedPosZ(EntityTracker::getPositionLong(entityIn->posZ)),
    encodedRotationYaw(MathHelper::floor(entityIn->rotationYaw * 256.0F / 360.0F)), encodedRotationPitch(MathHelper::floor(entityIn->rotationPitch * 256.0F / 360.0F)),
    lastHeadMotion(MathHelper::floor(entityIn->getRotationYawHead() * 256.0F / 360.0F)), onGround(entityIn->onGround){
}

void EntityTrackerEntry::updatePlayerList(const std::vector<EntityPlayerMP*> &players) {
    playerEntitiesUpdated = false;
      if (!updatedPlayerVisibility || trackedEntity->getDistanceSq(lastTrackedEntityPosX, lastTrackedEntityPosY, lastTrackedEntityPosZ) > 16.0) {
         lastTrackedEntityPosX = trackedEntity->posX;
         lastTrackedEntityPosY = trackedEntity->posY;
         lastTrackedEntityPosZ = trackedEntity->posZ;
         updatedPlayerVisibility = true;
         playerEntitiesUpdated = true;
         updatePlayerEntities(players);
      }

      auto list = trackedEntity->getPassengers();
      if (!(list == passengers)) {
         passengers = list;
         sendPacketToTrackedPlayers(new SPacketSetPassengers(trackedEntity));
      }

      if (Util::instanceof<EntityItemFrame>(trackedEntity) && updateCounter % 10 == 0) {
         auto entityitemframe = (EntityItemFrame*)trackedEntity;
         ItemStack itemstack = entityitemframe->getDisplayedItem();
         if (Util::instanceof<ItemMap>(itemstack.getItem())) {
            MapData mapdata = Items::FILLED_MAP.getMapData(itemstack, trackedEntity->world);
            Iterator var6 = players.iterator();

            while(var6.hasNext()) {
               EntityPlayer* entityplayer = (EntityPlayer)var6.next();
               EntityPlayerMP* entityplayermp = (EntityPlayerMP)entityplayer;
               mapdata.updateVisiblePlayers(entityplayermp, itemstack);
               Packet packet = Items::FILLED_MAP::createMapDataPacket(itemstack, trackedEntity->world, entityplayermp);
               if (packet != nullptr) {
                  entityplayermp->connection.sendPacket(packet);
               }
            }
         }

         sendMetadata();
      }

      if (updateCounter % updateFrequency == 0 || trackedEntity->isAirBorne || trackedEntity->getDataManager().isDirty()) {
         int32_t k1 = 0;
         if (trackedEntity->isRiding()) {
            auto k1 = MathHelper::floor(trackedEntity->rotationYaw * 256.0F / 360.0F);
            auto l1 = MathHelper::floor(trackedEntity->rotationPitch * 256.0F / 360.0F);
            bool flag3 = MathHelper::abs(k1 - encodedRotationYaw) >= 1 || MathHelper::abs(l1 - encodedRotationPitch) >= 1;
            if (flag3) {
               sendPacketToTrackedPlayers(new SPacketEntity.S16PacketEntityLook(trackedEntity->getEntityId(), (std::byte)k1, (std::byte)l1, trackedEntity->onGround));
               encodedRotationYaw = k1;
               encodedRotationPitch = l1;
            }

            encodedPosX = EntityTracker::getPositionLong(trackedEntity->posX);
            encodedPosY = EntityTracker::getPositionLong(trackedEntity->posY);
            encodedPosZ = EntityTracker::getPositionLong(trackedEntity->posZ);
            sendMetadata();
            ridingEntity = true;
         } else {
            ++ticksSinceLastForcedTeleport;
            auto i1 = EntityTracker::getPositionLong(trackedEntity->posX);
            auto i2 = EntityTracker::getPositionLong(trackedEntity->posY);
            auto j2 = EntityTracker::getPositionLong(trackedEntity->posZ);
            auto k2 = MathHelper::floor(trackedEntity->rotationYaw * 256.0F / 360.0F);
            auto i = MathHelper::floor(trackedEntity->rotationPitch * 256.0F / 360.0F);
            auto j = i1 - encodedPosX;
            auto k = i2 - encodedPosY;
            auto l = j2 - encodedPosZ;
            Packet packet1 = nullptr;
            bool flag = j * j + k * k + l * l >= 128L || updateCounter % 60 == 0;
            bool flag1 = MathHelper::abs(k2 - encodedRotationYaw) >= 1 || MathHelper::abs(i - encodedRotationPitch) >= 1;
            if (updateCounter > 0 || Util::instanceof<EntityArrow>(trackedEntity)) {
               if (j >= -32768L && j < 32768L && k >= -32768L && k < 32768L && l >= -32768L && l < 32768L && ticksSinceLastForcedTeleport <= 400 && !ridingEntity && onGround == trackedEntity->onGround) {
                  if ((!flag || !flag1) && !(Util::instanceof<EntityArrow>(trackedEntity))) {
                     if (flag) {
                        packet1 = new SPacketEntity.S15PacketEntityRelMove(trackedEntity->getEntityId(), j, k, l, trackedEntity->onGround);
                     } else if (flag1) {
                        packet1 = new SPacketEntity.S16PacketEntityLook(trackedEntity->getEntityId(), (std::byte)k2, (std::byte)i, trackedEntity->onGround);
                     }
                  } else {
                     packet1 = new SPacketEntity.S17PacketEntityLookMove(trackedEntity->getEntityId(), j, k, l, (std::byte)k2, (std::byte)i, trackedEntity->onGround);
                  }
               } else {
                  onGround = trackedEntity->onGround;
                  ticksSinceLastForcedTeleport = 0;
                  resetPlayerVisibility();
                  packet1 = new SPacketEntityTeleport(trackedEntity);
               }
            }

            bool flag2 = sendVelocityUpdates;
            if (Util::instanceof<EntityLivingBase>(trackedEntity) && ((EntityLivingBase*)trackedEntity)->isElytraFlying()) {
               flag2 = true;
            }

            if (flag2 && updateCounter > 0) {
               auto d0 = trackedEntity->motionX - lastTrackedEntityMotionX;
               auto d1 = trackedEntity->motionY - lastTrackedEntityMotionY;
               auto d2 = trackedEntity->motionZ - motionZ;
               auto d3 = 0.02;
               auto d4 = d0 * d0 + d1 * d1 + d2 * d2;
               if (d4 > 4.0E-4 || d4 > 0.0 && trackedEntity->motionX == 0.0 && trackedEntity->motionY == 0.0 && trackedEntity->motionZ == 0.0) {
                  lastTrackedEntityMotionX = trackedEntity->motionX;
                  lastTrackedEntityMotionY = trackedEntity->motionY;
                  motionZ = trackedEntity->motionZ;
                  sendPacketToTrackedPlayers(new SPacketEntityVelocity(trackedEntity->getEntityId(), lastTrackedEntityMotionX, lastTrackedEntityMotionY, motionZ));
               }
            }

            if (packet1 != nullptr) {
               sendPacketToTrackedPlayers((Packet)packet1);
            }

            sendMetadata();
            if (flag) {
               encodedPosX = i1;
               encodedPosY = i2;
               encodedPosZ = j2;
            }

            if (flag1) {
               encodedRotationYaw = k2;
               encodedRotationPitch = i;
            }

            ridingEntity = false;
         }

         k1 = MathHelper::floor(trackedEntity->getRotationYawHead() * 256.0F / 360.0F);
         if (MathHelper::abs(k1 - lastHeadMotion) >= 1) {
            sendPacketToTrackedPlayers(new SPacketEntityHeadLook(trackedEntity, (std::byte)k1));
            lastHeadMotion = k1;
         }

         trackedEntity->isAirBorne = false;
      }

      ++updateCounter;
      if (trackedEntity->velocityChanged) {
         sendToTrackingAndSelf(new SPacketEntityVelocity(trackedEntity));
         trackedEntity->velocityChanged = false;
      }
}

void EntityTrackerEntry::sendPacketToTrackedPlayers(Packet packetIn) {
    for(auto entityplayermp : trackingPlayers) {
         entityplayermp->connection.sendPacket(packetIn);
      }
}

void EntityTrackerEntry::sendToTrackingAndSelf(Packet packetIn) {
    sendPacketToTrackedPlayers(packetIn);
      if (Util::instanceof< EntityPlayerMP>(trackedEntity)) {
         ((EntityPlayerMP*)trackedEntity)->connection.sendPacket(packetIn);
      }
}

void EntityTrackerEntry::sendDestroyEntityPacketToTrackedPlayers() {
    for(auto entityplayermp : trackingPlayers) {
         trackedEntity->removeTrackingPlayer(entityplayermp);
         entityplayermp->removeEntity(trackedEntity);
      }
}

void EntityTrackerEntry::removeFromTrackedPlayers(EntityPlayerMP *playerMP) {
    if (trackingPlayers.contains(playerMP)) {
         trackedEntity->removeTrackingPlayer(playerMP);
         playerMP->removeEntity(trackedEntity);
         trackingPlayers.erase(playerMP);
      }
}

void EntityTrackerEntry::updatePlayerEntity(EntityPlayerMP *playerMP) {
    if (playerMP != trackedEntity) {
         if (isVisibleTo(playerMP)) {
            if (!trackingPlayers.contains(playerMP) && (isPlayerWatchingThisChunk(playerMP) || trackedEntity->forceSpawn)) {
               trackingPlayers.emplace(playerMP);
               Packet packet = createSpawnPacket();
               playerMP->connection.sendPacket(packet);
               if (!trackedEntity->getDataManager().isEmpty()) {
                  playerMP->connection.sendPacket(new SPacketEntityMetadata(trackedEntity->getEntityId(), trackedEntity->getDataManager(), true));
               }

               bool flag = sendVelocityUpdates;
               if (Util::instanceof<EntityLivingBase>(trackedEntity)) {
                  auto attributemap = (AttributeMap*)((EntityLivingBase*)trackedEntity)->getAttributeMap();
                  auto collection = attributemap->getWatchedAttributes();
                  if (!collection.empty()) {
                     playerMP->connection.sendPacket(new SPacketEntityProperties(trackedEntity->getEntityId(), collection));
                  }

                  if (((EntityLivingBase*)trackedEntity)->isElytraFlying()) {
                     flag = true;
                  }
               }

               lastTrackedEntityMotionX = trackedEntity->motionX;
               lastTrackedEntityMotionY = trackedEntity->motionY;
               motionZ = trackedEntity->motionZ;
               if (flag && !(Util::instanceof<SPacketSpawnMob>(packet))) {
                  playerMP->connection.sendPacket(new SPacketEntityVelocity(trackedEntity->getEntityId(), trackedEntity->motionX, trackedEntity->motionY, trackedEntity->motionZ));
               }

               if (Util::instanceof<EntityLivingBase>(trackedEntity)) {

                  for(auto entityequipmentslot : EntityEquipmentSlot::values()) {
                     ItemStack itemstack = ((EntityLivingBase*)trackedEntity)->getItemStackFromSlot(entityequipmentslot);
                     if (!itemstack.isEmpty()) {
                        playerMP->connection.sendPacket(new SPacketEntityEquipment(trackedEntity->getEntityId(), entityequipmentslot, itemstack));
                     }
                  }
               }

               if (Util::instanceof<EntityPlayer>(trackedEntity)) {
                  auto entityplayer = (EntityPlayer*)trackedEntity;
                  if (entityplayer->isPlayerSleeping()) {
                     playerMP->connection.sendPacket(new SPacketUseBed(entityplayer, new BlockPos(trackedEntity)));
                  }
               }

               if (Util::instanceof<EntityLivingBase>(trackedEntity)) {
                  auto entitylivingbase = (EntityLivingBase*)trackedEntity;

                   for(auto potioneffect : entitylivingbase->getActivePotionEffects()){
                     playerMP->connection.sendPacket(new SPacketEntityEffect(trackedEntity->getEntityId(), potioneffect));
                  }
               }

               if (!trackedEntity->getPassengers().empty()) {
                  playerMP->connection.sendPacket(new SPacketSetPassengers(trackedEntity));
               }

               if (trackedEntity->isRiding()) {
                  playerMP->connection.sendPacket(new SPacketSetPassengers(trackedEntity->getRidingEntity()));
               }

               trackedEntity->addTrackingPlayer(playerMP);
               playerMP->addEntity(trackedEntity);
            }
         } else if (trackingPlayers.contains(playerMP)) {
            trackingPlayers.erase(playerMP);
            trackedEntity->removeTrackingPlayer(playerMP);
            playerMP->removeEntity(trackedEntity);
         }
      }
}

bool EntityTrackerEntry::isVisibleTo(EntityPlayerMP *playerMP) const {
    double d0 = playerMP->posX - (double)encodedPosX / 4096.0;
      double d1 = playerMP->posZ - (double)encodedPosZ / 4096.0;
      auto i = MathHelper::min(range, maxRange);
      return d0 >= (double)(-i) && d0 <= (double)i && d1 >= (double)(-i) && d1 <= (double)i && trackedEntity->isSpectatedByPlayer(playerMP);
}

void EntityTrackerEntry::updatePlayerEntities(std::span<EntityPlayer*> players) {
    for(auto ply : players) {
         updatePlayerEntity((EntityPlayerMP*)ply);
      }
}

void EntityTrackerEntry::removeTrackedPlayerSymmetric(EntityPlayerMP *playerMP) {
    if (trackingPlayers.contains(playerMP)) {
         trackingPlayers.erase(playerMP);
         trackedEntity->removeTrackingPlayer(playerMP);
         playerMP->removeEntity(trackedEntity);
      }
}

Entity * EntityTrackerEntry::getTrackedEntity() const {
    return trackedEntity;
}

void EntityTrackerEntry::setMaxRange(int32_t maxRangeIn) {
    maxRange = maxRangeIn;
}

void EntityTrackerEntry::resetPlayerVisibility() {
    updatedPlayerVisibility = false;
}

void EntityTrackerEntry::sendMetadata() {
    EntityDataManager entitydatamanager = trackedEntity->getDataManager();
      if (entitydatamanager.isDirty()) {
         sendToTrackingAndSelf(new SPacketEntityMetadata(trackedEntity->getEntityId(), entitydatamanager, false));
      }

      if (Util::instanceof<EntityLivingBase>(trackedEntity)) {
         AttributeMap attributemap = (AttributeMap)((EntityLivingBase*)trackedEntity)->getAttributeMap();
         auto set = attributemap.getDirtyInstances();
         if (!set.empty()) {
            sendToTrackingAndSelf(new SPacketEntityProperties(trackedEntity->getEntityId(), set));
         }

         set.clear();
      }
}

bool EntityTrackerEntry::isPlayerWatchingThisChunk(EntityPlayerMP *playerMP) const {
    return playerMP->getServerWorld().getPlayerChunkMap().isPlayerWatchingChunk(playerMP, trackedEntity->chunkCoordX, trackedEntity->chunkCoordZ);
}

Packet EntityTrackerEntry::createSpawnPacket() {
    if (trackedEntity->isDead) {
         LOGGER->warn("Fetching addPacket for removed entity");
      }

      if (trackedEntity instanceof EntityPlayerMP) {
         return new SPacketSpawnPlayer((EntityPlayer)trackedEntity);
      } else if (trackedEntity instanceof IAnimals) {
         lastHeadMotion = MathHelper.floor(trackedEntity->getRotationYawHead() * 256.0F / 360.0F);
         return new SPacketSpawnMob((EntityLivingBase)trackedEntity);
      } else if (trackedEntity instanceof EntityPainting) {
         return new SPacketSpawnPainting((EntityPainting)trackedEntity);
      } else if (trackedEntity instanceof EntityItem) {
         return new SPacketSpawnObject(trackedEntity, 2, 1);
      } else if (trackedEntity instanceof EntityMinecart) {
         EntityMinecart entityminecart = (EntityMinecart)trackedEntity;
         return new SPacketSpawnObject(trackedEntity, 10, entityminecart.getType().getId());
      } else if (trackedEntity instanceof EntityBoat) {
         return new SPacketSpawnObject(trackedEntity, 1);
      } else if (trackedEntity instanceof EntityXPOrb) {
         return new SPacketSpawnExperienceOrb((EntityXPOrb)trackedEntity);
      } else if (trackedEntity instanceof EntityFishHook) {
         Entity entity2 = ((EntityFishHook)trackedEntity).getAngler();
         return new SPacketSpawnObject(trackedEntity, 90, entity2 == null ? trackedEntity->getEntityId() : entity2.getEntityId());
      } else {
         Entity entity;
         if (trackedEntity instanceof EntitySpectralArrow) {
            entity = ((EntitySpectralArrow)trackedEntity).shootingEntity;
            return new SPacketSpawnObject(trackedEntity, 91, 1 + (entity == null ? trackedEntity->getEntityId() : entity.getEntityId()));
         } else if (trackedEntity instanceof EntityTippedArrow) {
            entity = ((EntityArrow)trackedEntity).shootingEntity;
            return new SPacketSpawnObject(trackedEntity, 60, 1 + (entity == null ? trackedEntity->getEntityId() : entity.getEntityId()));
         } else if (trackedEntity instanceof EntitySnowball) {
            return new SPacketSpawnObject(trackedEntity, 61);
         } else if (trackedEntity instanceof EntityLlamaSpit) {
            return new SPacketSpawnObject(trackedEntity, 68);
         } else if (trackedEntity instanceof EntityPotion) {
            return new SPacketSpawnObject(trackedEntity, 73);
         } else if (trackedEntity instanceof EntityExpBottle) {
            return new SPacketSpawnObject(trackedEntity, 75);
         } else if (trackedEntity instanceof EntityEnderPearl) {
            return new SPacketSpawnObject(trackedEntity, 65);
         } else if (trackedEntity instanceof EntityEnderEye) {
            return new SPacketSpawnObject(trackedEntity, 72);
         } else if (trackedEntity instanceof EntityFireworkRocket) {
            return new SPacketSpawnObject(trackedEntity, 76);
         } else if (trackedEntity instanceof EntityFireball) {
            EntityFireball entityfireball = (EntityFireball)trackedEntity;
            SPacketSpawnObject spacketspawnobject = null;
            int i = 63;
            if (trackedEntity instanceof EntitySmallFireball) {
               i = 64;
            } else if (trackedEntity instanceof EntityDragonFireball) {
               i = 93;
            } else if (trackedEntity instanceof EntityWitherSkull) {
               i = 66;
            }

            if (entityfireball.shootingEntity != null) {
               spacketspawnobject = new SPacketSpawnObject(trackedEntity, i, ((EntityFireball)trackedEntity).shootingEntity.getEntityId());
            } else {
               spacketspawnobject = new SPacketSpawnObject(trackedEntity, i, 0);
            }

            spacketspawnobject.setSpeedX((int)(entityfireball.accelerationX * 8000.0D));
            spacketspawnobject.setSpeedY((int)(entityfireball.accelerationY * 8000.0D));
            spacketspawnobject.setSpeedZ((int)(entityfireball.accelerationZ * 8000.0D));
            return spacketspawnobject;
         } else if (trackedEntity instanceof EntityShulkerBullet) {
            SPacketSpawnObject spacketspawnobject1 = new SPacketSpawnObject(trackedEntity, 67, 0);
            spacketspawnobject1.setSpeedX((int)(trackedEntity->motionX * 8000.0D));
            spacketspawnobject1.setSpeedY((int)(trackedEntity->motionY * 8000.0D));
            spacketspawnobject1.setSpeedZ((int)(trackedEntity->motionZ * 8000.0D));
            return spacketspawnobject1;
         } else if (trackedEntity instanceof EntityEgg) {
            return new SPacketSpawnObject(trackedEntity, 62);
         } else if (trackedEntity instanceof EntityEvokerFangs) {
            return new SPacketSpawnObject(trackedEntity, 79);
         } else if (trackedEntity instanceof EntityTNTPrimed) {
            return new SPacketSpawnObject(trackedEntity, 50);
         } else if (trackedEntity instanceof EntityEnderCrystal) {
            return new SPacketSpawnObject(trackedEntity, 51);
         } else if (trackedEntity instanceof EntityFallingBlock) {
            EntityFallingBlock entityfallingblock = (EntityFallingBlock)trackedEntity;
            return new SPacketSpawnObject(trackedEntity, 70, Block.getStateId(entityfallingblock.getBlock()));
         } else if (trackedEntity instanceof EntityArmorStand) {
            return new SPacketSpawnObject(trackedEntity, 78);
         } else if (trackedEntity instanceof EntityItemFrame) {
            EntityItemFrame entityitemframe = (EntityItemFrame)trackedEntity;
            return new SPacketSpawnObject(trackedEntity, 71, entityitemframe.facingDirection.getHorizontalIndex(), entityitemframe.getHangingPosition());
         } else if (trackedEntity instanceof EntityLeashKnot) {
            EntityLeashKnot entityleashknot = (EntityLeashKnot)trackedEntity;
            return new SPacketSpawnObject(trackedEntity, 77, 0, entityleashknot.getHangingPosition());
         } else if (trackedEntity instanceof EntityAreaEffectCloud) {
            return new SPacketSpawnObject(trackedEntity, 3);
         } else {
            throw new IllegalArgumentException("Don't know how to add " + trackedEntity->getClass() + "!");
         }
      }
}

bool operator==(const EntityTrackerEntry &lhs, const EntityTrackerEntry &rhs) {
         return lhs.trackedEntity->getEntityId() == rhs.trackedEntity->getEntityId();
}
