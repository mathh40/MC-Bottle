#include "EntityAgeable.h"

#include "../item/ItemMonsterPlacer.h"

#include <typeindex>

DataParameter EntityAgeable::BABY = EntityDataManager.createKey(EntityAgeable.class, DataSerializers::BOOLEAN);

EntityAgeable::EntityAgeable(World *worldIn)
    :EntityCreature(worldIn)
{
}

bool EntityAgeable::processInteract(EntityPlayer *player, EnumHand hand) {
    ItemStack itemstack = player->getHeldItem(hand);
      if (itemstack.getItem() == Items::SPAWN_EGG) {
         if (!world->isRemote) {
            Class oclass = EntityList::REGISTRY.getObject(ItemMonsterPlacer::getNamedIdFrom(itemstack));
            if (oclass != nullptr && getClass() == oclass) {
               EntityAgeable* entityageable = createChild(this);
               if (entityageable != nullptr) {
                  entityageable->setGrowingAge(-24000);
                  entityageable->setLocationAndAngles(posX, posY, posZ, 0.0F, 0.0F);
                  world->spawnEntity(entityageable);
                  if (itemstack.hasDisplayName()) {
                     entityageable->setCustomNameTag(itemstack.getDisplayName());
                  }

                  if (!player->capabilities.isCreativeMode) {
                     itemstack.shrink(1);
                  }
               }
            }
         }

         return true;
      } else {
         return false;
      }
}

int32_t EntityAgeable::getGrowingAge() {
    if (world->isRemote) 
    {
         return dataManager.get(BABY) ? -1 : 1;
      } else {
         return growingAge;
      }
}

void EntityAgeable::ageUp(int32_t growthSeconds, bool updateForcedAge) {
    int32_t i = getGrowingAge();
      int32_t j = i;
      i += growthSeconds * 20;
      if (i > 0) {
         i = 0;
         if (j < 0) {
            onGrowingAdult();
         }
      }

      int32_t k = i - j;
      setGrowingAge(i);
      if (updateForcedAge) {
         forcedAge += k;
         if (forcedAgeTimer == 0) {
            forcedAgeTimer = 40;
         }
      }

      if (getGrowingAge() == 0) {
         setGrowingAge(forcedAge);
      }
}

void EntityAgeable::addGrowth(int32_t growth) {
    ageUp(growth, false);
}

void EntityAgeable::setGrowingAge(int32_t age) {
    dataManager.set(BABY, age < 0);
    growingAge = age;
    setScaleForAge(isChild());
}

void EntityAgeable::writeEntityToNBT(NBTTagCompound *compound) {
    EntityCreature::writeEntityToNBT(compound);
    compound->setInteger("Age", getGrowingAge());
    compound->setInteger("ForcedAge", forcedAge);
}

void EntityAgeable::readEntityFromNBT(NBTTagCompound *compound) {
    EntityCreature::readEntityFromNBT(compound);
    setGrowingAge(compound->getInteger("Age"));
    forcedAge = compound->getInteger("ForcedAge");
}

void EntityAgeable::notifyDataManagerChange(DataParameter key) {
    if (BABY == key) {
         setScaleForAge(isChild());
      }

      EntityCreature::notifyDataManagerChange(key);
}

void EntityAgeable::onLivingUpdate() {
    EntityCreature::onLivingUpdate();
      if (world->isRemote) {
         if (forcedAgeTimer > 0) {
            if (forcedAgeTimer % 4 == 0) {
               world->spawnParticle(EnumParticleTypes::VILLAGER_HAPPY, posX + (double)(MathHelper::nextFloat(rand) * width * 2.0F) - (double)width, posY + 0.5 + (double)(MathHelper::nextFloat(rand) * this.height), this.posZ + (double)(MathHelper::nextFloat(rand) * width * 2.0F) - (double)width, 0.0, 0.0, 0.0);
            }

            --forcedAgeTimer;
         }
      } else {
         int32_t i = getGrowingAge();
         if (i < 0) {
            ++i;
            setGrowingAge(i);
            if (i == 0) {
               onGrowingAdult();
            }
         } else if (i > 0) {
            --i;
            setGrowingAge(i);
         }
      }
}

bool EntityAgeable::isChild() {
    return getGrowingAge() < 0;
}

void EntityAgeable::setScaleForAge(bool child) {
    setScale(child ? 0.5F : 1.0F);
}

bool EntityAgeable::holdingSpawnEggOfClass(ItemStack stack, std::type_index entityClass) {
    if (stack.getItem() != Items::SPAWN_EGG) {
         return false;
      } else {
         auto oclass = EntityList::REGISTRY.getObject(ItemMonsterPlacer::getNamedIdFrom(stack));
         return oclass != nullptr && entityClass == oclass;
      }
}

void EntityAgeable::entityInit() {
    EntityCreature::entityInit();
    dataManager.registers(BABY, false);
}

void EntityAgeable::onGrowingAdult() {
    
}

void EntityAgeable::setSize(float width, float height) {
    bool flag = ageWidth > 0.0F;
      ageWidth = width;
      ageHeight = height;
      if (!flag) {
         setScale(1.0F);
      }
}

void EntityAgeable::setScale(float scale) {
    EntityCreature::setSize(ageWidth * scale, ageHeight * scale);
}
