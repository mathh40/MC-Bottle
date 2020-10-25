#include "EntityLiving.h"
#include "NBTTagFloat.h"
#include "SharedMonsterAttributes.h"
#include "../inventory/EntityEquipmentSlot.h"
#include "../item/ItemArmor.h"
#include "../pathfinding/PathNavigateGround.h"
#include "../world/WorldServer.h"
#include "ai/attributes/AbstractAttributeMap.h"
#include "datafix/DataFixer.h"
#include <typeindex>

AI_FLAGS = EntityDataManager.createKey(EntityLiving.class, DataSerializers.BYTE);

EntityLiving::EntityLiving(World *worldIn):
    EntityLivingBase(worldIn),
    inventoryHands(2, ItemStack::EMPTY),
    inventoryHandsDropChances(2, 0.085F),
    inventoryArmor(4, ItemStack::EMPTY),
    inventoryArmorDropChances(4, 0.085F),
    deathLootTable("", ""),
    moveHelper(this),
    jumpHelper(this),
    tasks(new EntityAITasks(worldIn != nullptr && worldIn->profiler != nullptr ? worldIn->profiler : nullptr)),
    targetTasks(new EntityAITasks(worldIn != nullptr && worldIn->profiler != nullptr ? worldIn->profiler : nullptr)),
    lookHelper(this),
    senses(this) {

    bodyHelper = createBodyHelper();
    navigator = createNavigator(worldIn);
    if (worldIn != nullptr && !worldIn->isRemote) {
        initEntityAI();
    }
}

float EntityLiving::getPathPriority(PathNodeType nodeType) {
    auto f = mapPathPriority.contains(nodeType);
    return f ? nodeType.getPriority() : mapPathPriority[nodeType];
}

void EntityLiving::setPathPriority(PathNodeType nodeType, float priority) {
    mapPathPriority.try_emplace(nodeType, priority);
}

EntityLookHelper &EntityLiving::getLookHelper() {
    return lookHelper;
}

EntityMoveHelper &EntityLiving::getMoveHelper() {
    return moveHelper;
}

EntityJumpHelper &EntityLiving::getJumpHelper() {
    return jumpHelper;
}

PathNavigate *EntityLiving::getNavigator() {
    return navigator;
}

EntitySenses &EntityLiving::getEntitySenses() {
    return senses;
}

EntityLivingBase *EntityLiving::getAttackTarget() {
    return attackTarget;
}

void EntityLiving::setAttackTarget(EntityLivingBase *entitylivingbaseIn) {
    attackTarget = entitylivingbaseIn;
}

bool EntityLiving::canAttackClass(std::type_index cls) {
    return cls != typeid(EntityGhast);
}

void EntityLiving::eatGrassBonus() {
}

int32_t EntityLiving::getTalkInterval() {
    return 80;
}

void EntityLiving::playLivingSound() {
    SoundEvent soundevent = getAmbientSound();
    if (soundevent != nullptr) {
        playSound(soundevent, getSoundVolume(), getSoundPitch());
    }
}

void EntityLiving::onEntityUpdate() {
    EntityLivingBase::onEntityUpdate();
    world->profiler.startSection("mobBaseTick");
    if (isEntityAlive() && rand(1000) < livingSoundTime++) {
        applyEntityAI();
        playLivingSound();
    }

    world->profiler.endSection();
}

void EntityLiving::spawnExplosionParticle() {
    if (world->isRemote) {
        for (auto i = 0; i < 20; ++i) {
            double d0 = MathHelper::nextGaussian<double>(rand) * 0.02;
            double d1 = MathHelper::nextGaussian<double>(rand) * 0.02;
            double d2 = MathHelper::nextGaussian<double>(rand) * 0.02;
            double d3 = 10.0;
            world->spawnParticle(EnumParticleTypes::EXPLOSION_NORMAL,
                                 posX + (double)(MathHelper::nextFloat(rand) * width * 2.0F) - (double)width - d0 *
                                 10.0, posY + (double)(MathHelper::nextFloat(rand) * height) - d1 * 10.0,
                                 posZ + (double)(MathHelper::nextFloat(rand) * width * 2.0F) - (double)width - d2 *
                                 10.0, d0, d1, d2);
        }
    } else {
        world->setEntityState(this, std::byte{20});
    }
}

void EntityLiving::handleStatusUpdate(std::byte id) {
    if (id == std::byte{20}) {
        spawnExplosionParticle();
    } else {
        EntityLivingBase::handleStatusUpdate(id);
    }
}

void EntityLiving::onUpdate() {
    EntityLivingBase::onUpdate();
    if (!world->isRemote) {
        updateLeashedState();
        if (ticksExisted % 5 == 0) {
            bool flag = !(Util::instanceof<EntityLiving>(getControllingPassenger()));
            bool flag1 = !(Util::instanceof<EntityBoat>(getRidingEntity()));
            tasks.setControlFlag(1, flag);
            tasks.setControlFlag(4, flag && flag1);
            tasks.setControlFlag(2, flag);
        }
    }
}

void EntityLiving::registerFixesMob(DataFixer fixer, std::type_index name) {
    fixer.registerWalker(FixTypes::ENTITY, new ItemStackDataLists(name, std::string[]
    {
        "ArmorItems", "HandItems"
    }
    )
    )
    ;
}

void EntityLiving::writeEntityToNBT(NBTTagCompound *compound) {
    EntityLivingBase::writeEntityToNBT(compound);
    compound->setBoolean("CanPickUpLoot", canPickUpLoot());
    compound->setBoolean("PersistenceRequired", persistenceRequired);
    NBTTagList *nbttaglist = new NBTTagList();

    NBTTagCompound *nbttagcompound;
    for (auto var3 = inventoryArmor.begin(); var3 != inventoryArmor.end(); nbttaglist->appendTag(nbttagcompound), ++var3
    ) {
        ItemStack itemstack = *var3;
        nbttagcompound = new NBTTagCompound();
        if (!itemstack.isEmpty()) {
            itemstack.writeToNBT(nbttagcompound);
        }
    }

    compound->setTag("ArmorItems", nbttaglist);
    NBTTagList *nbttaglist1 = new NBTTagList();

    NBTTagCompound *nbttagcompound2;
    for (auto var3 = inventoryHands.begin(); var3 != inventoryHands.end(); nbttaglist->appendTag(nbttagcompound), ++var3
    ) {
        ItemStack itemstack1 = (ItemStack)*var3;
        nbttagcompound2 = new NBTTagCompound();
        if (!itemstack1.isEmpty()) {
            itemstack1.writeToNBT(nbttagcompound2);
        }
    }

    compound->setTag("HandItems", nbttaglist1);
    NBTTagList *nbttaglist2 = new NBTTagList();
    auto var14 = inventoryArmorDropChances;
    int var16 = var14.size();

    int var7;
    for (var7 = 0; var7 < var16; ++var7) {
        float f = var14[var7];
        nbttaglist2->appendTag(new NBTTagFloat(f));
    }

    compound->setTag("ArmorDropChances", nbttaglist2);
    NBTTagList *nbttaglist3 = new NBTTagList();
    auto var17 = inventoryHandsDropChances;
    var7 = var17.size();

    for (int var19 = 0; var19 < var7; ++var19) {
        float f1 = var17[var19];
        nbttaglist3->appendTag(new NBTTagFloat(f1));
    }

    compound->setTag("HandDropChances", nbttaglist3);
    compound->setBoolean("Leashed", isLeashed);
    if (leashHolder != nullptr) {
        nbttagcompound2 = new NBTTagCompound();
        if (Util::instanceof<EntityLivingBase>(leashHolder)) {
            xg::Guid uuid = leashHolder->getUniqueID();
            nbttagcompound2->setUniqueId("UUID", uuid);
        } else if (Util::instanceof<EntityHanging>(leashHolder)) {
            BlockPos blockpos = ((EntityHanging *)leashHolder).getHangingPosition();
            nbttagcompound2->setInteger("X", blockpos.getx());
            nbttagcompound2->setInteger("Y", blockpos.gety());
            nbttagcompound2->setInteger("Z", blockpos.getz());
        }

        compound->setTag("Leash", nbttagcompound2);
    }

    compound->setBoolean("LeftHanded", isLeftHanded());
    if (deathLootTable != nullptr) {
        compound->setString("DeathLootTable", deathLootTable.to_string());
        if (deathLootTableSeed != 0L) {
            compound->setLong("DeathLootTableSeed", deathLootTableSeed);
        }
    }

    if (isAIDisabled()) {
        compound->setBoolean("NoAI", isAIDisabled());
    }
}

void EntityLiving::readEntityFromNBT(NBTTagCompound *compound) {
    EntityLivingBase::readEntityFromNBT(compound);
      if (compound->hasKey("CanPickUpLoot", 1)) {
         setCanPickUpLoot(compound->getBoolean("CanPickUpLoot"));
      }

      persistenceRequired = compound->getBoolean("PersistenceRequired");
      NBTTagList nbttaglist3;
      int l;
      if (compound->hasKey("ArmorItems", 9)) {
         nbttaglist3 = compound->getTagList("ArmorItems", 10);

         for(l = 0; l < inventoryArmor.size(); ++l) {
            inventoryArmor.emplace_back(l, ItemStack(nbttaglist3.getCompoundTagAt(l)));
         }
      }

      if (compound->hasKey("HandItems", 9)) {
         nbttaglist3 = compound->getTagList("HandItems", 10);

         for(l = 0; l < inventoryHands.size(); ++l) {
            inventoryHands.emplace_back(l, ItemStack(nbttaglist3.getCompoundTagAt(l)));
         }
      }

      if (compound->hasKey("ArmorDropChances", 9)) {
         nbttaglist3 = compound->getTagList("ArmorDropChances", 5);

         for(l = 0; l < nbttaglist3.tagCount(); ++l) {
            inventoryArmorDropChances[l] = nbttaglist3.getFloatAt(l);
         }
      }

      if (compound->hasKey("HandDropChances", 9)) {
         nbttaglist3 = compound->getTagList("HandDropChances", 5);

         for(l = 0; l < nbttaglist3.tagCount(); ++l) {
            inventoryHandsDropChances[l] = nbttaglist3.getFloatAt(l);
         }
      }

      isLeashed = compound->getBoolean("Leashed");
      if (isLeashed && compound->hasKey("Leash", 10)) {
         leashNBTTag = compound->getCompoundTag("Leash");
      }

      setLeftHanded(compound->getBoolean("LeftHanded"));
      if (compound->hasKey("DeathLootTable", 8)) {
         deathLootTable = new ResourceLocation(compound->getString("DeathLootTable"));
         deathLootTableSeed = compound->getLong("DeathLootTableSeed");
      }

      setNoAI(compound->getBoolean("NoAI"));
}

void EntityLiving::setMoveForward(float amount) {
    moveForward = amount;
}

void EntityLiving::setMoveVertical(float amount) {
    moveVertical = amount;
}

void EntityLiving::setMoveStrafing(float amount) {
    moveStrafing = amount;
}

void EntityLiving::setAIMoveSpeed(float speedIn) {
    EntityLivingBase::setAIMoveSpeed(speedIn);
    setMoveForward(speedIn);
}

void EntityLiving::onLivingUpdate() {
    EntityLivingBase::onLivingUpdate();
      world->profiler.startSection("looting");
      if (!world->isRemote && canPickUpLoot() && !dead && world->getGameRules().getBoolean("mobGriefing")) {
         auto var1 = world->getEntitiesWithinAABB<EntityItem>(getEntityBoundingBox().grow(1.0, 0.0, 1.0));

        for(auto entityitem : var1)
         {
            if (!entityitem->isDead && !entityitem->getItem().isEmpty() && !entityitem->cannotPickup()) {
               updateEquipmentIfNeeded(entityitem);
            }
         }
      }

      world->profiler.endSection();
}

int32_t EntityLiving::getVerticalFaceSpeed() {
    return 40;
}

int32_t EntityLiving::getHorizontalFaceSpeed() {
    return 10;
}

void EntityLiving::faceEntity(Entity *entityIn, float maxYawIncrease, float maxPitchIncrease) {
    double d0 = entityIn->posX - posX;
    double d2 = entityIn->posZ - posZ;
    double d1;
    if (Util::instanceof<EntityLivingBase>(entityIn)) {
        EntityLivingBase* entitylivingbase = (EntityLivingBase*)entityIn;
        d1 = entitylivingbase->posY + (double)entitylivingbase->getEyeHeight() - (posY + (double)getEyeHeight());
    } else {
        d1 = (entityIn->getEntityBoundingBox().getminY() + entityIn->getEntityBoundingBox().getmaxY()) / 2.0 - (posY + (double)getEyeHeight());
    }

    double d3 = (double)MathHelper::sqrt(d0 * d0 + d2 * d2);
    float f = (float)(MathHelper::atan2(d2, d0) * 57.29577951308232) - 90.0F;
    float f1 = (float)(-(MathHelper::atan2(d1, d3) * 57.29577951308232));
    rotationPitch = updateRotation(rotationPitch, f1, maxPitchIncrease);
    rotationYaw = updateRotation(rotationYaw, f, maxYawIncrease);
}

bool EntityLiving::getCanSpawnHere() {
    IBlockState* iblockstate = world->getBlockState((BlockPos(this)).down());
    return iblockstate->canEntitySpawn(this);
}

bool EntityLiving::isNotColliding() {
    return !world->containsAnyLiquid(getEntityBoundingBox()) && world->getCollisionBoxes(this, getEntityBoundingBox()).isEmpty() && world->checkNoEntityCollision(getEntityBoundingBox(), this);
}

float EntityLiving::getRenderSizeModifier() {
    return 1.0F;
}

int32_t EntityLiving::getMaxSpawnedInChunk() {
    return 4;
}

int32_t EntityLiving::getMaxFallHeight() {
    if (getAttackTarget() == nullptr) {
         return 3;
      } else {
         int32_t i = (getHealth() - getMaxHealth() * 0.33F);
         i -= (3 - world->getDifficulty().getId()) * 4;
         if (i < 0) {
            i = 0;
         }

         return i + 3;
      }
}

std::vector<ItemStack> & EntityLiving::getHeldEquipment() {
    return inventoryHands;
}

std::vector<ItemStack> & EntityLiving::getArmorInventoryList() {
    return inventoryArmor;
}

ItemStack EntityLiving::getItemStackFromSlot(EntityEquipmentSlot slotIn) {
    switch(slotIn.getSlotType()) {
      case EquipmentType::HAND:
         return inventoryHands[slotIn.getIndex()];
      case EquipmentType::ARMOR:
         return inventoryArmor[slotIn.getIndex()];
      default:
         return ItemStack::EMPTY;
      }
}

void EntityLiving::setItemStackToSlot(EntityEquipmentSlot slotIn, ItemStack stack) {
    switch(slotIn.getSlotType()) {
      case EquipmentType::HAND:
         inventoryHands[slotIn.getIndex()] = stack;
         break;
      case EquipmentType::ARMOR:
         inventoryArmor[slotIn.getIndex()] = stack;
      }
}

EntityEquipmentSlot EntityLiving::getSlotForItemStack(ItemStack stack) {
    if (stack.getItem() != Item::getItemFromBlock(Blocks::PUMPKIN) && stack.getItem() != Items::SKULL) {
         if (Util::instanceof<ItemArmor>(stack.getItem())) 
         {
            return ((ItemArmor*)stack.getItem())->armorType;
         }
        else if (stack.getItem() == Items::ELYTRA) 
         {
            return EntityEquipmentSlot::CHEST;
         }
        else 
         {
            return stack.getItem() == Items::SHIELD ? EntityEquipmentSlot::OFFHAND : EntityEquipmentSlot::MAINHAND;
         }
      } else {
         return EntityEquipmentSlot::HEAD;
      }
}

Item * EntityLiving::getArmorByChance(EntityEquipmentSlot slotIn, int32_t chance) {
    switch(slotIn) {
      case EntityEquipmentSlot::HEAD:
         if (chance == 0) {
            return Items::LEATHER_HELMET;
         } else if (chance == 1) {
            return Items::GOLDEN_HELMET;
         } else if (chance == 2) {
            return Items::CHAINMAIL_HELMET;
         } else if (chance == 3) {
            return Items::IRON_HELMET;
         } else if (chance == 4) {
            return Items::DIAMOND_HELMET;
         }
      case EntityEquipmentSlot::CHEST:
         if (chance == 0) {
            return Items::LEATHER_CHESTPLATE;
         } else if (chance == 1) {
            return Items::GOLDEN_CHESTPLATE;
         } else if (chance == 2) {
            return Items::CHAINMAIL_CHESTPLATE;
         } else if (chance == 3) {
            return Items::IRON_CHESTPLATE;
         } else if (chance == 4) {
            return Items::DIAMOND_CHESTPLATE;
         }
      case EntityEquipmentSlot::LEGS:
         if (chance == 0) {
            return Items::LEATHER_LEGGINGS;
         } else if (chance == 1) {
            return Items::GOLDEN_LEGGINGS;
         } else if (chance == 2) {
            return Items::CHAINMAIL_LEGGINGS;
         } else if (chance == 3) {
            return Items::IRON_LEGGINGS;
         } else if (chance == 4) {
            return Items::DIAMOND_LEGGINGS;
         }
      case EntityEquipmentSlot::FEET:
         if (chance == 0) {
            return Items::LEATHER_BOOTS;
         } else if (chance == 1) {
            return Items::GOLDEN_BOOTS;
         } else if (chance == 2) {
            return Items::CHAINMAIL_BOOTS;
         } else if (chance == 3) {
            return Items::IRON_BOOTS;
         } else if (chance == 4) {
            return Items::DIAMOND_BOOTS;
         }
      default:
         return nullptr;
      }
}

IEntityLivingData * EntityLiving::onInitialSpawn(DifficultyInstance difficulty, IEntityLivingData *livingdata) {
    getEntityAttribute(SharedMonsterAttributes::FOLLOW_RANGE).applyModifier(AttributeModifier("Random spawn bonus", MathHelper::nextGaussian<double>(rand) * 0.05, 1));
      if (MathHelper::nextFloat(rand) < 0.05F) {
         setLeftHanded(true);
      } else {
         setLeftHanded(false);
      }

      return livingdata;
}

bool EntityLiving::canBeSteered() {
    return false;
}

void EntityLiving::enablePersistence() {
    persistenceRequired = true;
}

void EntityLiving::setDropChance(EntityEquipmentSlot slotIn, float chance) {
    switch(slotIn.getSlotType()) {
      case EquipmentType::HAND:
         inventoryHandsDropChances[slotIn.getIndex()] = chance;
         break;
      case EquipmentType::ARMOR:
         inventoryArmorDropChances[slotIn.getIndex()] = chance;
      }
}

bool EntityLiving::canPickUpLoot() const{
    return canPickUpLoot;
}

void EntityLiving::setCanPickUpLoot(bool canPickup) {
    canPickUpLoot = canPickup;
}

bool EntityLiving::isNoDespawnRequired() const {
    return persistenceRequired;
}

bool EntityLiving::processInitialInteract(EntityPlayer *player, EnumHand hand) {
    if (getLeashed() && getLeashHolder() == player) {
         clearLeashed(true, !player->capabilities.isCreativeMode);
         return true;
      } else {
         ItemStack itemstack = player->getHeldItem(hand);
         if (itemstack.getItem() == Items::LEAD && canBeLeashedTo(player)) {
            setLeashHolder(player, true);
            itemstack.shrink(1);
            return true;
         } else {
            return processInteract(player, hand) ? true : EntityLivingBase::processInitialInteract(player, hand);
         }
      }
}

void EntityLiving::clearLeashed(bool sendPacket, bool dropLead) {
    if (isLeashed) {
         isLeashed = false;
         leashHolder = nullptr;
         if (!world->isRemote && dropLead) {
            dropItem(Items::LEAD, 1);
         }

         if (!world->isRemote && sendPacket && Util::instanceof<WorldServer>(world)) {
            ((WorldServer*)world)->getEntityTracker()->sendToTracking(this, new SPacketEntityAttach(this, nullptr));
         }
      }
}

bool EntityLiving::canBeLeashedTo(EntityPlayer *player) {
    return !getLeashed() && !Util::instanceof<IMob>(this);
}

bool EntityLiving::getLeashed() const {
    return isLeashed;
}

Entity * EntityLiving::getLeashHolder() const {
    return leashHolder;
}

void EntityLiving::setLeashHolder(Entity *entityIn, bool sendAttachNotification) {
    isLeashed = true;
      leashHolder = entityIn;
      if (!world->isRemote && sendAttachNotification && Util::instanceof<WorldServer>(world)) {
         ((WorldServer*)world)->getEntityTracker().sendToTracking(this, new SPacketEntityAttach(this, leashHolder));
      }

      if (isRiding()) {
         dismountRidingEntity();
      }
}

bool EntityLiving::startRiding(Entity *entityIn, bool force) {
    bool flag = EntityLivingBase::startRiding(entityIn, force);
      if (flag && getLeashed()) {
         clearLeashed(true, true);
      }

      return flag;
}

bool EntityLiving::replaceItemInInventory(int32_t inventorySlot, ItemStack itemStackIn) {
    EntityEquipmentSlot entityequipmentslot = EntityEquipmentSlot::FEET;
      if (inventorySlot == 98) {
         entityequipmentslot = EntityEquipmentSlot::MAINHAND;
      } else if (inventorySlot == 99) {
         entityequipmentslot = EntityEquipmentSlot::OFFHAND;
      } else if (inventorySlot == 100 + EntityEquipmentSlot::HEAD.getIndex()) {
         entityequipmentslot = EntityEquipmentSlot::HEAD;
      } else if (inventorySlot == 100 + EntityEquipmentSlot::CHEST.getIndex()) {
         entityequipmentslot = EntityEquipmentSlot::CHEST;
      } else if (inventorySlot == 100 + EntityEquipmentSlot::LEGS.getIndex()) {
         entityequipmentslot = EntityEquipmentSlot::LEGS;
      } else {
         if (inventorySlot != 100 + EntityEquipmentSlot::FEET.getIndex()) {
            return false;
         }

         entityequipmentslot = EntityEquipmentSlot::FEET;
      }

      if (!itemStackIn.isEmpty() && !isItemStackInSlot(entityequipmentslot, itemStackIn) && entityequipmentslot != EntityEquipmentSlot::HEAD) {
         return false;
      } else {
         setItemStackToSlot(entityequipmentslot, itemStackIn);
         return true;
      }
}

bool EntityLiving::canPassengerSteer() {
    return canBeSteered() && EntityLivingBase::canPassengerSteer();
}

bool EntityLiving::isItemStackInSlot(EntityEquipmentSlot slotIn, ItemStack stack) {
    EntityEquipmentSlot entityequipmentslot = getSlotForItemStack(stack);
      return entityequipmentslot == slotIn || entityequipmentslot == EntityEquipmentSlot::MAINHAND && slotIn == EntityEquipmentSlot::OFFHAND || entityequipmentslot == EntityEquipmentSlot::OFFHAND && slotIn == EntityEquipmentSlot::MAINHAND;
}

bool EntityLiving::isServerWorld() {
    return EntityLivingBase::isServerWorld() && !isAIDisabled();
}

void EntityLiving::setNoAI(bool disable) {
    std::byte b0 = dataManager.get(AI_FLAGS);
      dataManager.set(AI_FLAGS, disable ? (byte)(b0 |  std::byte{1}) : (byte)(b0 &  std::byte{-2}));
}

void EntityLiving::setLeftHanded(bool leftHanded) {
    std::byte b0 = dataManager.get(AI_FLAGS);
      dataManager.set(AI_FLAGS, leftHanded ? b0 | std::byte{2} : (b0 & std::byte{-3}));
}

bool EntityLiving::isAIDisabled() {
    return (dataManager.get(AI_FLAGS) & 1) != 0;
}

bool EntityLiving::isLeftHanded() {
    return (dataManager.get(AI_FLAGS) & 2) != 0;
}

EnumHandSide EntityLiving::getPrimaryHand() {
    return isLeftHanded() ? EnumHandSide::LEFT : EnumHandSide::RIGHT;
}

void EntityLiving::setEnchantmentBasedOnDifficulty(DifficultyInstance difficulty) {
    float f = difficulty.getClampedAdditionalDifficulty();
      if (!getHeldItemMainhand().isEmpty() && MathHelper::nextFloat(rand) < 0.25F * f) {
         setItemStackToSlot(EntityEquipmentSlot::MAINHAND, EnchantmentHelper::addRandomEnchantment(rand, getHeldItemMainhand(), (int)(5.0F + f * (float)rand(18)), false));
      }

    for(auto entityequipmentslot : EntityEquipmentSlot::values())
    {
         if (entityequipmentslot->getSlotType() == EntityEquipmentSlot::Type::ARMOR) {
            ItemStack itemstack = getItemStackFromSlot(entityequipmentslot);
            if (!itemstack.isEmpty() && MathHelper::nextFloat(rand) < 0.5F * f) {
               setItemStackToSlot(entityequipmentslot, EnchantmentHelper::addRandomEnchantment(rand, itemstack, (int)(5.0F + f * (float)rand(18)), false));
            }
         }
      }
}

bool EntityLiving::processInteract(EntityPlayer *player, EnumHand hand) {
    return false;
}

void EntityLiving::updateLeashedState() {
    if (leashNBTTag != nullptr) {
         recreateLeash();
      }

      if (isLeashed) {
         if (!isEntityAlive()) {
            clearLeashed(true, true);
         }

         if (leashHolder == nullptr || leashHolder->isDead) {
            clearLeashed(true, true);
         }
      }
}

void EntityLiving::initEntityAI() {
}

void EntityLiving::applyEntityAttributes() {
    EntityLivingBase::applyEntityAttributes();
    getAttributeMap()->registerAttribute(SharedMonsterAttributes::FOLLOW_RANGE).setBaseValue(16.0);
}

PathNavigate *EntityLiving::createNavigator(World *worldIn) {
    return new PathNavigateGround(this, worldIn);
}

EntityBodyHelper EntityLiving::createBodyHelper() {
    return EntityBodyHelper(this);
}

void EntityLiving::entityInit() {
    EntityLivingBase::entityInit();
    dataManager.
    register
    (AI_FLAGS, 0);
}

void EntityLiving::playHurtSound(DamageSource::DamageSource source) {
    applyEntityAI();
    EntityLivingBase::playHurtSound(source);
}

int32_t EntityLiving::getExperiencePoints(EntityPlayer *player) {
    if (experienceValue > 0) {
        auto i = experienceValue;

        for (size_t k = 0; k < inventoryArmor.size(); ++k) {
            if (!inventoryArmor[k].isEmpty() && inventoryArmorDropChances[k] <= 1.0F) {
                i += 1 + rand(3);
            }
        }

        for (size_t k = 0; k < inventoryHands.size(); ++k) {
            if (!inventoryHands[k].isEmpty() && inventoryHandsDropChances[k] <= 1.0F) {
                i += 1 + rand(3);
            }
        }

        return i;
    } else {
        return experienceValue;
    }
}

float EntityLiving::updateDistance(float p_110146_1_, float p_110146_2_) {
    bodyHelper.updateRenderAngles();
    return p_110146_2_;
}

SoundEvent EntityLiving::getAmbientSound() {

}

Item *EntityLiving::getDropItem() {
    return nullptr;
}

void EntityLiving::dropFewItems(bool wasRecentlyHit, int32_t lootingModifier) {
    Item *item = getDropItem();
    if (item != nullptr) {
        auto i = rand(3);
        if (lootingModifier > 0) {
            i += rand(lootingModifier + 1);
        }

        for (auto j = 0; j < i; ++j) {
            dropItem(item, 1);
        }
    }
}

std::optional<ResourceLocation> EntityLiving::getLootTable() {
    return std::nullopt;
}

void EntityLiving::dropLoot(bool wasRecentlyHit, int32_t lootingModifier, DamageSource::DamageSource source) {
    auto resourcelocation = deathLootTable;
      if (!resourcelocation) {
         resourcelocation = getLootTable();
      }

      if (resourcelocation) {
         auto loottable = world->getLootTableManager().getLootTableFromLocation(resourcelocation);
         deathLootTable = std::nullopt;
         LootContext::Builder lootcontext$builder = (LootContext::Builder((WorldServer*)world))->withLootedEntity(this).withDamageSource(source);
         if (wasRecentlyHit && attackingPlayer != nullptr) {
            lootcontext$builder = lootcontext$builder.withPlayer(attackingPlayer).withLuck(attackingPlayer->getLuck());
         }

         Iterator var7 = loottable.generateLootForPools(deathLootTableSeed == 0L ? rand : pcg32(deathLootTableSeed), lootcontext$builder.build()).iterator();

         while(var7.hasNext()) {
            ItemStack itemstack = (ItemStack)var7.next();
            entityDropItem(itemstack, 0.0F);
         }

         dropEquipment(wasRecentlyHit, lootingModifier);
      } else {
         EntityLivingBase::dropLoot(wasRecentlyHit, lootingModifier, source);
      }
}

void EntityLiving::updateEquipmentIfNeeded(EntityItem *itemEntity) {
    ItemStack itemstack = itemEntity->getItem();
      EntityEquipmentSlot entityequipmentslot = getSlotForItemStack(itemstack);
      bool flag = true;
      ItemStack itemstack1 = getItemStackFromSlot(entityequipmentslot);
      if (!itemstack1.isEmpty()) {
         if (entityequipmentslot->getSlotType() == EntityEquipmentSlot::Type::HAND) {
            if (itemstack.getItem() instanceof ItemSword && !(itemstack1.getItem() instanceof ItemSword)) {
               flag = true;
            } else if (itemstack.getItem() instanceof ItemSword && itemstack1.getItem() instanceof ItemSword) {
               ItemSword* itemsword = (ItemSword*)itemstack.getItem();
               ItemSword* itemsword1 = (ItemSword*)itemstack1.getItem();
               if (itemsword.getAttackDamage() == itemsword1.getAttackDamage()) {
                  flag = itemstack.getMetadata() > itemstack1.getMetadata() || itemstack.hasTagCompound() && !itemstack1.hasTagCompound();
               } else {
                  flag = itemsword.getAttackDamage() > itemsword1.getAttackDamage();
               }
            } else if (itemstack.getItem() instanceof ItemBow && itemstack1.getItem() instanceof ItemBow) {
               flag = itemstack.hasTagCompound() && !itemstack1.hasTagCompound();
            } else {
               flag = false;
            }
         } else if (itemstack.getItem() instanceof ItemArmor && !(itemstack1.getItem() instanceof ItemArmor)) {
            flag = true;
         } else if (itemstack.getItem() instanceof ItemArmor && itemstack1.getItem() instanceof ItemArmor && !EnchantmentHelper.hasBindingCurse(itemstack1)) {
            ItemArmor* itemarmor = (ItemArmor)itemstack.getItem();
            ItemArmor* itemarmor1 = (ItemArmor)itemstack1.getItem();
            if (itemarmor.damageReduceAmount == itemarmor1.damageReduceAmount) {
               flag = itemstack.getMetadata() > itemstack1.getMetadata() || itemstack.hasTagCompound() && !itemstack1.hasTagCompound();
            } else {
               flag = itemarmor.damageReduceAmount > itemarmor1.damageReduceAmount;
            }
         } else {
            flag = false;
         }
      }

      if (flag && canEquipItem(itemstack)) {
         double d0;
         switch(entityequipmentslot->getSlotType()) {
         case EquipmentType::HAND:
            d0 = (double)inventoryHandsDropChances[entityequipmentslot->getIndex()];
            break;
         case EquipmentType::ARMOR:
            d0 = (double)inventoryArmorDropChances[entityequipmentslot->getIndex()];
            break;
         default:
            d0 = 0.0;
         }

         if (!itemstack1.isEmpty() && (double)(MathHelper::nextFloat(rand) - 0.1F) < d0) {
            entityDropItem(itemstack1, 0.0F);
         }

         setItemStackToSlot(entityequipmentslot, itemstack);
         switch(entityequipmentslot->getSlotType()) {
         case EquipmentType::HAND:
            inventoryHandsDropChances[entityequipmentslot->getIndex()] = 2.0F;
            break;
         case EquipmentType::ARMOR:
            inventoryArmorDropChances[entityequipmentslot->getIndex()] = 2.0F;
         }

         persistenceRequired = true;
         onItemPickup(itemEntity, itemstack.getCount());
         itemEntity->setDead();
      }
}

bool EntityLiving::canEquipItem(ItemStack stack) {
    return true;
}

bool EntityLiving::canDespawn() {
    return true;
}

void EntityLiving::despawnEntity() {
    if (persistenceRequired) {
         idleTime = 0;
      } else {
         Entity* entity = world->getClosestPlayerToEntity(this, -1.0);
         if (entity != nullptr) {
            double d0 = entity->posX - posX;
            double d1 = entity->posY - posY;
            double d2 = entity->posZ - posZ;
            double d3 = d0 * d0 + d1 * d1 + d2 * d2;
            if (canDespawn() && d3 > 16384.0) {
               setDead();
            }

            if (idleTime > 600 && rand(800) == 0 && d3 > 1024.0 && canDespawn()) {
               setDead();
            } else if (d3 < 1024.0) {
               idleTime = 0;
            }
         }
      }
}

void EntityLiving::updateEntityActionState() {
    ++idleTime;
    world->profiler.startSection("checkDespawn");
    despawnEntity();
    world->profiler.endSection();
    world->profiler.startSection("sensing");
    senses.clearSensingCache();
    world->profiler.endSection();
    world->profiler.startSection("targetSelector");
    targetTasks.onUpdateTasks();
    world->profiler.endSection();
    world->profiler.startSection("goalSelector");
    tasks.onUpdateTasks();
    world->profiler.endSection();
    world->profiler.startSection("navigation");
    navigator->onUpdateNavigation();
    world->profiler.endSection();
    world->profiler.startSection("mob tick");
    updateAITasks();
    world->profiler.endSection();
    if (isRiding() && Util::instanceof<EntityLiving>(getRidingEntity())) {
        EntityLiving* entityliving = (EntityLiving*)getRidingEntity();
        entityliving->getNavigator()->setPath(getNavigator()->getPath(), 1.5);
        entityliving->getMoveHelper().read(getMoveHelper());
    }

    world->profiler.startSection("controls");
    world->profiler.startSection("move");
    moveHelper.onUpdateMoveHelper();
    world->profiler.endStartSection("look");
    lookHelper.onUpdateLook();
    world->profiler.endStartSection("jump");
    jumpHelper.doJump();
    world->profiler.endSection();
    world->profiler.endSection();
}

void EntityLiving::updateAITasks() {

}

void EntityLiving::dropEquipment(bool wasRecentlyHit, int32_t lootingModifier) {
    auto var3 = EntityEquipmentSlot::values();

      for(auto entityequipmentslot : var3) {
         ItemStack itemstack = getItemStackFromSlot(entityequipmentslot);
         double d0;
         switch(entityequipmentslot->getSlotType()) {
         case EquipmentType::HAND:
            d0 = inventoryHandsDropChances[entityequipmentslot->getIndex()];
            break;
         case EquipmentType::ARMOR:
            d0 = inventoryArmorDropChances[entityequipmentslot->getIndex()];
            break;
         default:
            d0 = 0.0;
         }

         bool flag = d0 > 1.0;
         if (!itemstack.isEmpty() && !EnchantmentHelper::hasVanishingCurse(itemstack) && (wasRecentlyHit || flag) && (double)(MathHelper::nextFloat(rand) - (float)lootingModifier * 0.01F) < d0) {
            if (!flag && itemstack.isItemStackDamageable()) {
               itemstack.setItemDamage(itemstack.getMaxDamage() - rand(1 + rand(MathHelper::max(itemstack.getMaxDamage() - 3, 1))));
            }

            entityDropItem(itemstack, 0.0F);
         }
      }
}

void EntityLiving::setEquipmentBasedOnDifficulty(DifficultyInstance difficulty) {
    if (MathHelper::nextFloat(rand) < 0.15F * difficulty.getClampedAdditionalDifficulty()) {
         auto i = rand(2);
         float f = world->getDifficulty() == EnumDifficulty::HARD ? 0.1F : 0.25F;
         if (MathHelper::nextFloat(rand) < 0.095F) {
            ++i;
         }

         if (MathHelper::nextFloat(rand) < 0.095F) {
            ++i;
         }

         if (MathHelper::nextFloat(rand) < 0.095F) {
            ++i;
         }

         bool flag = true;

        for (auto entityequipmentslot : EntityEquipmentSlot::values())
        {
            if (entityequipmentslot->getSlotType() == EntityEquipmentSlot::Type::ARMOR) {
               ItemStack itemstack = getItemStackFromSlot(entityequipmentslot);
               if (!flag && MathHelper::nextFloat(rand) < f) {
                  break;
               }

               flag = false;
               if (itemstack.isEmpty()) {
                  Item* item = getArmorByChance(entityequipmentslot, i);
                  if (item != nullptr) {
                     setItemStackToSlot(entityequipmentslot, ItemStack(item));
                  }
               }
            }
         }
      }
}

void EntityLiving::applyEntityAI() {
    livingSoundTime = -getTalkInterval();
}

float EntityLiving::updateRotation(float angle, float targetAngle, float maxIncrease) const {
    float f = MathHelper::wrapDegrees(targetAngle - angle);
    if (f > maxIncrease) {
        f = maxIncrease;
    }

    if (f < -maxIncrease) {
        f = -maxIncrease;
    }

    return angle + f;
}

void EntityLiving::recreateLeash() {
    if (isLeashed && leashNBTTag != nullptr) {
         if (leashNBTTag->hasUniqueId("UUID")) {
            auto uuid = leashNBTTag->getUniqueId("UUID");
            auto var2 = world->getEntitiesWithinAABB<EntityLivingBase>(getEntityBoundingBox().grow(10.0));

            for (auto entitylivingbase : var2)
            {
               if (entitylivingbase->getUniqueID() == uuid) {
                  setLeashHolder(entitylivingbase, true);
                  break;
               }
            }
         } else if (leashNBTTag->hasKey("X", 99) && leashNBTTag->hasKey("Y", 99) && leashNBTTag->hasKey("Z", 99)) {
            BlockPos blockpos = BlockPos(leashNBTTag->getInteger("X"), leashNBTTag->getInteger("Y"), leashNBTTag->getInteger("Z"));
            auto entityleashknot = EntityLeashKnot::getKnotForPosition(world, blockpos);
            if (entityleashknot == nullptr) {
               entityleashknot = EntityLeashKnot::createKnot(world, blockpos);
            }

            setLeashHolder(entityleashknot, true);
         } else {
            clearLeashed(false, true);
         }
      }

      leashNBTTag = nullptr;
}
