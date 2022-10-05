#include "EntityPlayer.h"

#include "../../inventory/ContainerPlayer.h"
#include "../../item/ItemArmor.h"
#include "../../item/ItemAxe.h"
#include "../../item/ItemElytra.h"
#include "../../item/ItemSword.h"
#include "../../scoreboard/ScorePlayerTeam.h"
#include "../../scoreboard/Team.h"
#include "../../stats/StatList.h"
#include "../../util/MD5.h"
#include "../../world/WorldProvider.h"
#include "../../world/WorldServer.h"
#include "EntityList.h"
#include "IEntityMultiPart.h"
#include "MultiPartEntityPart.h"
#include "SharedMonsterAttributes.h"
#include "ai/EntityAIFollowOwner.h"
#include "ai/attributes/AbstractAttributeMap.h"
#include "ai/attributes/IAttributeInstance.h"
#include "datafix/DataFixesManager.h"
#include "datafix/FixTypes.h"
#include "datafix/IDataWalker.h"
#include "item/EntityBoat.h"
#include "item/EntityItem.h"
#include "item/EntityMinecart.h"
#include "monster/EntityMob.h"
#include "text/TextComponentString.h"

DataParameter EntityPlayer::ABSORPTION        = EntityDataManager.createKey(EntityPlayer.class, DataSerializers.FLOAT);
DataParameter EntityPlayer::PLAYER_SCORE      = EntityDataManager.createKey(EntityPlayer.class, DataSerializers.VARINT);
DataParameter EntityPlayer::PLAYER_MODEL_FLAG = EntityDataManager.createKey(EntityPlayer.class, DataSerializers.BYTE);
DataParameter EntityPlayer::MAIN_HAND         = EntityDataManager.createKey(EntityPlayer.class, DataSerializers.BYTE);
DataParameter EntityPlayer::LEFT_SHOULDER_ENTITY =
    EntityDataManager.createKey(EntityPlayer.class, DataSerializers.COMPOUND_TAG);
DataParameter EntityPlayer::RIGHT_SHOULDER_ENTITY =
    EntityDataManager.createKey(EntityPlayer.class, DataSerializers.COMPOUND_TAG);

EntityPlayer::EnumChatVisibility EntityPlayer::EnumChatVisibility::FULL(0, "options.chat.visibility.full");
EntityPlayer::EnumChatVisibility EntityPlayer::EnumChatVisibility::SYSTEM(1, "options.chat.visibility.system");
EntityPlayer::EnumChatVisibility EntityPlayer::EnumChatVisibility::HIDDEN(2, "options.chat.visibility.hidden");

class EntityPlayerWalker : public IDataWalker
{

  public:
    NBTTagCompound *process(IDataFixer *fixer, NBTTagCompound *compound, int32_t versionIn)
    {
        DataFixesManager::processInventory(fixer, compound, versionIn, "Inventory");
        DataFixesManager::processInventory(fixer, compound, versionIn, "EnderItems");
        if (compound->hasKey("ShoulderEntityLeft", 10))
        {
            compound->setTag(
                "ShoulderEntityLeft",
                fixer->process(FixTypes::ENTITY, compound->getCompoundTag("ShoulderEntityLeft"), versionIn));
        }

        if (compound->hasKey("ShoulderEntityRight", 10))
        {
            compound->setTag(
                "ShoulderEntityRight",
                fixer->process(FixTypes::ENTITY, compound->getCompoundTag("ShoulderEntityRight"), versionIn));
        }

        return compound;
    }
};

EntityPlayer::SleepEnemyPredicate::SleepEnemyPredicate(EntityPlayer *playerIn) : player(playerIn)
{
}

bool EntityPlayer::SleepEnemyPredicate::operator()(EntityMob *p_apply_1_) const
{
    return p_apply_1_->isPreventingPlayerRest(player);
}

EntityPlayer::EnumChatVisibility::EnumChatVisibility(int32_t id, std::string_view resourceKey)
    : chatVisibility(id), resourceKey(resourceKey)
{
    ID_LOOKUP.emplace_back(this);
}

int32_t EntityPlayer::EnumChatVisibility::getChatVisibility() const
{
    return chatVisibility;
}

EntityPlayer::EnumChatVisibility *EntityPlayer::EnumChatVisibility::getEnumChatVisibility(int32_t id)
{
    return ID_LOOKUP[id % ID_LOOKUP.size()];
}

std::string_view EntityPlayer::EnumChatVisibility::getResourceKey() const
{
    return resourceKey;
}

EntityPlayer::EntityPlayer(World *worldIn, const GameProfile &gameProfileIn)
    : EntityLivingBase(worldIn), inventory(this), itemStackMainHand(ItemStack::EMPTY),
      cooldownTracker(createCooldownTracker()), gameProfile(gameProfileIn),
      inventoryContainer(std::make_unique<ContainerPlayer>(&inventory, !worldIn->isRemote, this)),
      openContainer(inventoryContainer.get())
{
    setUniqueId(getUUID(gameProfileIn));
    BlockPos blockpos = worldIn->getSpawnPoint();
    setLocationAndAngles(blockpos.getx() + 0.5, (blockpos.gety() + 1), blockpos.getz() + 0.5, 0.0F, 0.0F);
    unused180 = 180.0F;
}

void EntityPlayer::onUpdate()
{
    noClip = isSpectator();
    if (isSpectator())
    {
        onGround = false;
    }

    if (xpCooldown > 0)
    {
        --xpCooldown;
    }

    if (isPlayerSleeping())
    {
        ++sleepTimer;
        if (sleepTimer > 100)
        {
            sleepTimer = 100;
        }

        if (!world->isRemote)
        {
            if (!isInBed())
            {
                wakeUpPlayer(true, true, false);
            }
            else if (world->isDaytime())
            {
                wakeUpPlayer(false, true, true);
            }
        }
    }
    else if (sleepTimer > 0)
    {
        ++sleepTimer;
        if (sleepTimer >= 110)
        {
            sleepTimer = 0;
        }
    }

    EntityLivingBase::onUpdate();
    if (!world->isRemote && openContainer != nullptr && !openContainer->canInteractWith(this))
    {
        closeScreen();
        openContainer = inventoryContainer.get();
    }

    if (isBurning() && capabilities.disableDamage)
    {
        extinguish();
    }

    updateCape();
    if (!world->isRemote)
    {
        foodStats.onUpdate(this);
        addStat(StatList::PLAY_ONE_MINUTE);
        if (isEntityAlive())
        {
            addStat(StatList::TIME_SINCE_DEATH);
        }

        if (isSneaking())
        {
            addStat(StatList::SNEAK_TIME);
        }
    }

    auto i    = 29999999;
    double d0 = MathHelper::clamp(posX, -2.9999999E7, 2.9999999E7);
    double d1 = MathHelper::clamp(posZ, -2.9999999E7, 2.9999999E7);
    if (d0 != posX || d1 != posZ)
    {
        setPosition(d0, posY, d1);
    }

    ++ticksSinceLastSwing;
    ItemStack itemstack = getHeldItemMainhand();
    if (!ItemStack::areItemStacksEqual(itemStackMainHand, itemstack))
    {
        if (!ItemStack::areItemsEqualIgnoreDurability(itemStackMainHand, itemstack))
        {
            resetCooldown();
        }

        itemStackMainHand = itemstack.isEmpty() ? ItemStack::EMPTY : itemstack.copy();
    }

    cooldownTracker.tick();
    updateSize();
}

int32_t EntityPlayer::getMaxInPortalTime() const
{
    return capabilities.disableDamage ? 1 : 80;
}

int32_t EntityPlayer::getPortalCooldown()
{
    return 10;
}

void EntityPlayer::playSound(SoundEvent soundIn, float volume, float pitch)
{
    world->playSound(this, posX, posY, posZ, soundIn, getSoundCategory(), volume, pitch);
}

SoundCategory EntityPlayer::getSoundCategory()
{
    return SoundCategory::PLAYERS;
}

void EntityPlayer::handleStatusUpdate(std::byte id)
{
    if (id == std::byte{9})
    {
        onItemUseFinish();
    }
    else if (id == std::byte{23})
    {
        hasReducedDebug = false;
    }
    else if (id == std::byte{22})
    {
        hasReducedDebug = true;
    }
    else
    {
        EntityLivingBase::handleStatusUpdate(id);
    }
}

void EntityPlayer::updateRidden()
{
    if (!world->isRemote && isSneaking() && isRiding())
    {
        dismountRidingEntity();
        setSneaking(false);
    }
    else
    {
        double d0 = posX;
        double d1 = posY;
        double d2 = posZ;
        float f   = rotationYaw;
        float f1  = rotationPitch;
        EntityLivingBase::updateRidden();
        prevCameraYaw = cameraYaw;
        cameraYaw     = 0.0F;
        addMountedMovementStat(posX - d0, posY - d1, posZ - d2);
        if (Util:: instanceof <EntityPig>(getRidingEntity()))
        {
            rotationPitch   = f1;
            rotationYaw     = f;
            renderYawOffset = ((EntityPig *)getRidingEntity())->renderYawOffset;
        }
    }
}

void EntityPlayer::preparePlayerToSpawn()
{
    setSize(0.6F, 1.8F);
    EntityLivingBase::preparePlayerToSpawn();
    setHealth(getMaxHealth());
    deathTime = 0;
}

void EntityPlayer::onLivingUpdate()
{
    if (flyToggleTimer > 0)
    {
        --flyToggleTimer;
    }

    if (world->getDifficulty() == EnumDifficulty::PEACEFUL && world->getGameRules().getBoolean("naturalRegeneration"))
    {
        if (getHealth() < getMaxHealth() && ticksExisted % 20 == 0)
        {
            heal(1.0F);
        }

        if (foodStats.needFood() && ticksExisted % 10 == 0)
        {
            foodStats.setFoodLevel(foodStats.getFoodLevel() + 1);
        }
    }

    inventory.decrementAnimations();
    prevCameraYaw = cameraYaw;
    EntityLivingBase::onLivingUpdate();
    auto iattributeinstance = getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED.get());
    if (!world->isRemote)
    {
        iattributeinstance->setBaseValue((double)capabilities.getWalkSpeed());
    }

    jumpMovementFactor = speedInAir;
    if (isSprinting())
    {
        jumpMovementFactor = (float)((double)jumpMovementFactor + (double)speedInAir * 0.3);
    }

    setAIMoveSpeed((float)iattributeinstance->getAttributeValue());
    float f  = MathHelper::sqrt(motionX * motionX + motionZ * motionZ);
    float f1 = (float)(MathHelper::atan(-motionY * 0.20000000298023224) * 15.0);
    if (f > 0.1F)
    {
        f = 0.1F;
    }

    if (!onGround || getHealth() <= 0.0F)
    {
        f = 0.0F;
    }

    if (onGround || getHealth() <= 0.0F)
    {
        f1 = 0.0F;
    }

    cameraYaw += (f - cameraYaw) * 0.4F;
    cameraPitch += (f1 - cameraPitch) * 0.8F;
    if (getHealth() > 0.0F && !isSpectator())
    {
        AxisAlignedBB axisalignedbb;
        if (isRiding() && !getRidingEntity()->isDead)
        {
            axisalignedbb =
                getEntityBoundingBox().aunion(getRidingEntity()->getEntityBoundingBox()).grow(1.0, 0.0, 1.0);
        }
        else
        {
            axisalignedbb = getEntityBoundingBox().grow(1.0, 0.5, 1.0);
        }

        auto list = world->getEntitiesWithinAABBExcludingEntity(this, axisalignedbb);

        for (auto entity : list)
        {
            if (!entity->isDead)
            {
                collideWithPlayer(entity);
            }
        }
    }

    playShoulderEntityAmbientSound(getLeftShoulderEntity());
    playShoulderEntityAmbientSound(getRightShoulderEntity());
    if (!world->isRemote && (fallDistance > 0.5F || isInWater() || isRiding()) || capabilities.isFlying)
    {
        spawnShoulderEntities();
    }
}

int32_t EntityPlayer::getScore()
{
    return dataManager.get(PLAYER_SCORE);
}

void EntityPlayer::setScore(int32_t scoreIn)
{
    dataManager.set(PLAYER_SCORE, scoreIn);
}

void EntityPlayer::addScore(int32_t scoreIn)
{
    int32_t i = getScore();
    dataManager.set(PLAYER_SCORE, i + scoreIn);
}

void EntityPlayer::onDeath(DamageSource::DamageSource cause)
{
    EntityLivingBase::onDeath(cause);
    setSize(0.2F, 0.2F);
    setPosition(posX, posY, posZ);
    motionY = 0.10000000149011612;
    if ("Notch" == (getName()))
    {
        dropItem(ItemStack(Items::APPLE, 1), true, false);
    }

    if (!world->getGameRules().getBoolean("keepInventory") && !isSpectator())
    {
        destroyVanishingCursedItems();
        inventory.dropAllItems();
    }

    if (cause != nullptr)
    {
        motionX = (double)(-MathHelper::cos((attackedAtYaw + rotationYaw) * 0.017453292F) * 0.1F);
        motionZ = (double)(-MathHelper::sin((attackedAtYaw + rotationYaw) * 0.017453292F) * 0.1F);
    }
    else
    {
        motionX = 0.0;
        motionZ = 0.0;
    }

    addStat(StatList::DEATHS);
    takeStat(StatList::TIME_SINCE_DEATH);
    extinguish();
    setFlag(0, false);
}

EntityItem *EntityPlayer::dropItem(bool dropAll)
{
    return dropItem(inventory.decrStackSize(inventory.currentItem, dropAll && !inventory.getCurrentItem().isEmpty()
                                                                       ? inventory.getCurrentItem().getCount()
                                                                       : 1),
                    false, true);
}

EntityItem *EntityPlayer::dropItem(const ItemStack &itemStackIn, bool unused)
{
    return dropItem(itemStackIn, false, unused);
}

EntityItem *EntityPlayer::dropItem(ItemStack droppedItem, bool dropAround, bool traceItem)
{
    if (droppedItem.isEmpty())
    {
        return nullptr;
    }
    else
    {
        double d0              = posY - 0.30000001192092896 + (double)getEyeHeight();
        EntityItem *entityitem = new EntityItem(world, posX, d0, posZ, droppedItem);
        entityitem->setPickupDelay(40);
        if (traceItem)
        {
            entityitem->setThrower(getName());
        }

        float f;
        float f1;
        if (dropAround)
        {
            f                   = MathHelper::nextFloat(rand) * 0.5F;
            f1                  = MathHelper::nextFloat(rand) * 6.2831855F;
            entityitem->motionX = (double)(-MathHelper::sin(f1) * f);
            entityitem->motionZ = (double)(MathHelper::cos(f1) * f);
            entityitem->motionY = 0.20000000298023224;
        }
        else
        {
            f                   = 0.3F;
            entityitem->motionX = (double)(-MathHelper::sin(rotationYaw * 0.017453292F) *
                                           MathHelper::cos(rotationPitch * 0.017453292F) * f);
            entityitem->motionZ = (double)(MathHelper::cos(rotationYaw * 0.017453292F) *
                                           MathHelper::cos(rotationPitch * 0.017453292F) * f);
            entityitem->motionY = (double)(-MathHelper::sin(rotationPitch * 0.017453292F) * f + 0.1F);
            f1                  = MathHelper::nextFloat(rand) * 6.2831855F;
            f                   = 0.02F * MathHelper::nextFloat(rand);
            entityitem->motionX += MathHelper::cos((double)f1) * (double)f;
            entityitem->motionY += (double)((MathHelper::nextFloat(rand) - MathHelper::nextFloat(rand)) * 0.1F);
            entityitem->motionZ += MathHelper::sin((double)f1) * (double)f;
        }

        ItemStack itemstack = dropItemAndGetStack(entityitem);
        if (traceItem)
        {
            if (!itemstack.isEmpty())
            {
                addStat(StatList::getDroppedObjectStats(itemstack.getItem()), droppedItem.getCount());
            }

            addStat(StatList::DROP);
        }

        return entityitem;
    }
}

float EntityPlayer::getDigSpeed(IBlockState *state)
{
    float f = inventory.getDestroySpeed(state);
    if (f > 1.0F)
    {
        auto i              = EnchantmentHelper::getEfficiencyModifier(this);
        ItemStack itemstack = getHeldItemMainhand();
        if (i > 0 && !itemstack.isEmpty())
        {
            f += (float)(i * i + 1);
        }
    }

    if (isPotionActive(MobEffects::HASTE))
    {
        f *= 1.0F + (float)(getActivePotionEffect(MobEffects::HASTE).getAmplifier() + 1) * 0.2F;
    }

    if (isPotionActive(MobEffects::MINING_FATIGUE))
    {
        float f1;
        switch (getActivePotionEffect(MobEffects::MINING_FATIGUE).getAmplifier())
        {
        case 0:
            f1 = 0.3F;
            break;
        case 1:
            f1 = 0.09F;
            break;
        case 2:
            f1 = 0.0027F;
            break;
        case 3:
        default:
            f1 = 8.1E-4F;
        }

        f *= f1;
    }

    if (isInsideOfMaterial(Material::WATER) && !EnchantmentHelper::getAquaAffinityModifier(this))
    {
        f /= 5.0F;
    }

    if (!onGround)
    {
        f /= 5.0F;
    }

    return f;
}

bool EntityPlayer::canHarvestBlock(IBlockState *state)
{
    return inventory.canHarvestBlock(state);
}

void EntityPlayer::registerFixesPlayer(DataFixer fixer)
{
    fixer.registerWalker(FixTypes::PLAYER, new EntityPlayerWalker());
}

void EntityPlayer::readEntityFromNBT(NBTTagCompound *compound)
{
    EntityLivingBase::readEntityFromNBT(compound);
    setUniqueId(getUUID(gameProfile));
    NBTTagList *nbttaglist = compound->getTagList("Inventory", 10);
    inventory.readFromNBT(nbttaglist);
    inventory.currentItem = compound->getInteger("SelectedItemSlot");
    sleeping              = compound->getBoolean("Sleeping");
    sleepTimer            = compound->getShort("SleepTimer");
    experience            = compound->getFloat("XpP");
    experienceLevel       = compound->getInteger("XpLevel");
    experienceTotal       = compound->getInteger("XpTotal");
    xpSeed                = compound->getInteger("XpSeed");
    if (xpSeed == 0)
    {
        xpSeed = rand();
    }

    setScore(compound->getInteger("Score"));
    if (sleeping)
    {
        bedLocation = BlockPos(this);
        wakeUpPlayer(true, true, false);
    }

    if (compound->hasKey("SpawnX", 99) && compound->hasKey("SpawnY", 99) && compound->hasKey("SpawnZ", 99))
    {
        spawnPos =
            BlockPos(compound->getInteger("SpawnX"), compound->getInteger("SpawnY"), compound->getInteger("SpawnZ"));
        spawnForced = compound->getBoolean("SpawnForced");
    }

    foodStats.readNBT(compound);
    capabilities.readCapabilitiesFromNBT(compound);
    if (compound->hasKey("EnderItems", 9))
    {
        NBTTagList *nbttaglist1 = compound->getTagList("EnderItems", 10);
        enderChest.loadInventoryFromNBT(nbttaglist1);
    }

    if (compound->hasKey("ShoulderEntityLeft", 10))
    {
        setLeftShoulderEntity(compound->getCompoundTag("ShoulderEntityLeft"));
    }

    if (compound->hasKey("ShoulderEntityRight", 10))
    {
        setRightShoulderEntity(compound->getCompoundTag("ShoulderEntityRight"));
    }
}

void EntityPlayer::writeEntityToNBT(NBTTagCompound *compound)
{
    EntityLivingBase::writeEntityToNBT(compound);
    compound->setInteger("DataVersion", 1343);
    compound->setTag("Inventory", inventory.writeToNBT(new NBTTagList()));
    compound->setInteger("SelectedItemSlot", inventory.currentItem);
    compound->setBoolean("Sleeping", sleeping);
    compound->setShort("SleepTimer", (short)sleepTimer);
    compound->setFloat("XpP", experience);
    compound->setInteger("XpLevel", experienceLevel);
    compound->setInteger("XpTotal", experienceTotal);
    compound->setInteger("XpSeed", xpSeed);
    compound->setInteger("Score", getScore());
    if (spawnPos.has_value())
    {
        compound->setInteger("SpawnX", spawnPos->getx());
        compound->setInteger("SpawnY", spawnPos->gety());
        compound->setInteger("SpawnZ", spawnPos->getz());
        compound->setBoolean("SpawnForced", spawnForced);
    }

    foodStats.writeNBT(compound);
    capabilities.writeCapabilitiesToNBT(compound);
    compound->setTag("EnderItems", enderChest.saveInventoryToNBT());
    if (!getLeftShoulderEntity().isEmpty())
    {
        compound->setTag("ShoulderEntityLeft", getLeftShoulderEntity());
    }

    if (!getRightShoulderEntity().isEmpty())
    {
        compound->setTag("ShoulderEntityRight", getRightShoulderEntity());
    }
}

bool EntityPlayer::attackEntityFrom(DamageSource::DamageSource source, float amount)
{
    if (isEntityInvulnerable(source))
    {
        return false;
    }
    else if (capabilities.disableDamage && !source.canHarmInCreative())
    {
        return false;
    }
    else
    {
        idleTime = 0;
        if (getHealth() <= 0.0F)
        {
            return false;
        }
        else
        {
            if (isPlayerSleeping() && !world->isRemote)
            {
                wakeUpPlayer(true, true, false);
            }

            spawnShoulderEntities();
            if (source.isDifficultyScaled())
            {
                if (world->getDifficulty() == EnumDifficulty::PEACEFUL)
                {
                    amount = 0.0F;
                }

                if (world->getDifficulty() == EnumDifficulty::EASY)
                {
                    amount = MathHelper::min(amount / 2.0F + 1.0F, amount);
                }

                if (world->getDifficulty() == EnumDifficulty::HARD)
                {
                    amount = amount * 3.0F / 2.0F;
                }
            }

            return amount == 0.0F ? false : EntityLivingBase::attackEntityFrom(source, amount);
        }
    }
}

float EntityPlayer::getArmorVisibility()
{
    auto i = 0;

    for (auto itemstack : inventory.armorInventory)
    {
        if (!itemstack.isEmpty())
        {
            ++i;
        }
    }

    return (float)i / (float)inventory.armorInventory.size();
}

void EntityPlayer::openEditSign(TileEntitySign *signTile)
{
}

void EntityPlayer::displayGuiEditCommandCart(CommandBlockBaseLogic *commandBlock)
{
}

void EntityPlayer::displayGuiCommandBlock(TileEntityCommandBlock *commandBlock)
{
}

void EntityPlayer::openEditStructure(TileEntityStructure *structure)
{
}

void EntityPlayer::displayVillagerTradeGui(IMerchant *villager)
{
}

void EntityPlayer::displayGUIChest(IInventory *chestInventory)
{
}

void EntityPlayer::openGuiHorseInventory(AbstractHorse *horse, IInventory *inventoryIn)
{
}

void EntityPlayer::displayGui(IInteractionObject *guiOwner)
{
}

void EntityPlayer::openBook(ItemStack stack, EnumHand hand)
{
}

EnumActionResult EntityPlayer::interactOn(Entity *entityToInteractOn, EnumHand hand)
{
    if (isSpectator())
    {
        if (Util:: instanceof <IInventory>(entityToInteractOn))
        {
            displayGUIChest((IInventory *)entityToInteractOn);
        }

        return EnumActionResult::PASS;
    }
    else
    {
        ItemStack itemstack  = getHeldItem(hand);
        ItemStack itemstack1 = itemstack.isEmpty() ? ItemStack::EMPTY : itemstack.copy();
        if (entityToInteractOn->processInitialInteract(this, hand))
        {
            if (capabilities.isCreativeMode && itemstack == getHeldItem(hand) &&
                itemstack.getCount() < itemstack1.getCount())
            {
                itemstack.setCount(itemstack1.getCount());
            }

            return EnumActionResult::SUCCESS;
        }
        else
        {
            if (!itemstack.isEmpty() && Util:: instanceof <EntityLivingBase>(entityToInteractOn))
            {
                if (capabilities.isCreativeMode)
                {
                    itemstack = itemstack1;
                }

                if (itemstack.interactWithEntity(this, (EntityLivingBase *)entityToInteractOn, hand))
                {
                    if (itemstack.isEmpty() && !capabilities.isCreativeMode)
                    {
                        setHeldItem(hand, ItemStack::EMPTY);
                    }

                    return EnumActionResult::SUCCESS;
                }
            }

            return EnumActionResult::PASS;
        }
    }
}

double EntityPlayer::getYOffset()
{
    return -0.35;
}

void EntityPlayer::dismountRidingEntity()
{
    EntityLivingBase::dismountRidingEntity();
    rideCooldown = 0;
}

void EntityPlayer::attackTargetEntityWithCurrentItem(Entity *targetEntity)
{
    if (targetEntity->canBeAttackedWithItem() && !targetEntity->hitByEntity(this))
    {
        float f = (float)getEntityAttribute(SharedMonsterAttributes::ATTACK_DAMAGE.get())->getAttributeValue();
        float f1;
        if (Util:: instanceof <EntityLivingBase>(targetEntity))
        {
            f1 = EnchantmentHelper::getModifierForCreature(getHeldItemMainhand(),
                                                           ((EntityLivingBase *)targetEntity)->getCreatureAttribute());
        }
        else
        {
            f1 = EnchantmentHelper::getModifierForCreature(getHeldItemMainhand(), EnumCreatureAttribute::UNDEFINED);
        }

        float f2 = getCooledAttackStrength(0.5F);
        f *= 0.2F + f2 * f2 * 0.8F;
        f1 *= f2;
        resetCooldown();
        if (f > 0.0F || f1 > 0.0F)
        {
            bool flag  = f2 > 0.9F;
            bool flag1 = false;
            auto i     = EnchantmentHelper::getKnockbackModifier(this);
            if (isSprinting() && flag)
            {
                world->playSound(nullptr, posX, posY, posZ, SoundEvents::ENTITY_PLAYER_ATTACK_KNOCKBACK,
                                 getSoundCategory(), 1.0F, 1.0F);
                ++i;
                flag1 = true;
            }

            bool flag2 = flag && fallDistance > 0.0F && !onGround && !isOnLadder() && !isInWater() &&
                         !isPotionActive(MobEffects::BLINDNESS) && !isRiding() && Util::
                instanceof <EntityLivingBase>(targetEntity);
            flag2 = flag2 && !isSprinting();
            if (flag2)
            {
                f *= 1.5F;
            }

            f += f1;
            bool flag3 = false;
            double d0  = (double)(distanceWalkedModified - prevDistanceWalkedModified);
            if (flag && !flag2 && !flag1 && onGround && d0 < (double)getAIMoveSpeed())
            {
                ItemStack itemstack = getHeldItem(EnumHand::MAIN_HAND);
                if (Util:: instanceof <ItemSword>(itemstack.getItem()))
                {
                    flag3 = true;
                }
            }

            float f4   = 0.0F;
            bool flag4 = false;
            auto j     = EnchantmentHelper::getFireAspectModifier(this);
            if (Util:: instanceof <EntityLivingBase>(targetEntity))
            {
                f4 = ((EntityLivingBase *)targetEntity)->getHealth();
                if (j > 0 && !targetEntity->isBurning())
                {
                    flag4 = true;
                    targetEntity->setFire(1);
                }
            }

            double d1  = targetEntity->motionX;
            double d2  = targetEntity->motionY;
            double d3  = targetEntity->motionZ;
            bool flag5 = targetEntity->attackEntityFrom(DamageSource::causePlayerDamage(this), f);
            if (flag5)
            {
                if (i > 0)
                {
                    if (Util:: instanceof <EntityLivingBase>(targetEntity))
                    {
                        ((EntityLivingBase *)targetEntity)
                            ->knockBack(this, (float)i * 0.5F, (double)MathHelper::sin(rotationYaw * 0.017453292F),
                                        (double)(-MathHelper::cos(rotationYaw * 0.017453292F)));
                    }
                    else
                    {
                        targetEntity->addVelocity(
                            (double)(-MathHelper::sin(rotationYaw * 0.017453292F) * (float)i * 0.5F), 0.1,
                            (double)(MathHelper::cos(rotationYaw * 0.017453292F) * (float)i * 0.5F));
                    }

                    motionX *= 0.6;
                    motionZ *= 0.6;
                    setSprinting(false);
                }

                if (flag3)
                {
                    float f3   = 1.0F + EnchantmentHelper::getSweepingDamageRatio(this) * f;
                    auto var23 = world->getEntitiesWithinAABB<EntityLivingBase>(
                        targetEntity->getEntityBoundingBox().grow(1.0, 0.25, 1.0));

                    for (auto entitylivingbase : var23)
                    {
                        if (entitylivingbase != this && entitylivingbase != targetEntity &&
                            !isOnSameTeam(entitylivingbase) && getDistanceSq(entitylivingbase) < 9.0)
                        {
                            entitylivingbase->knockBack(this, 0.4F, (double)MathHelper::sin(rotationYaw * 0.017453292F),
                                                        (double)(-MathHelper::cos(rotationYaw * 0.017453292F)));
                            entitylivingbase->attackEntityFrom(DamageSource::causePlayerDamage(this), f3);
                        }
                    }

                    world->playSound(nullptr, posX, posY, posZ, SoundEvents::ENTITY_PLAYER_ATTACK_SWEEP,
                                     getSoundCategory(), 1.0F, 1.0F);
                    spawnSweepParticles();
                }

                if (Util:: instanceof <EntityPlayerMP>(targetEntity) && targetEntity->velocityChanged)
                {
                    ((EntityPlayerMP *)targetEntity)->connection.sendPacket(new SPacketEntityVelocity(targetEntity));
                    targetEntity->velocityChanged = false;
                    targetEntity->motionX         = d1;
                    targetEntity->motionY         = d2;
                    targetEntity->motionZ         = d3;
                }

                if (flag2)
                {
                    world->playSound(nullptr, posX, posY, posZ, SoundEvents::ENTITY_PLAYER_ATTACK_CRIT,
                                     getSoundCategory(), 1.0F, 1.0F);
                    onCriticalHit(targetEntity);
                }

                if (!flag2 && !flag3)
                {
                    if (flag)
                    {
                        world->playSound(nullptr, posX, posY, posZ, SoundEvents::ENTITY_PLAYER_ATTACK_STRONG,
                                         getSoundCategory(), 1.0F, 1.0F);
                    }
                    else
                    {
                        world->playSound(nullptr, posX, posY, posZ, SoundEvents::ENTITY_PLAYER_ATTACK_WEAK,
                                         getSoundCategory(), 1.0F, 1.0F);
                    }
                }

                if (f1 > 0.0F)
                {
                    onEnchantmentCritical(targetEntity);
                }

                setLastAttackedEntity(targetEntity);
                if (Util:: instanceof <EntityLivingBase>(targetEntity))
                {
                    EnchantmentHelper::applyThornEnchantments((EntityLivingBase *)targetEntity, this);
                }

                EnchantmentHelper::applyArthropodEnchantments(this, targetEntity);
                ItemStack itemstack1 = getHeldItemMainhand();
                Entity *entity       = targetEntity;
                if (Util:: instanceof <MultiPartEntityPart>(targetEntity))
                {
                    IEntityMultiPart *ientitymultipart = ((MultiPartEntityPart *)targetEntity)->parent;
                    if (Util:: instanceof <EntityLivingBase>(ientitymultipart))
                    {
                        entity = (EntityLivingBase *)ientitymultipart;
                    }
                }

                if (!itemstack1.isEmpty() && Util:: instanceof <EntityLivingBase>(entity))
                {
                    itemstack1.hitEntity((EntityLivingBase *)entity, this);
                    if (itemstack1.isEmpty())
                    {
                        setHeldItem(EnumHand::MAIN_HAND, ItemStack::EMPTY);
                    }
                }

                if (Util:: instanceof <EntityLivingBase>(targetEntity))
                {
                    float f5 = f4 - ((EntityLivingBase *)targetEntity)->getHealth();
                    addStat(StatList::DAMAGE_DEALT, MathHelper::round(f5 * 10.0F));
                    if (j > 0)
                    {
                        targetEntity->setFire(j * 4);
                    }

                    if (Util:: instanceof <WorldServer>(world) && f5 > 2.0F)
                    {
                        int k = (int)((double)f5 * 0.5);
                        ((WorldServer *)world)
                            ->spawnParticle(EnumParticleTypes::DAMAGE_INDICATOR, targetEntity->posX,
                                            targetEntity->posY + (double)(targetEntity->height * 0.5F),
                                            targetEntity->posZ, k, 0.1, 0.0, 0.1, 0.2, {});
                    }
                }

                addExhaustion(0.1F);
            }
            else
            {
                world->playSound(nullptr, posX, posY, posZ, SoundEvents::ENTITY_PLAYER_ATTACK_NODAMAGE,
                                 getSoundCategory(), 1.0F, 1.0F);
                if (flag4)
                {
                    targetEntity->extinguish();
                }
            }
        }
    }
}

void EntityPlayer::disableShield(bool p_190777_1_)
{
    float f = 0.25F + (float)EnchantmentHelper::getEfficiencyModifier(this) * 0.05F;
    if (p_190777_1_)
    {
        f += 0.75F;
    }

    if (MathHelper::nextFloat(rand) < f)
    {
        getCooldownTracker().setCooldown(Items::SHIELD, 100);
        resetActiveHand();
        world->setEntityState(this, std::byte{30});
    }
}

void EntityPlayer::onCriticalHit(Entity *entityHit)
{
}

void EntityPlayer::onEnchantmentCritical(Entity *entityHit)
{
}

void EntityPlayer::spawnSweepParticles() const
{
    double d0 = (double)(-MathHelper::sin(rotationYaw * 0.017453292F));
    double d1 = (double)MathHelper::cos(rotationYaw * 0.017453292F);
    if (Util:: instanceof <WorldServer>(world))
    {
        ((WorldServer *)world)
            ->spawnParticle(EnumParticleTypes::SWEEP_ATTACK, posX + d0, posY + (double)height * 0.5, posZ + d1, 0, d0,
                            0.0, d1, 0.0, {});
    }
}

void EntityPlayer::respawnPlayer()
{
}

void EntityPlayer::setDead()
{
    EntityLivingBase::setDead();
    inventoryContainer->onContainerClosed(this);
    if (openContainer != nullptr)
    {
        openContainer->onContainerClosed(this);
    }
}

bool EntityPlayer::isEntityInsideOpaqueBlock() const
{
    return !sleeping && EntityLivingBase::isEntityInsideOpaqueBlock();
}

bool EntityPlayer::isUser()
{
    return false;
}

const GameProfile &EntityPlayer::getGameProfile() const
{
    return gameProfile;
}

EntityPlayer::SleepResult EntityPlayer::trySleep(const BlockPos &bedLocationin)
{
    EnumFacing enumfacing = world->getBlockState(bedLocationin)->getValue(BlockHorizontal::FACING);
    if (!world->isRemote)
    {
        if (isPlayerSleeping() || !isEntityAlive())
        {
            return EntityPlayer::SleepResult::OTHER_PROBLEM;
        }

        if (!world->provider->isSurfaceWorld())
        {
            return EntityPlayer::SleepResult::NOT_POSSIBLE_HERE;
        }

        if (world->isDaytime())
        {
            return EntityPlayer::SleepResult::NOT_POSSIBLE_NOW;
        }

        if (!bedInRange(bedLocationin, enumfacing))
        {
            return EntityPlayer::SleepResult::TOO_FAR_AWAY;
        }

        double d0 = 8.0;
        double d1 = 5.0;
        auto list = world->getEntitiesWithinAABB<EntityMob>(
            AxisAlignedBB((double)bedLocationin.getx() - 8.0, (double)bedLocationin.gety() - 5.0,
                          (double)bedLocationin.getz() - 8.0, (double)bedLocationin.getx() + 8.0,
                          (double)bedLocationin.gety() + 5.0, (double)bedLocationin.getz() + 8.0),
            EntityPlayer::SleepEnemyPredicate(this));
        if (!list.empty())
        {
            return EntityPlayer::SleepResult::NOT_SAFE;
        }
    }

    if (isRiding())
    {
        dismountRidingEntity();
    }

    spawnShoulderEntities();
    setSize(0.2F, 0.2F);
    if (world->isBlockLoaded(bedLocationin))
    {
        float f1 = 0.5F + (float)enumfacing.getXOffset() * 0.4F;
        float f  = 0.5F + (float)enumfacing.getZOffset() * 0.4F;
        setRenderOffsetForSleep(enumfacing);
        setPosition((double)((float)bedLocationin.getx() + f1), (double)((float)bedLocationin.gety() + 0.6875F),
                    (double)((float)bedLocationin.getz() + f));
    }
    else
    {
        setPosition((double)((float)bedLocationin.getx() + 0.5F), (double)((float)bedLocationin.gety() + 0.6875F),
                    (double)((float)bedLocationin.getz() + 0.5F));
    }

    sleeping    = true;
    sleepTimer  = 0;
    bedLocation = bedLocationin;
    motionX     = 0.0;
    motionY     = 0.0;
    motionZ     = 0.0;
    if (!world->isRemote)
    {
        world->updateAllPlayersSleepingFlag();
    }

    return EntityPlayer::SleepResult::OK;
}

void EntityPlayer::wakeUpPlayer(bool immediately, bool updateWorldFlag, bool setSpawn)
{
    setSize(0.6F, 1.8F);
    IBlockState *iblockstate = world->getBlockState(bedLocation);
    if (bedLocation.has_value() && iblockstate->getBlock() == Blocks::BED)
    {
        world->setBlockState(bedLocation.value(), iblockstate->withProperty(BlockBed::OCCUPIED, false), 4);
        BlockPos blockpos = BlockBed::getSafeExitLocation(world, bedLocation, 0);
        if (blockpos == nullptr)
        {
            blockpos = bedLocation->up();
        }

        setPosition((double)((float)blockpos.getx() + 0.5F), (double)((float)blockpos.gety() + 0.1F),
                    (double)((float)blockpos.getz() + 0.5F));
    }

    sleeping = false;
    if (!world->isRemote && updateWorldFlag)
    {
        world->updateAllPlayersSleepingFlag();
    }

    sleepTimer = immediately ? 0 : 100;
    if (setSpawn)
    {
        setSpawnPoint(bedLocation, false);
    }
}

std::optional<BlockPos> EntityPlayer::getBedSpawnLocation(World *worldIn, const BlockPos &bedLocation, bool forceSpawn)
{
    Block *block = worldIn->getBlockState(bedLocation)->getBlock();
    if (block != Blocks::BED)
    {
        if (!forceSpawn)
        {
            return std::nullopt;
        }
        else
        {
            bool flag  = block->canSpawnInBlock();
            bool flag1 = worldIn->getBlockState(bedLocation.up())->getBlock()->canSpawnInBlock();
            return flag && flag1 ? bedLocation : std::nullopt;
        }
    }
    else
    {
        return BlockBed::getSafeExitLocation(worldIn, bedLocation, 0);
    }
}

float EntityPlayer::getBedOrientationInDegrees()
{
    if (bedLocation.has_value())
    {
        EnumFacing enumfacing =
            (EnumFacing)world->getBlockState(bedLocation.value())->getValue(BlockHorizontal::FACING);
        switch (enumfacing)
        {
        case EnumFacing::SOUTH:
            return 90.0F;
        case EnumFacing::WEST:
            return 0.0F;
        case EnumFacing::NORTH:
            return 270.0F;
        case EnumFacing::EAST:
            return 180.0F;
        }
    }

    return 0.0F;
}

bool EntityPlayer::isPlayerSleeping()
{
    return sleeping;
}

bool EntityPlayer::isPlayerFullyAsleep() const
{
    return sleeping && sleepTimer >= 100;
}

int32_t EntityPlayer::getSleepTimer() const
{
    return sleepTimer;
}

void EntityPlayer::sendStatusMessage(ITextComponent *chatComponent, bool actionBar)
{
}

BlockPos EntityPlayer::getBedLocation()
{
    return spawnPos.value();
}

bool EntityPlayer::isSpawnForced() const
{
    return spawnForced;
}

void EntityPlayer::setSpawnPoint(std::optional<BlockPos> pos, bool forced)
{
    if (pos.has_value())
    {
        spawnPos    = pos;
        spawnForced = forced;
    }
    else
    {
        spawnPos    = std::nullopt;
        spawnForced = false;
    }
}

void EntityPlayer::addStat(StatBase *stat)
{
    addStat(stat, 1);
}

void EntityPlayer::addStat(StatBase *stat, int32_t amount)
{
}

void EntityPlayer::takeStat(StatBase *stat)
{
}

void EntityPlayer::unlockRecipes(std::vector<IRecipe *> p_192021_1_)
{
}

void EntityPlayer::unlockRecipes(std::vector<ResourceLocation> p_193102_1_)
{
}

void EntityPlayer::resetRecipes(std::vector<IRecipe *> p_192022_1_)
{
}

void EntityPlayer::jump()
{
    EntityLivingBase::jump();
    addStat(&StatList::JUMP);
    if (isSprinting())
    {
        addExhaustion(0.2F);
    }
    else
    {
        addExhaustion(0.05F);
    }
}

void EntityPlayer::travel(float strafe, float vertical, float forward)
{
    double d0 = posX;
    double d1 = posY;
    double d2 = posZ;
    if (capabilities.isFlying && !isRiding())
    {
        double d3          = motionY;
        float f            = jumpMovementFactor;
        jumpMovementFactor = capabilities.getFlySpeed() * (float)(isSprinting() ? 2 : 1);
        EntityLivingBase::travel(strafe, vertical, forward);
        motionY            = d3 * 0.6;
        jumpMovementFactor = f;
        fallDistance       = 0.0F;
        setFlag(7, false);
    }
    else
    {
        EntityLivingBase::travel(strafe, vertical, forward);
    }

    addMovementStat(posX - d0, posY - d1, posZ - d2);
}

float EntityPlayer::getAIMoveSpeed()
{
    return (float)getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED.get())->getAttributeValue();
}

void EntityPlayer::addMovementStat(double p_71000_1_, double p_71000_3_, double p_71000_5_)
{
    if (!isRiding())
    {
        int k;
        if (isInsideOfMaterial(Material::WATER))
        {
            k = MathHelper::round(
                MathHelper::sqrt(p_71000_1_ * p_71000_1_ + p_71000_3_ * p_71000_3_ + p_71000_5_ * p_71000_5_) * 100.0F);
            if (k > 0)
            {
                addStat(&StatList::DIVE_ONE_CM, k);
                addExhaustion(0.01F * (float)k * 0.01F);
            }
        }
        else if (isInWater())
        {
            k = MathHelper::round(MathHelper::sqrt(p_71000_1_ * p_71000_1_ + p_71000_5_ * p_71000_5_) * 100.0F);
            if (k > 0)
            {
                addStat(&StatList::SWIM_ONE_CM, k);
                addExhaustion(0.01F * (float)k * 0.01F);
            }
        }
        else if (isOnLadder())
        {
            if (p_71000_3_ > 0.0)
            {
                addStat(&StatList::CLIMB_ONE_CM, (int)MathHelper::round(p_71000_3_ * 100.0));
            }
        }
        else if (onGround)
        {
            k = MathHelper::round(MathHelper::sqrt(p_71000_1_ * p_71000_1_ + p_71000_5_ * p_71000_5_) * 100.0F);
            if (k > 0)
            {
                if (isSprinting())
                {
                    addStat(&StatList::SPRINT_ONE_CM, k);
                    addExhaustion(0.1F * (float)k * 0.01F);
                }
                else if (isSneaking())
                {
                    addStat(&StatList::CROUCH_ONE_CM, k);
                    addExhaustion(0.0F * (float)k * 0.01F);
                }
                else
                {
                    addStat(&StatList::WALK_ONE_CM, k);
                    addExhaustion(0.0F * (float)k * 0.01F);
                }
            }
        }
        else if (isElytraFlying())
        {
            k = MathHelper::round(
                MathHelper::sqrt(p_71000_1_ * p_71000_1_ + p_71000_3_ * p_71000_3_ + p_71000_5_ * p_71000_5_) * 100.0F);
            addStat(&StatList::AVIATE_ONE_CM, k);
        }
        else
        {
            k = MathHelper::round(MathHelper::sqrt(p_71000_1_ * p_71000_1_ + p_71000_5_ * p_71000_5_) * 100.0F);
            if (k > 25)
            {
                addStat(&StatList::FLY_ONE_CM, k);
            }
        }
    }
}

void EntityPlayer::fall(float distance, float damageMultiplier)
{
    if (!capabilities.allowFlying)
    {
        if (distance >= 2.0F)
        {
            addStat(&StatList::FALL_ONE_CM, MathHelper::round((double)distance * 100.0));
        }

        EntityLivingBase::fall(distance, damageMultiplier);
    }
}

void EntityPlayer::onKillEntity(EntityLivingBase *entityLivingIn)
{
    EntityList::EntityEggInfo entitylist$entityegginfo =
        (EntityList::EntityEggInfo)EntityList::ENTITY_EGGS.at(EntityList::getKey((Entity *)entityLivingIn));
    if (entitylist$entityegginfo != null)
    {
        addStat(&entitylist$entityegginfo.killEntityStat);
    }
}

void EntityPlayer::setInWeb()
{
    if (!capabilities.isFlying)
    {
        EntityLivingBase::setInWeb();
    }
}

void EntityPlayer::addExperience(int32_t amount)
{
    addScore(amount);
    auto i = std::numeric_limits<int32_t>::max() - experienceTotal;
    if (amount > i)
    {
        amount = i;
    }

    experience += (float)amount / (float)xpBarCap();

    for (experienceTotal += amount; experience >= 1.0F; experience /= (float)xpBarCap())
    {
        experience = (experience - 1.0F) * (float)xpBarCap();
        addExperienceLevel(1);
    }
}

int32_t EntityPlayer::getXPSeed() const
{
    return xpSeed;
}

void EntityPlayer::onEnchant(ItemStack enchantedItem, int32_t cost)
{
    experienceLevel -= cost;
    if (experienceLevel < 0)
    {
        experienceLevel = 0;
        experience      = 0.0F;
        experienceTotal = 0;
    }

    xpSeed = rand();
}

void EntityPlayer::addExperienceLevel(int32_t levels)
{
    experienceLevel += levels;
    if (experienceLevel < 0)
    {
        experienceLevel = 0;
        experience      = 0.0F;
        experienceTotal = 0;
    }

    if (levels > 0 && experienceLevel % 5 == 0 && (float)lastXPSound < (float)ticksExisted - 100.0F)
    {
        float f = experienceLevel > 30 ? 1.0F : (float)experienceLevel / 30.0F;
        world->playSound(nullptr, posX, posY, posZ, SoundEvents::ENTITY_PLAYER_LEVELUP, getSoundCategory(), f * 0.75F,
                         1.0F);
        lastXPSound = ticksExisted;
    }
}

int32_t EntityPlayer::xpBarCap() const
{
    if (experienceLevel >= 30)
    {
        return 112 + (experienceLevel - 30) * 9;
    }
    else
    {
        return experienceLevel >= 15 ? 37 + (experienceLevel - 15) * 5 : 7 + experienceLevel * 2;
    }
}

void EntityPlayer::addExhaustion(float exhaustion)
{
    if (!capabilities.disableDamage && !world->isRemote)
    {
        foodStats.addExhaustion(exhaustion);
    }
}

FoodStats &EntityPlayer::getFoodStats()
{
    return foodStats;
}

bool EntityPlayer::canEat(bool ignoreHunger) const
{
    return (ignoreHunger || foodStats.needFood()) && !capabilities.disableDamage;
}

bool EntityPlayer::shouldHeal()
{
    return getHealth() > 0.0F && getHealth() < getMaxHealth();
}

bool EntityPlayer::isAllowEdit()
{
    return capabilities.allowEdit;
}

bool EntityPlayer::canPlayerEdit(const BlockPos &pos, EnumFacing facing, ItemStack stack)
{
    if (capabilities.allowEdit)
    {
        return true;
    }
    else if (stack.isEmpty())
    {
        return false;
    }
    else
    {
        BlockPos blockpos = pos.offset(facing.getOpposite());
        Block *block      = world->getBlockState(blockpos)->getBlock();
        return stack.canPlaceOn(block) || stack.canEditBlocks();
    }
}

bool EntityPlayer::getAlwaysRenderNameTagForRender()
{
    return true;
}

void EntityPlayer::sendPlayerAbilities()
{
}

void EntityPlayer::setGameType(GameType gameType)
{
}

std::string EntityPlayer::getName() const
{
    return gameProfile.getName();
}

InventoryEnderChest &EntityPlayer::getInventoryEnderChest()
{
    return enderChest;
}

ItemStack EntityPlayer::getItemStackFromSlot(EntityEquipmentSlot slotIn)
{
    if (slotIn == EntityEquipmentSlot::MAINHAND)
    {
        return inventory.getCurrentItem();
    }
    else if (slotIn == EntityEquipmentSlot::OFFHAND)
    {
        return inventory.offHandInventory[0];
    }
    else
    {
        return slotIn.getSlotType() == EntityEquipmentSlot::Type::ARMOR
                   ? (ItemStack)inventory.armorInventory[slotIn.getIndex()]
                   : ItemStack::EMPTY;
    }
}

void EntityPlayer::setItemStackToSlot(EntityEquipmentSlot slotIn, ItemStack stack)
{
    if (slotIn == EntityEquipmentSlot::MAINHAND)
    {
        playEquipSound(stack);
        inventory.mainInventory[inventory.currentItem] = stack;
    }
    else if (slotIn == EntityEquipmentSlot::OFFHAND)
    {
        playEquipSound(stack);
        inventory.offHandInventory[0] = stack;
    }
    else if (slotIn.getSlotType() == EntityEquipmentSlot::Type::ARMOR)
    {
        playEquipSound(stack);
        inventory.armorInventory[slotIn.getIndex()] = stack;
    }
}

bool EntityPlayer::addItemStackToInventory(ItemStack p_191521_1_)
{
    playEquipSound(p_191521_1_);
    return inventory.addItemStackToInventory(p_191521_1_);
}

std::vector<ItemStack> EntityPlayer::getHeldEquipment()
{
    return {getHeldItemMainhand(), getHeldItemOffhand()};
}

std::vector<ItemStack> &EntityPlayer::getArmorInventoryList()
{
    return inventory.armorInventory;
}

bool EntityPlayer::addShoulderEntity(NBTTagCompound *p_192027_1_)
{
    if (!isRiding() && onGround && !isInWater())
    {
        if (getLeftShoulderEntity().isEmpty())
        {
            setLeftShoulderEntity(p_192027_1_);
            return true;
        }
        else if (getRightShoulderEntity().isEmpty())
        {
            setRightShoulderEntity(p_192027_1_);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool EntityPlayer::isInvisibleToPlayer(EntityPlayer *player)
{
    if (!isInvisible())
    {
        return false;
    }
    else if (player->isSpectator())
    {
        return false;
    }
    else
    {
        Team *team = getTeam();
        return team == nullptr || player == nullptr || player->getTeam() != team ||
               !team->getSeeFriendlyInvisiblesEnabled();
    }
}

bool EntityPlayer::isPushedByWater()
{
    return !capabilities.isFlying;
}

Scoreboard EntityPlayer::getWorldScoreboard() const
{
    return world->getScoreboard();
}

Team *EntityPlayer::getTeam()
{
    return getWorldScoreboard().getPlayersTeam(getName());
}

ITextComponent *EntityPlayer::getDisplayName()
{
    ITextComponent *itextcomponent = new TextComponentString(ScorePlayerTeam::formatPlayerName(getTeam(), getName()));
    itextcomponent->getStyle().setClickEvent(
        new ClickEvent(ClickEvent::Action::SUGGEST_COMMAND, "/msg " + getName() + " "));
    itextcomponent->getStyle().setHoverEvent(getHoverEvent());
    itextcomponent->getStyle().setInsertion(getName());
    return itextcomponent;
}

float EntityPlayer::getEyeHeight()
{
    float f = 1.62F;
    if (isPlayerSleeping())
    {
        f = 0.2F;
    }
    else if (!isSneaking() && height != 1.65F)
    {
        if (isElytraFlying() || height == 0.6F)
        {
            f = 0.4F;
        }
    }
    else
    {
        f -= 0.08F;
    }

    return f;
}

void EntityPlayer::setAbsorptionAmount(float amount)
{
    if (amount < 0.0F)
    {
        amount = 0.0F;
    }

    getDataManager().set(ABSORPTION, amount);
}

float EntityPlayer::getAbsorptionAmount()
{
    return getDataManager().get(ABSORPTION);
}

xg::Guid EntityPlayer::getUUID(const GameProfile &profile)
{
    xg::Guid uuid = profile.getId();
    if (!uuid.isValid())
    {
        uuid = getOfflineUUID(profile.getName());
    }

    return uuid;
}

xg::Guid EntityPlayer::getOfflineUUID(std::string_view username)
{
    std::string name("OfflinePlayer:");
    name.insert(name.end(), username.begin(), username.end());
    auto hash = ConstexprHashes::md5(name);
    return xg::Guid(hash);
}

bool EntityPlayer::canOpen(LockCode code)
{
    if (code.isEmpty())
    {
        return true;
    }
    else
    {
        ItemStack itemstack = getHeldItemMainhand();
        return !itemstack.isEmpty() && itemstack.hasDisplayName() ? itemstack.getDisplayName() == (code.getLock())
                                                                  : false;
    }
}

bool EntityPlayer::isWearing(const EnumPlayerModelParts &part)
{
    return (getDataManager().get(PLAYER_MODEL_FLAG) & part.getPartMask()) == part.getPartMask();
}

bool EntityPlayer::sendCommandFeedback()
{
    return getServer()->worlds[0].getGameRules().getBoolean("sendCommandFeedback");
}

bool EntityPlayer::replaceItemInInventory(int32_t inventorySlot, ItemStack itemStackIn)
{
    if (inventorySlot >= 0 && inventorySlot < inventory.mainInventory.size())
    {
        inventory.setInventorySlotContents(inventorySlot, itemStackIn);
        return true;
    }
    else
    {
        std::optional<EntityEquipmentSlot> entityequipmentslot = std::nullopt;
        if (inventorySlot == 100 + EntityEquipmentSlot::HEAD.getIndex())
        {
            entityequipmentslot = EntityEquipmentSlot::HEAD;
        }
        else if (inventorySlot == 100 + EntityEquipmentSlot::CHEST.getIndex())
        {
            entityequipmentslot = EntityEquipmentSlot::CHEST;
        }
        else if (inventorySlot == 100 + EntityEquipmentSlot::LEGS.getIndex())
        {
            entityequipmentslot = EntityEquipmentSlot::LEGS;
        }
        else if (inventorySlot == 100 + EntityEquipmentSlot::FEET.getIndex())
        {
            entityequipmentslot = EntityEquipmentSlot::FEET;
        }
        else
        {
            entityequipmentslot = std::nullopt;
        }

        if (inventorySlot == 98)
        {
            setItemStackToSlot(EntityEquipmentSlot::MAINHAND, itemStackIn);
            return true;
        }
        else if (inventorySlot == 99)
        {
            setItemStackToSlot(EntityEquipmentSlot::OFFHAND, itemStackIn);
            return true;
        }
        else if (entityequipmentslot == std::nullopt)
        {
            auto i = inventorySlot - 200;
            if (i >= 0 && i < enderChest.getSizeInventory())
            {
                enderChest.setInventorySlotContents(i, itemStackIn);
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (!itemStackIn.isEmpty())
            {
                if (!(Util:: instanceof <ItemArmor>(itemStackIn.getItem())) &&
                    !(Util:: instanceof <ItemElytra>(itemStackIn.getItem())))
                {
                    if (entityequipmentslot.value() != EntityEquipmentSlot::HEAD)
                    {
                        return false;
                    }
                }
                else if (EntityLiving::getSlotForItemStack(itemStackIn) != entityequipmentslot.value())
                {
                    return false;
                }
            }

            inventory.setInventorySlotContents(entityequipmentslot->getIndex() + inventory.mainInventory.size(),
                                               itemStackIn);
            return true;
        }
    }
}

bool EntityPlayer::hasReducedDebug() const
{
    return bhasReducedDebug;
}

EnumHandSide EntityPlayer::getPrimaryHand()
{
    return dataManager.get(MAIN_HAND) == 0 ? EnumHandSide::LEFT : EnumHandSide::RIGHT;
}

void EntityPlayer::setPrimaryHand(EnumHandSide hand)
{
    dataManager.set(MAIN_HAND, (hand == EnumHandSide::LEFT ? 0 : 1));
}

NBTTagCompound *EntityPlayer::getLeftShoulderEntity()
{
    return dataManager.get(LEFT_SHOULDER_ENTITY);
}

NBTTagCompound *EntityPlayer::getRightShoulderEntity()
{
    return dataManager.get(RIGHT_SHOULDER_ENTITY);
}

float EntityPlayer::getCooldownPeriod()
{
    return (float)(1.0 / getEntityAttribute(SharedMonsterAttributes::ATTACK_SPEED.get())->getAttributeValue() * 20.0);
}

float EntityPlayer::getCooledAttackStrength(float adjustTicks)
{
    return MathHelper::clamp(((float)ticksSinceLastSwing + adjustTicks) / getCooldownPeriod(), 0.0F, 1.0F);
}

void EntityPlayer::resetCooldown()
{
    ticksSinceLastSwing = 0;
}

CooldownTracker EntityPlayer::getCooldownTracker() const
{
    return cooldownTracker;
}

void EntityPlayer::applyEntityCollision(Entity *entityIn)
{
    if (!isPlayerSleeping())
    {
        EntityLivingBase::applyEntityCollision(entityIn);
    }
}

float EntityPlayer::getLuck()
{
    return getEntityAttribute(SharedMonsterAttributes::LUCK.get())->getAttributeValue();
}

bool EntityPlayer::canUseCommandBlock()
{
    return capabilities.isCreativeMode && canUseCommand(2, "");
}

void EntityPlayer::setLeftShoulderEntity(NBTTagCompound *tag)
{
    dataManager.set(LEFT_SHOULDER_ENTITY, tag);
}

void EntityPlayer::setRightShoulderEntity(NBTTagCompound *tag)
{
    dataManager.set(RIGHT_SHOULDER_ENTITY, tag);
}

int32_t EntityPlayer::getExperiencePoints(EntityPlayer *player)
{
    if (!world->getGameRules().getBoolean("keepInventory") && !isSpectator())
    {
        auto i = experienceLevel * 7;
        return i > 100 ? 100 : i;
    }
    else
    {
        return 0;
    }
}

bool EntityPlayer::isPlayer()
{
    return true;
}

bool EntityPlayer::canTriggerWalking()
{
    return !capabilities.isFlying;
}

void EntityPlayer::spawnShoulderEntity(NBTTagCompound *p_192026_1_)
{
    if (!world->isRemote && !p_192026_1_->isEmpty())
    {
        Entity *entity = EntityList::createEntityFromNBT(p_192026_1_, world);
        if (Util:: instanceof <EntityTameable>(entity))
        {
            ((EntityTameable *)entity)->setOwnerId(entityUniqueID);
        }

        entity->setPosition(posX, posY + 0.699999988079071, posZ);
        world->spawnEntity(entity);
    }
}

void EntityPlayer::spawnShoulderEntities()
{
    spawnShoulderEntity(getLeftShoulderEntity());
    setLeftShoulderEntity(new NBTTagCompound());
    spawnShoulderEntity(getRightShoulderEntity());
    setRightShoulderEntity(new NBTTagCompound());
}

void EntityPlayer::setReducedDebug(bool reducedDebug)
{
    bhasReducedDebug = reducedDebug;
}

void EntityPlayer::applyEntityAttributes()
{
    EntityLivingBase::applyEntityAttributes();
    getAttributeMap()->registerAttribute(SharedMonsterAttributes::ATTACK_DAMAGE.get())->setBaseValue(1.0);
    getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED.get())->setBaseValue(0.10000000149011612);
    getAttributeMap()->registerAttribute(SharedMonsterAttributes::ATTACK_SPEED.get());
    getAttributeMap()->registerAttribute(SharedMonsterAttributes::LUCK.get());
}

void EntityPlayer::entityInit()
{
    EntityLivingBase::entityInit();
    dataManager.registe(ABSORPTION, 0.0F);
    dataManager.registe(PLAYER_SCORE, 0);
    dataManager.registe(PLAYER_MODEL_FLAG, 0);
    dataManager.registe(MAIN_HAND, 1);
    dataManager.registe(LEFT_SHOULDER_ENTITY, new NBTTagCompound());
    dataManager.registe(RIGHT_SHOULDER_ENTITY, new NBTTagCompound());
}

void EntityPlayer::updateSize()
{
    float f;
    float f1;
    if (isElytraFlying())
    {
        f  = 0.6F;
        f1 = 0.6F;
    }
    else if (isPlayerSleeping())
    {
        f  = 0.2F;
        f1 = 0.2F;
    }
    else if (isSneaking())
    {
        f  = 0.6F;
        f1 = 1.65F;
    }
    else
    {
        f  = 0.6F;
        f1 = 1.8F;
    }

    if (f != width || f1 != height)
    {
        AxisAlignedBB axisalignedbb = getEntityBoundingBox();
        axisalignedbb = AxisAlignedBB(axisalignedbb.getminX(), axisalignedbb.getminY(), axisalignedbb.getminZ(),
                                      axisalignedbb.getminX() + (double)f, axisalignedbb.getminY() + (double)f1,
                                      axisalignedbb.getminZ() + (double)f);
        if (!world->collidesWithAnyBlock(axisalignedbb))
        {
            setSize(f, f1);
        }
    }
}

SoundEvent EntityPlayer::getSwimSound()
{
    return SoundEvents::ENTITY_PLAYER_SWIM;
}

SoundEvent EntityPlayer::getSplashSound()
{
    return SoundEvents::ENTITY_PLAYER_SPLASH;
}

int32_t EntityPlayer::getFireImmuneTicks()
{
    return 20;
}

bool EntityPlayer::isMovementBlocked()
{
    return getHealth() <= 0.0F || isPlayerSleeping();
}

void EntityPlayer::closeScreen()
{
    openContainer = inventoryContainer.get();
}

void EntityPlayer::updateEntityActionState()
{
    EntityLivingBase::updateEntityActionState();
    updateArmSwingProgress();
    rotationYawHead = rotationYaw;
}

void EntityPlayer::destroyVanishingCursedItems()
{
    for (int i = 0; i < inventory.getSizeInventory(); ++i)
    {
        ItemStack itemstack = inventory.getStackInSlot(i);
        if (!itemstack.isEmpty() && EnchantmentHelper::hasVanishingCurse(itemstack))
        {
            inventory.removeStackFromSlot(i);
        }
    }
}

SoundEvent EntityPlayer::getHurtSound(DamageSource::DamageSource damageSourceIn)
{
    if (damageSourceIn == DamageSource::ON_FIRE)
    {
        return SoundEvents::ENTITY_PLAYER_HURT_ON_FIRE;
    }
    else
    {
        return damageSourceIn == DamageSource::DROWN ? SoundEvents::ENTITY_PLAYER_HURT_DROWN
                                                     : SoundEvents::ENTITY_PLAYER_HURT;
    }
}

SoundEvent EntityPlayer::getDeathSound()
{
    return SoundEvents::ENTITY_PLAYER_DEATH;
}

ItemStack EntityPlayer::dropItemAndGetStack(EntityItem *p_184816_1_) const
{
    world->spawnEntity(p_184816_1_);
    return p_184816_1_->getItem();
}

void EntityPlayer::blockUsingShield(EntityLivingBase *p_190629_1_)
{
    EntityLivingBase::blockUsingShield(p_190629_1_);
    if (Util:: instanceof <ItemAxe>(p_190629_1_->getHeldItemMainhand().getItem()))
    {
        disableShield(true);
    }
}

bool EntityPlayer::canAttackPlayer(EntityPlayer *other)
{
    Team *team  = getTeam();
    Team *team1 = other->getTeam();
    if (team == nullptr)
    {
        return true;
    }
    else
    {
        return !team->isSameTeam(team1) ? true : team->getAllowFriendlyFire();
    }
}

void EntityPlayer::damageArmor(float damage)
{
    inventory.damageArmor(damage);
}

void EntityPlayer::damageShield(float damage)
{
    if (damage >= 3.0F && activeItemStack.getItem() == Items::SHIELD)
    {
        auto i = 1 + MathHelper::floor(damage);
        activeItemStack.damageItem(i, this);
        if (activeItemStack.isEmpty())
        {
            EnumHand enumhand = getActiveHand();
            if (enumhand == EnumHand::MAIN_HAND)
            {
                setItemStackToSlot(EntityEquipmentSlot::MAINHAND, ItemStack::EMPTY);
            }
            else
            {
                setItemStackToSlot(EntityEquipmentSlot::OFFHAND, ItemStack::EMPTY);
            }

            activeItemStack = ItemStack::EMPTY;
            playSound(SoundEvents::ITEM_SHIELD_BREAK, 0.8F, 0.8F + MathHelper::nextFloat(world->rand) * 0.4F);
        }
    }
}

void EntityPlayer::damageEntity(DamageSource::DamageSource damageSrc, float damageAmount)
{
    if (!isEntityInvulnerable(damageSrc))
    {
        damageAmount = applyArmorCalculations(damageSrc, damageAmount);
        damageAmount = applyPotionDamageCalculations(damageSrc, damageAmount);
        float f      = damageAmount;
        damageAmount = MathHelper::max(damageAmount - getAbsorptionAmount(), 0.0F);
        setAbsorptionAmount(getAbsorptionAmount() - (f - damageAmount));
        if (damageAmount != 0.0F)
        {
            addExhaustion(damageSrc.getHungerDamage());
            float f1 = getHealth();
            setHealth(getHealth() - damageAmount);
            getCombatTracker().trackDamage(damageSrc, f1, damageAmount);
            if (damageAmount < 3.4028235E37F)
            {
                addStat(StatList::DAMAGE_TAKEN, MathHelper::round(damageAmount * 10.0F));
            }
        }
    }
}

void EntityPlayer::doWaterSplashEffect()
{
    if (!isSpectator())
    {
        EntityLivingBase::doWaterSplashEffect();
    }
}

SoundEvent EntityPlayer::getFallSound(int32_t heightIn)
{
    return heightIn > 4 ? SoundEvents::ENTITY_PLAYER_BIG_FALL : SoundEvents::ENTITY_PLAYER_SMALL_FALL;
}

CooldownTracker EntityPlayer::createCooldownTracker()
{
    return CooldownTracker();
}

void EntityPlayer::updateCape()
{
    prevChasingPosX = chasingPosX;
    prevChasingPosY = chasingPosY;
    prevChasingPosZ = chasingPosZ;
    double d0       = posX - chasingPosX;
    double d1       = posY - chasingPosY;
    double d2       = posZ - chasingPosZ;
    double d3       = 10.0;
    if (d0 > 10.0)
    {
        chasingPosX     = posX;
        prevChasingPosX = chasingPosX;
    }

    if (d2 > 10.0)
    {
        chasingPosZ     = posZ;
        prevChasingPosZ = chasingPosZ;
    }

    if (d1 > 10.0)
    {
        chasingPosY     = posY;
        prevChasingPosY = chasingPosY;
    }

    if (d0 < -10.0)
    {
        chasingPosX     = posX;
        prevChasingPosX = chasingPosX;
    }

    if (d2 < -10.0)
    {
        chasingPosZ     = posZ;
        prevChasingPosZ = chasingPosZ;
    }

    if (d1 < -10.0)
    {
        chasingPosY     = posY;
        prevChasingPosY = chasingPosY;
    }

    chasingPosX += d0 * 0.25;
    chasingPosZ += d2 * 0.25;
    chasingPosY += d1 * 0.25;
}

void EntityPlayer::playShoulderEntityAmbientSound(NBTTagCompound *p_192028_1_)
{
    if (p_192028_1_ != nullptr && !p_192028_1_->hasKey("Silent") || !p_192028_1_->getBoolean("Silent"))
    {
        auto s = p_192028_1_->getString("id");
        if (s == (EntityList::getKey(EntityParrot.class).toString()))
        {
            EntityParrot::playAmbientSound(world, this);
        }
    }
}

void EntityPlayer::collideWithPlayer(Entity *entityIn)
{
    entityIn->onCollideWithPlayer(this);
}

bool EntityPlayer::bedInRange(const BlockPos &p_190774_1_, EnumFacing p_190774_2_) const
{
    if (MathHelper::abs(posX - (double)p_190774_1_.getx()) <= 3.0 &&
        MathHelper::abs(posY - (double)p_190774_1_.gety()) <= 2.0 &&
        MathHelper::abs(posZ - (double)p_190774_1_.getz()) <= 3.0)
    {
        return true;
    }
    else
    {
        BlockPos blockpos = p_190774_1_.offset(p_190774_2_.getOpposite());
        return MathHelper::abs(posX - (double)blockpos.getx()) <= 3.0 &&
               MathHelper::abs(posY - (double)blockpos.gety()) <= 2.0 &&
               MathHelper::abs(posZ - (double)blockpos.getz()) <= 3.0;
    }
}

void EntityPlayer::setRenderOffsetForSleep(EnumFacing bedDirection)
{
    renderOffsetX = -1.8F * (float)bedDirection.getXOffset();
    renderOffsetZ = -1.8F * (float)bedDirection.getZOffset();
}

bool EntityPlayer::isInBed()
{
    return world->getBlockState(bedLocation.value())->getBlock() == Blocks::BED;
}

void EntityPlayer::addMountedMovementStat(double p_71015_1_, double p_71015_3_, double p_71015_5_)
{
    if (isRiding())
    {
        int i = MathHelper::round(
            MathHelper::sqrt(p_71015_1_ * p_71015_1_ + p_71015_3_ * p_71015_3_ + p_71015_5_ * p_71015_5_) * 100.0F);
        if (i > 0)
        {
            if (Util:: instanceof <EntityMinecart>(getRidingEntity()))
            {
                addStat(&StatList::MINECART_ONE_CM, i);
            }
            else if (Util:: instanceof <EntityBoat>(getRidingEntity()))
            {
                addStat(&StatList::BOAT_ONE_CM, i);
            }
            else if (Util:: instanceof <EntityPig>(getRidingEntity()))
            {
                addStat(&StatList::PIG_ONE_CM, i);
            }
            else if (Util:: instanceof <AbstractHorse>(getRidingEntity()))
            {
                addStat(&StatList::HORSE_ONE_CM, i);
            }
        }
    }
}
