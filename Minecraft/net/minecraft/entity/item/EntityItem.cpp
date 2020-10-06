#include "EntityItem.h"

#include "DamageSource.h"
#include "../../stats/StatList.h"
#include "datafix/DataFixer.h"
#include "datafix/FixTypes.h"
#include "text/translation/I18n.h"

std::shared_ptr<spdlog::logger> EntityItem::LOGGER = spdlog::get("Minecraft")->clone("EntityItem");
DataParameter EntityItem::ITEM = EntityDataManager::createKey(EntityItem.class, DataSerializers::ITEM_STACK);

EntityItem::EntityItem(World *worldIn, double x, double y, double z)
    :Entity(worldIn),health(5),hoverStart(MathHelper::random() * 3.141592653589793 * 2.0){
    setSize(0.25F, 0.25F);
    setPosition(x, y, z);
    rotationYaw = 360.0 * MathHelper::random();
    motionX = MathHelper::random() * 0.20000000298023224 - 0.10000000149011612;
    motionY = 0.20000000298023224;
    motionZ = MathHelper::random() * 0.20000000298023224 - 0.10000000149011612;
}

EntityItem::EntityItem(World *worldIn, double x, double y, double z, ItemStack stack)
    :EntityItem(worldIn, x, y, z){
    setItem(stack);
}

EntityItem::EntityItem(World *worldIn)
    :Entity(worldIn),health(5),hoverStart(MathHelper::random() * 3.141592653589793 * 2.0){
    setSize(0.25F, 0.25F);
    setItem(ItemStack::EMPTY);
}

void EntityItem::onUpdate() {
    if (getItem().isEmpty()) {
         setDead();
      } else {
         Entity::onUpdate();
         if (pickupDelay > 0 && pickupDelay != 32767) {
            --pickupDelay;
         }

         prevPosX = posX;
         prevPosY = posY;
         prevPosZ = posZ;
         double d0 = motionX;
         double d1 = motionY;
         double d2 = motionZ;
         if (!hasNoGravity()) {
            motionY -= 0.03999999910593033;
         }

         if (world->isRemote) {
            noClip = false;
         } else {
            noClip = pushOutOfBlocks(posX, (getEntityBoundingBox().getminY() + getEntityBoundingBox().getmaxY()) / 2.0, posZ);
         }

         move(MoverType::SELF, motionX, motionY, motionZ);
         bool flag = (int)prevPosX != (int)posX || (int)prevPosY != (int)posY || (int)prevPosZ != (int)posZ;
         if (flag || ticksExisted % 25 == 0) {
            if (world->getBlockState(BlockPos(this))->getMaterial() == Material::LAVA) {
               motionY = 0.20000000298023224;
               motionX = (double)((MathHelper::nextFloat(rand) - MathHelper::nextFloat(rand)) * 0.2F);
               motionZ = (double)((MathHelper::nextFloat(rand) - MathHelper::nextFloat(rand)) * 0.2F);
               playSound(SoundEvents::ENTITY_GENERIC_BURN, 0.4F, 2.0F + MathHelper::nextFloat(rand) * 0.4F);
            }

            if (!world->isRemote) {
               searchForOtherItemsNearby();
            }
         }

         float f = 0.98F;
         if (onGround) {
            f = world->getBlockState(BlockPos(MathHelper::floor(posX), MathHelper::floor(getEntityBoundingBox().getminY()) - 1, MathHelper::floor(posZ)))->getBlock()->slipperiness * 0.98F;
         }

         motionX *= (double)f;
         motionY *= 0.9800000190734863;
         motionZ *= (double)f;
         if (onGround) {
            motionY *= -0.5;
         }

         if (age != -32768) {
            ++age;
         }

         handleWaterMovement();
         if (!world->isRemote) {
            double d3 = motionX - d0;
            double d4 = motionY - d1;
            double d5 = motionZ - d2;
            double d6 = d3 * d3 + d4 * d4 + d5 * d5;
            if (d6 > 0.01) {
               isAirBorne = true;
            }
         }

         if (!world->isRemote && age >= 6000) {
            setDead();
         }
      }
}

void EntityItem::setAgeToCreativeDespawnTime() {
    age = 4800;
}

bool EntityItem::handleWaterMovement() {
    if (world->handleMaterialAcceleration(getEntityBoundingBox(), Material::WATER, this)) {
         if (!inWater && !firstUpdate) {
            doWaterSplashEffect();
         }

         inWater = true;
      } else {
         inWater = false;
      }

      return inWater;
}

bool EntityItem::attackEntityFrom(DamageSource::DamageSource source, float amount) {
    if (isEntityInvulnerable(source)) {
         return false;
      } else if (!getItem().isEmpty() && getItem().getItem() == Items::NETHER_STAR && source.isExplosion()) {
         return false;
      } else {
         markVelocityChanged();
         health = (int)((float)health - amount);
         if (health <= 0) {
            setDead();
         }

         return false;
      }
}

void EntityItem::registerFixesItem(DataFixer fixer) {
    fixer.registerWalker(FixTypes::ENTITY, new ItemStackData(EntityItem.class, {"Item"}));
}

void EntityItem::writeEntityToNBT(NBTTagCompound *compound) {
    compound->setShort("Health", (short)health);
    compound->setShort("Age", (short)age);
    compound->setShort("PickupDelay", (short)pickupDelay);
    if (getThrower() != nullptr) {
        compound->setString("Thrower", thrower);
    }

    if (getOwner() != nullptr) {
        compound->setString("Owner", owner);
    }

    if (!getItem().isEmpty()) {
        compound->setTag("Item", getItem().writeToNBT(new NBTTagCompound()));
    }
}

void EntityItem::readEntityFromNBT(NBTTagCompound *compound) {
    health = compound->getShort("Health");
      age = compound->getShort("Age");
      if (compound->hasKey("PickupDelay")) {
         pickupDelay = compound->getShort("PickupDelay");
      }

      if (compound->hasKey("Owner")) {
         owner = compound->getString("Owner");
      }

      if (compound->hasKey("Thrower")) {
         thrower = compound->getString("Thrower");
      }

      NBTTagCompound* nbttagcompound = compound->getCompoundTag("Item");
      setItem(ItemStack(nbttagcompound));
      if (getItem().isEmpty()) {
         setDead();
      }
}

void EntityItem::onCollideWithPlayer(EntityPlayer *entityIn) {
    if (!world->isRemote) {
         ItemStack itemstack = getItem();
         Item* item = itemstack.getItem();
         int i = itemstack.getCount();
         if (pickupDelay == 0 && (owner.empty() || 6000 - age <= 200 || owner == entityIn->getName())) && entityIn->inventory.addItemStackToInventory(itemstack) {
            entityIn->onItemPickup(this, i);
            if (itemstack.isEmpty()) {
               setDead();
               itemstack.setCount(i);
            }

            entityIn->addStat(StatList::getObjectsPickedUpStats(item), i);
         }
      }
}

std::string EntityItem::getName() {
    return hasCustomName() ? getCustomNameTag() : I18n::translateToLocal("item." + getItem().getTranslationKey());
}

bool EntityItem::canBeAttackedWithItem() {
    return false;
}

Entity * EntityItem::changeDimension(int32_t dimensionIn) {
    Entity* entity = Entity::changeDimension(dimensionIn);
      if (!world->isRemote && Util::instanceof<EntityItem>(entity)) {
         ((EntityItem*)entity)->searchForOtherItemsNearby();
      }

      return entity;
}

ItemStack EntityItem::getItem() {
    return getDataManager().get(ITEM);
}

void EntityItem::setItem(ItemStack stack) {
    getDataManager().set(ITEM, stack);
    getDataManager().setDirty(ITEM);
}

std::string EntityItem::getOwner() const {
    return owner;
}

void EntityItem::setOwner(std::string_view ownerIn) {
    owner = ownerIn;
}

std::string EntityItem::getThrower() const {
    return thrower;
}

void EntityItem::setThrower(std::string_view throwerIn) {
    thrower = throwerIn;
}

int32_t EntityItem::getAge() const {
    return age;
}

void EntityItem::setDefaultPickupDelay() {
    pickupDelay = 10;
}

void EntityItem::setNoPickupDelay() {
    pickupDelay = 0;
}

void EntityItem::setInfinitePickupDelay() {
    pickupDelay = 32767;
}

void EntityItem::setPickupDelay(int32_t ticks) {
    pickupDelay = ticks;
}

bool EntityItem::cannotPickup() const {
    return pickupDelay > 0;
}

void EntityItem::setNoDespawn() {
    age = -6000;
}

void EntityItem::makeFakeItem() {
    setInfinitePickupDelay();
    age = 5999;
}

bool EntityItem::canTriggerWalking() {
    return false;
}

void EntityItem::entityInit() {
    getDataManager().registe(ITEM, ItemStack::EMPTY);
}

void EntityItem::dealFireDamage(int32_t amount) {
    attackEntityFrom(DamageSource::IN_FIRE, amount);
}

void EntityItem::searchForOtherItemsNearby() {
    auto var1 = world->getEntitiesWithinAABB<EntityItem>(getEntityBoundingBox().grow(0.5, 0.0, 0.5));

    for(auto entityitem : var1){
        combineItems(entityitem);
    }
}

bool EntityItem::combineItems(EntityItem *other) {
    if (other == this) {
         return false;
      } else if (other->isEntityAlive() && isEntityAlive()) {
         ItemStack itemstack = getItem();
         ItemStack itemstack1 = other->getItem();
         if (pickupDelay != 32767 && other->pickupDelay != 32767) {
            if (age != -32768 && other->age != -32768) {
               if (itemstack1.getItem() != itemstack.getItem()) {
                  return false;
               } else if (itemstack1.hasTagCompound() ^ itemstack.hasTagCompound()) {
                  return false;
               } else if (itemstack1.hasTagCompound() && !itemstack1.getTagCompound() == itemstack.getTagCompound()) {
                  return false;
               } else if (itemstack1.getItem() == nullptr) {
                  return false;
               } else if (itemstack1.getItem()->getHasSubtypes() && itemstack1.getMetadata() != itemstack.getMetadata()) {
                  return false;
               } else if (itemstack1.getCount() < itemstack.getCount()) {
                  return other->combineItems(this);
               } else if (itemstack1.getCount() + itemstack.getCount() > itemstack1.getMaxStackSize()) {
                  return false;
               } else {
                  itemstack1.grow(itemstack.getCount());
                  other->pickupDelay = MathHelper::max(other->pickupDelay, pickupDelay);
                  other->age = MathHelper::min(other->age, age);
                  other->setItem(itemstack1);
                  setDead();
                  return true;
               }
            } else {
               return false;
            }
         } else {
            return false;
         }
      } else {
         return false;
      }
}
