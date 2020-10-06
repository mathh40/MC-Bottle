#include "EntityFireworkRocket.h"

#include "../EntityLivingBase.h"
#include "datafix/DataFixer.h"
#include "datafix/FixTypes.h"

DataParameter EntityFireworkRocket::FIREWORK_ITEM = EntityDataManager::createKey(EntityFireworkRocket.class, DataSerializers::ITEM_STACK);
DataParameter EntityFireworkRocket::BOOSTED_ENTITY_ID = EntityDataManager::createKey(EntityFireworkRocket.class, DataSerializers::VARINT);

EntityFireworkRocket::EntityFireworkRocket(World *worldIn)
    :Entity(worldIn){
    setSize(0.25F, 0.25F);
}

EntityFireworkRocket::EntityFireworkRocket(World *worldIn, double x, double y, double z, ItemStack givenItem)
    :Entity(worldIn),fireworkAge(0){
        setSize(0.25F, 0.25F);
        setPosition(x, y, z);
        int32_t i = 1;
        if (!givenItem.isEmpty() && givenItem.hasTagCompound()) {
            dataManager.set(FIREWORK_ITEM, givenItem.copy());
            NBTTagCompound* nbttagcompound = givenItem.getTagCompound();
            NBTTagCompound* nbttagcompound1 = nbttagcompound->getCompoundTag("Fireworks");
            i += nbttagcompound1->getByte("Flight");
        }

        motionX = MathHelper::nextGaussian<double>(rand) * 0.001;
        motionZ = MathHelper::nextGaussian<double>(rand) * 0.001;
        motionY = 0.05;
        lifetime = 10 * i + rand(6) + rand(7);
}

EntityFireworkRocket::EntityFireworkRocket(World *worldIn, ItemStack givenItem, EntityLivingBase *p_i47367_3_)
    :EntityFireworkRocket(worldIn, p_i47367_3_->posX, p_i47367_3_->posY, p_i47367_3_->posZ, givenItem){
      dataManager.set(BOOSTED_ENTITY_ID, p_i47367_3_->getEntityId());
      boostedEntity = p_i47367_3_;
}

bool EntityFireworkRocket::isInRangeToRenderDist(double distance) {
    return distance < 4096.0 && !isAttachedToEntity();
}

bool EntityFireworkRocket::isInRangeToRender3d(double x, double y, double z) {
    return Entity::isInRangeToRender3d(x, y, z) && !isAttachedToEntity();
}

void EntityFireworkRocket::setVelocity(double x, double y, double z) {
    motionX = x;
    motionY = y;
    motionZ = z;
    if (prevRotationPitch == 0.0F && prevRotationYaw == 0.0F) {
        float f = MathHelper::sqrt(x * x + z * z);
        rotationYaw = (float)(MathHelper::atan2(x, z) * 57.29577951308232);
        rotationPitch = (float)(MathHelper::atan2(y, (double)f) * 57.29577951308232);
        prevRotationYaw = rotationYaw;
        prevRotationPitch = rotationPitch;
    }
}

void EntityFireworkRocket::onUpdate() {
    lastTickPosX = posX;
      lastTickPosY = posY;
      lastTickPosZ = posZ;
      Entity::onUpdate();
      if (isAttachedToEntity()) {
         if (boostedEntity == nullptr) {
            Entity* entity = world->getEntityByID(dataManager.get(BOOSTED_ENTITY_ID));
            if (Util::instanceof<EntityLivingBase>(entity)) {
               boostedEntity = (EntityLivingBase*)entity;
            }
         }

         if (boostedEntity != nullptr) {
            if (boostedEntity->isElytraFlying()) {
               Vec3d vec3d = boostedEntity->getLookVec();
               double d0 = 1.5;
               double d1 = 0.1;
               boostedEntity->motionX += vec3d.getx() * 0.1 + (vec3d.getx() * 1.5 - boostedEntity->motionX) * 0.5;
               boostedEntity->motionY += vec3d.gety() * 0.1 + (vec3d.gety() * 1.5 - boostedEntity->motionY) * 0.5;
               boostedEntity->motionZ += vec3d.getz() * 0.1 + (vec3d.getz() * 1.5 - boostedEntity->motionZ) * 0.5;
            }

            setPosition(boostedEntity->posX, boostedEntity->posY, boostedEntity->posZ);
            motionX = boostedEntity->motionX;
            motionY = boostedEntity->motionY;
            motionZ = boostedEntity->motionZ;
         }
      } else {
         motionX *= 1.15;
         motionZ *= 1.15;
         motionY += 0.04;
         move(MoverType::SELF, motionX, motionY, motionZ);
      }

      float f = MathHelper:::sqrt(motionX * motionX + motionZ * motionZ);
      rotationYaw = (MathHelper::atan2(motionX, motionZ) * 57.29577951308232);

      for(rotationPitch = (MathHelper::atan2(motionY, f) * 57.29577951308232); rotationPitch - prevRotationPitch < -180.0F; prevRotationPitch -= 360.0F) {
      }

      while(rotationPitch - prevRotationPitch >= 180.0F) {
         prevRotationPitch += 360.0F;
      }

      while(rotationYaw - prevRotationYaw < -180.0F) {
         prevRotationYaw -= 360.0F;
      }

      while(rotationYaw - prevRotationYaw >= 180.0F) {
         prevRotationYaw += 360.0F;
      }

      rotationPitch = prevRotationPitch + (rotationPitch - prevRotationPitch) * 0.2F;
      rotationYaw = prevRotationYaw + (rotationYaw - prevRotationYaw) * 0.2F;
      if (fireworkAge == 0 && !isSilent()) {
         world->playSound(nullptr, posX, posY, posZ, SoundEvents::ENTITY_FIREWORK_LAUNCH, SoundCategory::AMBIENT, 3.0F, 1.0F);
      }

      ++fireworkAge;
      if (world->isRemote && fireworkAge % 2 < 2) {
         world->spawnParticle(EnumParticleTypes::FIREWORKS_SPARK, posX, posY - 0.3, posZ, MathHelper::nextGaussian<double>(rand) * 0.05, -motionY * 0.5, MathHelper::nextGaussian<double>(rand) * 0.05);
      }

      if (!world->isRemote && fireworkAge > lifetime) {
         world->setEntityState(this, std::byte(17));
         dealExplosionDamage();
         setDead();
      }
}

bool EntityFireworkRocket::isAttachedToEntity() {
    return dataManager.get(BOOSTED_ENTITY_ID) > 0;
}

void EntityFireworkRocket::handleStatusUpdate(std::byte id) {
    if (id == std::byte{17} && world->isRemote) {
         ItemStack itemstack = (ItemStack)dataManager.get(FIREWORK_ITEM);
         NBTTagCompound* nbttagcompound = itemstack.isEmpty() ? nullptr : itemstack.getSubCompound("Fireworks");
         world->makeFireworks(posX, posY, posZ, motionX, motionY, motionZ, nbttagcompound);
      }

      Entity::handleStatusUpdate(id);
}

void EntityFireworkRocket::registerFixesFireworkRocket(DataFixer fixer) {
    fixer.registerWalker(FixTypes::ENTITY, new ItemStackData(EntityFireworkRocket.class, {"FireworksItem"}));
}

void EntityFireworkRocket::writeEntityToNBT(NBTTagCompound *compound) {
    compound->setInteger("Life", fireworkAge);
    compound->setInteger("LifeTime", lifetime);
    ItemStack itemstack = (ItemStack)dataManager.get(FIREWORK_ITEM);
    if (!itemstack.isEmpty()) {
        compound->setTag("FireworksItem", itemstack.writeToNBT(new NBTTagCompound()));
    }
}

void EntityFireworkRocket::readEntityFromNBT(NBTTagCompound *compound) {
    fireworkAge = compound->getInteger("Life");
    lifetime = compound->getInteger("LifeTime");
    NBTTagCompound* nbttagcompound = compound->getCompoundTag("FireworksItem");
    if (nbttagcompound != nullptr) {
        ItemStack itemstack = ItemStack(nbttagcompound);
        if (!itemstack.isEmpty()) {
            this.dataManager.set(FIREWORK_ITEM, itemstack);
        }
    }
}

bool EntityFireworkRocket::canBeAttackedWithItem() {
    return false;
}

void EntityFireworkRocket::entityInit() {
    dataManager.registe(FIREWORK_ITEM, ItemStack::EMPTY);
    dataManager.registe(BOOSTED_ENTITY_ID, 0);
}

void EntityFireworkRocket::dealExplosionDamage() {
    float f = 0.0F;
      ItemStack itemstack = (ItemStack)dataManager.get(FIREWORK_ITEM);
      NBTTagCompound* nbttagcompound = itemstack.isEmpty() ? nullptr : itemstack.getSubCompound("Fireworks");
      NBTTagList* nbttaglist = nbttagcompound != nullptr ? nbttagcompound.getTagList("Explosions", 10) : nullptr;
      if (nbttaglist != nullptr && !nbttaglist.isEmpty()) {
         f = (float)(5 + nbttaglist.tagCount() * 2);
      }

      if (f > 0.0F) {
         if (boostedEntity != nullptr) {
            boostedEntity->attackEntityFrom(DamageSource::FIREWORKS, (float)(5 + nbttaglist.tagCount() * 2));
         }

         double d0 = 5.0;
         Vec3d vec3d = Vec3d(posX, posY, posZ);
         auto var8 = world->getEntitiesWithinAABB<EntityLivingBase>(getEntityBoundingBox().grow(5.0));

         while(true) {
            EntityLivingBase* entitylivingbase;
            do {
               do {
                  if (!var8.hasNext()) {
                     return;
                  }

                  entitylivingbase = (EntityLivingBase)var8.next();
               } while(entitylivingbase == boostedEntity);
            } while(getDistanceSq(entitylivingbase) > 25.0);

            bool flag = false;

            for(int i = 0; i < 2; ++i) {
               auto raytraceresult = world->rayTraceBlocks(vec3d, Vec3d(entitylivingbase->posX, entitylivingbase->posY + (double)entitylivingbase->height * 0.5 * (double)i, entitylivingbase->posZ), false, true, false);
               if (raytraceresult == std::nullopt || raytraceresult.typeOfHit == RayTraceResult::Type::MISS) {
                  flag = true;
                  break;
               }
            }

            if (flag) {
               float f1 = f * (float)MathHelper::sqrt((5.0 - (double)getDistance(entitylivingbase)) / 5.0);
               entitylivingbase->attackEntityFrom(DamageSource::FIREWORKS, f1);
            }
         }
      }
}
