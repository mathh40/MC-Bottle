#include "EntityArmorStand.h"

#include "../EntityLiving.h"
#include "WorldServer.h"
#include "datafix/DataFixer.h"

const Rotations DEFAULT_HEAD_ROTATION     = Rotations(0.0F, 0.0F, 0.0F);
const Rotations DEFAULT_BODY_ROTATION     = Rotations(0.0F, 0.0F, 0.0F);
const Rotations DEFAULT_LEFTARM_ROTATION  = Rotations(-10.0F, 0.0F, -10.0F);
const Rotations DEFAULT_RIGHTARM_ROTATION = Rotations(-15.0F, 0.0F, 10.0F);
const Rotations DEFAULT_LEFTLEG_ROTATION  = Rotations(-1.0F, 0.0F, -1.0F);
const Rotations DEFAULT_RIGHTLEG_ROTATION = Rotations(1.0F, 0.0F, 1.0F);
const auto IS_RIDEABLE_MINECART           = [](Entity *p_apply_1_) -> bool {
    return Util:: instanceof
        <EntityMinecart>(p_apply_1_) && ((EntityMinecart *)p_apply_1_)->getType() == EntityMinecart::Type::RIDEABLE;
};

DataParameter EntityArmorStand::STATUS = EntityDataManager::createKey(EntityArmorStand.class, DataSerializers::BYTE);
DataParameter EntityArmorStand::HEAD_ROTATION =
    EntityDataManager::createKey(EntityArmorStand.class, DataSerializers::ROTATIONS);
DataParameter EntityArmorStand::BODY_ROTATION =
    EntityDataManager::createKey(EntityArmorStand.class, DataSerializers::ROTATIONS);
DataParameter EntityArmorStand::LEFT_ARM_ROTATION =
    EntityDataManager::createKey(EntityArmorStand.class, DataSerializers::ROTATIONS);
DataParameter EntityArmorStand::RIGHT_ARM_ROTATION =
    EntityDataManager::createKey(EntityArmorStand.class, DataSerializers::ROTATIONS);
DataParameter EntityArmorStand::LEFT_LEG_ROTATION =
    EntityDataManager::createKey(EntityArmorStand.class, DataSerializers::ROTATIONS);
DataParameter EntityArmorStand::RIGHT_LEG_ROTATION =
    EntityDataManager::createKey(EntityArmorStand.class, DataSerializers::ROTATIONS);

EntityArmorStand::EntityArmorStand(World *worldIn)
    : EntityLivingBase(worldIn), handItems(2, ItemStack::EMPTY), armorItems(4, ItemStack::EMPTY),
      headRotation(DEFAULT_HEAD_ROTATION), bodyRotation(DEFAULT_BODY_ROTATION),
      leftArmRotation(DEFAULT_LEFTARM_ROTATION), rightArmRotation(DEFAULT_RIGHTARM_ROTATION),
      leftLegRotation(DEFAULT_LEFTLEG_ROTATION), rightLegRotation(DEFAULT_RIGHTLEG_ROTATION)
{
    noClip = hasNoGravity();
    EntityArmorStand::setSize(0.5F, 1.975F);
}

EntityArmorStand::EntityArmorStand(World *worldIn, double posX, double posY, double posZ) : EntityArmorStand(worldIn)
{
    EntityArmorStand::setPosition(posX, posY, posZ);
}

bool EntityArmorStand::isServerWorld()
{
    return EntityLivingBase::isServerWorld() && !hasNoGravity();
}

std::vector<ItemStack> &EntityArmorStand::getHeldEquipment()
{
    return handItems;
}

std::vector<ItemStack> &EntityArmorStand::getArmorInventoryList()
{
    return armorItems;
}

ItemStack EntityArmorStand::getItemStackFromSlot(EntityEquipmentSlot slotIn)
{
    switch (slotIn.getSlotType())
    {
    case EquipmentType::HAND:
        return handItems[slotIn.getIndex()];
    case EquipmentType::ARMOR:
        return armorItems[slotIn.getIndex()];
    default:
        return ItemStack::EMPTY;
    }
}

void EntityArmorStand::setItemStackToSlot(EntityEquipmentSlot slotIn, ItemStack stack)
{
    switch (slotIn.getSlotType())
    {
    case EquipmentType::HAND:
        playEquipSound(stack);
        handItems[slotIn.getIndex()] = stack;
        break;
    case EquipmentType::ARMOR:
        playEquipSound(stack);
        armorItems[slotIn.getIndex()] = stack;
    }
}

bool EntityArmorStand::replaceItemInInventory(int32_t inventorySlot, ItemStack itemStackIn)
{
    EntityEquipmentSlot entityequipmentslot = EntityEquipmentSlot::FEET;
    if (inventorySlot == 98)
    {
        entityequipmentslot = EntityEquipmentSlot::MAINHAND;
    }
    else if (inventorySlot == 99)
    {
        entityequipmentslot = EntityEquipmentSlot::OFFHAND;
    }
    else if (inventorySlot == 100 + EntityEquipmentSlot::HEAD.getIndex())
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
    else
    {
        if (inventorySlot != 100 + EntityEquipmentSlot::FEET.getIndex())
        {
            return false;
        }

        entityequipmentslot = EntityEquipmentSlot::FEET;
    }

    if (!itemStackIn.isEmpty() && !EntityLiving::isItemStackInSlot(entityequipmentslot, itemStackIn) &&
        entityequipmentslot != EntityEquipmentSlot::HEAD)
    {
        return false;
    }
    else
    {
        setItemStackToSlot(entityequipmentslot, itemStackIn);
        return true;
    }
}

void EntityArmorStand::writeEntityToNBT(NBTTagCompound *compound)
{
    EntityLivingBase::writeEntityToNBT(compound);
    auto nbttaglist = std::make_unique<NBTTagList>();

    for (auto itemstack : armorItems)
    {
        auto nbttagcompound = std::make_unique<NBTTagCompound>();
        if (!itemstack.isEmpty())
        {
            itemstack.writeToNBT(nbttagcompound.get());
        }
        nbttaglist->appendTag(nbttagcompound);
    }

    compound->setTag("ArmorItems", nbttaglist);

    auto nbttaglist1 = std::make_unique<NBTTagList>();

    for (auto itemstack1 : handItems)
    {
        auto nbttagcompound1 = std::make_unique<NBTTagCompound>();
        if (!itemstack1.isEmpty())
        {
            itemstack1.writeToNBT(nbttagcompound1.get());
        }
        nbttaglist1->appendTag(nbttagcompound1);
    }

    compound->setTag("HandItems", nbttaglist1);
    compound->setBoolean("Invisible", isInvisible());
    compound->setBoolean("Small", isSmall());
    compound->setBoolean("ShowArms", getShowArms());
    compound->setInteger("DisabledSlots", disabledSlots);
    compound->setBoolean("NoBasePlate", hasNoBasePlate());
    if (hasMarker())
    {
        compound->setBoolean("Marker", hasMarker());
    }

    compound->setTag("Pose", readPoseFromNBT());
}

void EntityArmorStand::readEntityFromNBT(NBTTagCompound *compound)
{
    EntityLivingBase::readEntityFromNBT(compound);
    NBTTagList *nbttaglist1;
    int j;
    if (compound->hasKey("ArmorItems", 9))
    {
        nbttaglist1 = compound->getTagList("ArmorItems", 10);

        for (j = 0; j < armorItems.size(); ++j)
        {
            armorItems[j] = ItemStack(nbttaglist1->getCompoundTagAt(j).get());
        }
    }

    if (compound->hasKey("HandItems", 9))
    {
        nbttaglist1 = compound->getTagList("HandItems", 10);

        for (j = 0; j < handItems.size(); ++j)
        {
            handItems[j] = ItemStack(nbttaglist1->getCompoundTagAt(j).get());
        }
    }

    setInvisible(compound->getBoolean("Invisible"));
    setSmall(compound->getBoolean("Small"));
    setShowArms(compound->getBoolean("ShowArms"));
    disabledSlots = compound->getInteger("DisabledSlots");
    setNoBasePlate(compound->getBoolean("NoBasePlate"));
    setMarker(compound->getBoolean("Marker"));
    wasMarker           = !hasMarker();
    noClip              = hasNoGravity();
    auto nbttagcompound = compound->getCompoundTag("Pose");
    writePoseToNBT(nbttagcompound);
}

bool EntityArmorStand::canBePushed()
{
    return false;
}

EnumActionResult EntityArmorStand::applyPlayerInteraction(EntityPlayer *player, Vec3d vec, EnumHand hand)
{
    ItemStack itemstack = player->getHeldItem(hand);
    if (!hasMarker() && itemstack.getItem() != Items::NAME_TAG)
    {
        if (!world->isRemote && !player->isSpectator())
        {
            EntityEquipmentSlot entityequipmentslot = EntityLiving::getSlotForItemStack(itemstack);
            if (itemstack.isEmpty())
            {
                EntityEquipmentSlot entityequipmentslot1 = getClickedSlot(vec);
                EntityEquipmentSlot entityequipmentslot2 =
                    isDisabled(entityequipmentslot1) ? entityequipmentslot : entityequipmentslot1;
                if (hasItemInSlot(entityequipmentslot2))
                {
                    swapItem(player, entityequipmentslot2, itemstack, hand);
                }
            }
            else
            {
                if (isDisabled(entityequipmentslot))
                {
                    return EnumActionResult::FAIL;
                }

                if (entityequipmentslot.getSlotType() == EntityEquipmentSlot::Type::HAND && !getShowArms())
                {
                    return EnumActionResult::FAIL;
                }

                swapItem(player, entityequipmentslot, itemstack, hand);
            }

            return EnumActionResult::SUCCESS;
        }
        else
        {
            return EnumActionResult::SUCCESS;
        }
    }
    else
    {
        return EnumActionResult::PASS;
    }
}

bool EntityArmorStand::attackEntityFrom(DamageSource::DamageSource source, float amount)
{
    if (!world->isRemote && !isDead)
    {
        if (DamageSource::OUT_OF_WORLD == source)
        {
            setDead();
            return false;
        }
        else if (!isEntityInvulnerable(source) && !canInteract && !hasMarker())
        {
            if (source.isExplosion())
            {
                dropContents();
                setDead();
                return false;
            }
            else if (DamageSource::IN_FIRE == source)
            {
                if (isBurning())
                {
                    damageArmorStand(0.15F);
                }
                else
                {
                    setFire(5);
                }

                return false;
            }
            else if (DamageSource::ON_FIRE == source && getHealth() > 0.5F)
            {
                damageArmorStand(4.0F);
                return false;
            }
            else
            {
                bool flag  = "arrow" == (source.getDamageType());
                bool flag1 = "player" == (source.getDamageType());
                if (!flag1 && !flag)
                {
                    return false;
                }
                else
                {
                    if (Util:: instanceof <EntityArrow>(source.getImmediateSource()))
                    {
                        source.getImmediateSource()->setDead();
                    }

                    if (Util:: instanceof <EntityPlayer>(source.getTrueSource()) &&
                                              !((EntityPlayer *)source.getTrueSource())->capabilities.allowEdit)
                    {
                        return false;
                    }
                    else if (source.isCreativePlayer())
                    {
                        playBrokenSound();
                        playParticles();
                        setDead();
                        return false;
                    }
                    else
                    {
                        int64_t i = world->getTotalWorldTime();
                        if (i - punchCooldown > 5L && !flag)
                        {
                            world->setEntityState(this, (std::byte)32);
                            punchCooldown = i;
                        }
                        else
                        {
                            dropBlock();
                            playParticles();
                            setDead();
                        }

                        return false;
                    }
                }
            }
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

void EntityArmorStand::handleStatusUpdate(std::byte id)
{
    if (id == std::byte(32))
    {
        if (world->isRemote)
        {
            world->playSound(posX, posY, posZ, SoundEvents::ENTITY_ARMORSTAND_HIT, getSoundCategory(), 0.3F, 1.0F,
                             false);
            punchCooldown = world->getTotalWorldTime();
        }
    }
    else
    {
        EntityLivingBase::handleStatusUpdate(id);
    }
}

bool EntityArmorStand::isInRangeToRenderDist(double distance)
{
    double d0 = getEntityBoundingBox().getAverageEdgeLength() * 4.0;
    if (std::isnan(d0) || d0 == 0.0)
    {
        d0 = 4.0;
    }

    d0 *= 64.0;
    return distance < d0 * d0;
}

float EntityArmorStand::getEyeHeight()
{
    return isChild() ? height * 0.5F : height * 0.9F;
}

double EntityArmorStand::getYOffset()
{
    return hasMarker() ? 0.0 : 0.10000000149011612;
}

void EntityArmorStand::travel(float strafe, float vertical, float forward)
{
    if (!hasNoGravity())
    {
        EntityLivingBase::travel(strafe, vertical, forward);
    }
}

void EntityArmorStand::setRenderYawOffset(float offset)
{
    prevRenderYawOffset = prevRotationYaw = offset;
    prevRotationYawHead = rotationYawHead = offset;
}

void EntityArmorStand::setRotationYawHead(float rotation)
{
    prevRenderYawOffset = prevRotationYaw = rotation;
    prevRotationYawHead = rotationYawHead = rotation;
}

void EntityArmorStand::onUpdate()
{
    EntityLivingBase::onUpdate();
    Rotations rotations = dataManager.get(HEAD_ROTATION);
    if (!(headRotation == rotations))
    {
        setHeadRotation(rotations);
    }

    Rotations rotations1 = dataManager.get(BODY_ROTATION);
    if (!(bodyRotation == rotations1))
    {
        setBodyRotation(rotations1);
    }

    Rotations rotations2 = dataManager.get(LEFT_ARM_ROTATION);
    if (!(leftArmRotation == rotations2))
    {
        setLeftArmRotation(rotations2);
    }

    Rotations rotations3 = dataManager.get(RIGHT_ARM_ROTATION);
    if (!(rightArmRotation == rotations3))
    {
        setRightArmRotation(rotations3);
    }

    Rotations rotations4 = dataManager.get(LEFT_LEG_ROTATION);
    if (!(leftLegRotation == rotations4))
    {
        setLeftLegRotation(rotations4);
    }

    Rotations rotations5 = dataManager.get(RIGHT_LEG_ROTATION);
    if (!(rightLegRotation == rotations5))
    {
        setRightLegRotation(rotations5);
    }

    bool flag = hasMarker();
    if (wasMarker != flag)
    {
        updateBoundingBox(flag);
        preventEntitySpawning = !flag;
        wasMarker             = flag;
    }
}

void EntityArmorStand::setInvisible(bool invisible)
{
    canInteract = invisible;
    EntityLivingBase::setInvisible(invisible);
}

bool EntityArmorStand::isChild()
{
    return isSmall();
}

void EntityArmorStand::onKillCommand()
{
    setDead();
}

bool EntityArmorStand::isImmuneToExplosions()
{
    return isInvisible();
}

EnumPushReaction EntityArmorStand::getPushReaction()
{
    return hasMarker() ? EnumPushReaction::IGNORE : EntityLivingBase::getPushReaction();
}

bool EntityArmorStand::isSmall()
{
    return ((std::byte)dataManager.get(STATUS) & 1) != 0;
}

bool EntityArmorStand::getShowArms()
{
    return ((std::byte)dataManager.get(STATUS) & 4) != 0;
}

bool EntityArmorStand::hasNoBasePlate()
{
    return ((std::byte)dataManager.get(STATUS) & 8) != 0;
}

bool EntityArmorStand::hasMarker()
{
    return ((std::byte)dataManager.get(STATUS) & 16) != 0;
}

void EntityArmorStand::setHeadRotation(Rotations vec)
{
    headRotation = vec;
    dataManager.set(HEAD_ROTATION, vec);
}

void EntityArmorStand::setBodyRotation(Rotations vec)
{
    bodyRotation = vec;
    dataManager.set(BODY_ROTATION, vec);
}

void EntityArmorStand::setLeftArmRotation(Rotations vec)
{
    leftArmRotation = vec;
    dataManager.set(LEFT_ARM_ROTATION, vec);
}

void EntityArmorStand::setRightArmRotation(Rotations vec)
{
    rightArmRotation = vec;
    dataManager.set(RIGHT_ARM_ROTATION, vec);
}

void EntityArmorStand::setLeftLegRotation(Rotations vec)
{
    leftLegRotation = vec;
    dataManager.set(LEFT_LEG_ROTATION, vec);
}

void EntityArmorStand::setRightLegRotation(Rotations vec)
{
    rightLegRotation = vec;
    dataManager.set(RIGHT_LEG_ROTATION, vec);
}

Rotations EntityArmorStand::getHeadRotation() const
{
    return headRotation;
}

Rotations EntityArmorStand::getBodyRotation() const
{
    return bodyRotation;
}

Rotations EntityArmorStand::getLeftArmRotation() const
{
    return leftArmRotation;
}

Rotations EntityArmorStand::getRightArmRotation() const
{
    return rightArmRotation;
}

Rotations EntityArmorStand::getLeftLegRotation() const
{
    return leftLegRotation;
}

Rotations EntityArmorStand::getRightLegRotation() const
{
    return rightLegRotation;
}

bool EntityArmorStand::canBeCollidedWith()
{
    return EntityLivingBase::canBeCollidedWith() && !hasMarker();
}

EnumHandSide EntityArmorStand::getPrimaryHand()
{
    return EnumHandSide::RIGHT;
}

void EntityArmorStand::onStruckByLightning(EntityLightningBolt *lightningBolt)
{
}

bool EntityArmorStand::canBeHitWithPotion()
{
    return false;
}

void EntityArmorStand::notifyDataManagerChange(DataParameter key)
{
    if (STATUS == key)
    {
        setSize(0.5F, 1.975F);
    }

    EntityLivingBase::notifyDataManagerChange(key);
}

bool EntityArmorStand::attackable()
{
    return false;
}

void EntityArmorStand::registerFixesArmorStand(DataFixer fixer)
{
    fixer.registerWalker(FixTypes::ENTITY, new ItemStackDataLists(EntityArmorStand.class, {"ArmorItems", "HandItems"}));
}

void EntityArmorStand::setSize(float width, float height)
{
    double d0 = posX;
    double d1 = posY;
    double d2 = posZ;
    float f   = hasMarker() ? 0.0F : (isChild() ? 0.5F : 1.0F);
    EntityLivingBase::setSize(width * f, height * f);
    setPosition(d0, d1, d2);
}

void EntityArmorStand::entityInit()
{
    EntityLivingBase::entityInit();
    dataManager.registe(STATUS, 0);
    dataManager.registe(HEAD_ROTATION, DEFAULT_HEAD_ROTATION);
    dataManager.registe(BODY_ROTATION, DEFAULT_BODY_ROTATION);
    dataManager.registe(LEFT_ARM_ROTATION, DEFAULT_LEFTARM_ROTATION);
    dataManager.registe(RIGHT_ARM_ROTATION, DEFAULT_RIGHTARM_ROTATION);
    dataManager.registe(LEFT_LEG_ROTATION, DEFAULT_LEFTLEG_ROTATION);
    dataManager.registe(RIGHT_LEG_ROTATION, DEFAULT_RIGHTLEG_ROTATION);
}

void EntityArmorStand::collideWithNearbyEntities()
{
    auto list = world->getEntitiesInAABBexcluding(this, getEntityBoundingBox(), IS_RIDEABLE_MINECART);

    for (auto entity : list)
    {
        if (getDistanceSq(entity) <= 0.2)
        {
            entity->applyEntityCollision(this);
        }
    }
}

EntityEquipmentSlot EntityArmorStand::getClickedSlot(Vec3d p_190772_1_)
{
    EntityEquipmentSlot entityequipmentslot = EntityEquipmentSlot::MAINHAND;
    bool flag                               = isSmall();
    double d0                               = flag ? p_190772_1_.gety() * 2.0 : p_190772_1_.gety();
    if (d0 >= 0.1 && d0 < 0.1 + (flag ? 0.8 : 0.45) && hasItemInSlot(EntityEquipmentSlot::FEET))
    {
        entityequipmentslot = EntityEquipmentSlot::FEET;
    }
    else if (d0 >= 0.9 + (flag ? 0.3 : 0.0) && d0 < 0.9 + (flag ? 1.0 : 0.7) &&
             hasItemInSlot(EntityEquipmentSlot::CHEST))
    {
        entityequipmentslot = EntityEquipmentSlot::CHEST;
    }
    else if (d0 >= 0.4 && d0 < 0.4 + (flag ? 1.0 : 0.8) && hasItemInSlot(EntityEquipmentSlot::LEGS))
    {
        entityequipmentslot = EntityEquipmentSlot::LEGS;
    }
    else if (d0 >= 1.6 && hasItemInSlot(EntityEquipmentSlot::HEAD))
    {
        entityequipmentslot = EntityEquipmentSlot::HEAD;
    }

    return entityequipmentslot;
}

float EntityArmorStand::updateDistance(float p_110146_1_, float p_110146_2_)
{
    prevRenderYawOffset = prevRotationYaw;
    renderYawOffset     = rotationYaw;
    return 0.0F;
}

void EntityArmorStand::updatePotionMetadata()
{
    setInvisible(canInteract);
}

SoundEvent EntityArmorStand::getFallSound(int32_t heightIn)
{
    return SoundEvents::ENTITY_ARMORSTAND_FALL;
}

SoundEvent EntityArmorStand::getHurtSound(DamageSource::DamageSource damageSourceIn)
{
    return SoundEvents::ENTITY_ARMORSTAND_HIT;
}

SoundEvent EntityArmorStand::getDeathSound()
{
    return SoundEvents::ENTITY_ARMORSTAND_BREAK;
}

void EntityArmorStand::writePoseToNBT(NBTTagCompound *tagCompound)
{
    auto nbttaglist = tagCompound->getTagList("Head", 5);
    setHeadRotation(nbttaglist->isEmpty() ? DEFAULT_HEAD_ROTATION : Rotations(nbttaglist));
    auto nbttaglist1 = tagCompound->getTagList("Body", 5);
    setBodyRotation(nbttaglist1->isEmpty() ? DEFAULT_BODY_ROTATION : Rotations(nbttaglist1));
    auto nbttaglist2 = tagCompound->getTagList("LeftArm", 5);
    setLeftArmRotation(nbttaglist2->isEmpty() ? DEFAULT_LEFTARM_ROTATION : Rotations(nbttaglist2));
    auto nbttaglist3 = tagCompound->getTagList("RightArm", 5);
    setRightArmRotation(nbttaglist3->isEmpty() ? DEFAULT_RIGHTARM_ROTATION : Rotations(nbttaglist3));
    auto nbttaglist4 = tagCompound->getTagList("LeftLeg", 5);
    setLeftLegRotation(nbttaglist4->isEmpty() ? DEFAULT_LEFTLEG_ROTATION : Rotations(nbttaglist4));
    auto nbttaglist5 = tagCompound->getTagList("RightLeg", 5);
    setRightLegRotation(nbttaglist5->isEmpty() ? DEFAULT_RIGHTLEG_ROTATION : Rotations(nbttaglist5));
}

std::unique_ptr<NBTTagCompound> EntityArmorStand::readPoseFromNBT() const
{
    auto nbttagcompound = std::make_unique<NBTTagCompound>();
    if (!(DEFAULT_HEAD_ROTATION == headRotation))
    {
        nbttagcompound->setTag("Head", headRotation.writeToNBT());
    }

    if (!(DEFAULT_BODY_ROTATION == bodyRotation))
    {
        nbttagcompound->setTag("Body", bodyRotation.writeToNBT());
    }

    if (!(DEFAULT_LEFTARM_ROTATION == leftArmRotation))
    {
        nbttagcompound->setTag("LeftArm", leftArmRotation.writeToNBT());
    }

    if (!(DEFAULT_RIGHTARM_ROTATION == rightArmRotation))
    {
        nbttagcompound->setTag("RightArm", rightArmRotation.writeToNBT());
    }

    if (!(DEFAULT_LEFTLEG_ROTATION == leftLegRotation))
    {
        nbttagcompound->setTag("LeftLeg", leftLegRotation.writeToNBT());
    }

    if (!(DEFAULT_RIGHTLEG_ROTATION == rightLegRotation))
    {
        nbttagcompound->setTag("RightLeg", rightLegRotation.writeToNBT());
    }

    return nbttagcompound;
}

bool EntityArmorStand::isDisabled(EntityEquipmentSlot slotIn) const
{
    return (disabledSlots & 1 << slotIn.getSlotIndex()) != 0;
}

void EntityArmorStand::swapItem(EntityPlayer *player, EntityEquipmentSlot p_184795_2_, ItemStack p_184795_3_,
                                EnumHand hand)
{
    ItemStack itemstack = getItemStackFromSlot(p_184795_2_);
    if ((itemstack.isEmpty() || (disabledSlots & 1 << p_184795_2_.getSlotIndex() + 8) == 0) &&
        (!itemstack.isEmpty() || (disabledSlots & 1 << p_184795_2_.getSlotIndex() + 16) == 0))
    {
        ItemStack itemstack1 = ItemStack::EMPTY;
        if (player->capabilities.isCreativeMode && itemstack.isEmpty() && !p_184795_3_.isEmpty())
        {
            itemstack1 = p_184795_3_.copy();
            itemstack1.setCount(1);
            setItemStackToSlot(p_184795_2_, itemstack1);
        }
        else if (!p_184795_3_.isEmpty() && p_184795_3_.getCount() > 1)
        {
            if (itemstack.isEmpty())
            {
                itemstack1 = p_184795_3_.copy();
                itemstack1.setCount(1);
                setItemStackToSlot(p_184795_2_, itemstack1);
                p_184795_3_.shrink(1);
            }
        }
        else
        {
            setItemStackToSlot(p_184795_2_, p_184795_3_);
            player->setHeldItem(hand, itemstack);
        }
    }
}

void EntityArmorStand::playParticles()
{
    if (Util:: instanceof <WorldServer>(world))
    {
        ((WorldServer *)world)
            ->spawnParticle(EnumParticleTypes::BLOCK_DUST, posX, posY + (double)height / 1.5, posZ, 10,
                            (double)(width / 4.0F), (double)(height / 4.0F), (double)(width / 4.0F), 0.05,
                            Block::getStateId(Blocks::PLANKS.getDefaultState()));
    }
}

void EntityArmorStand::damageArmorStand(float damage)
{
    float f = getHealth();
    f -= damage;
    if (f <= 0.5F)
    {
        dropContents();
        setDead();
    }
    else
    {
        setHealth(f);
    }
}

void EntityArmorStand::dropBlock()
{
    Block::spawnAsEntity(world, BlockPos(this), ItemStack(Items::ARMOR_STAND));
    dropContents();
}

void EntityArmorStand::dropContents()
{
    playBrokenSound();

    int j;
    ItemStack itemstack1 = ItemStack::EMPTY;
    for (j = 0; j < handItems.size(); ++j)
    {
        itemstack1 = handItems[j];
        if (!itemstack1.isEmpty())
        {
            Block::spawnAsEntity(world, (BlockPos(this)).up(), itemstack1);
            handItems[j] = ItemStack::EMPTY;
        }
    }

    for (j = 0; j < armorItems.size(); ++j)
    {
        itemstack1 = armorItems[j];
        if (!itemstack1.isEmpty())
        {
            Block::spawnAsEntity(world, (BlockPos(this)).up(), itemstack1);
            armorItems[j] = ItemStack::EMPTY;
        }
    }
}

void EntityArmorStand::playBrokenSound()
{
    world->playSound(nullptr, posX, posY, posZ, SoundEvents::ENTITY_ARMORSTAND_BREAK, getSoundCategory(), 1.0F, 1.0F);
}

void EntityArmorStand::updateBoundingBox(bool p_181550_1_)
{
    if (p_181550_1_)
    {
        setSize(0.0F, 0.0F);
    }
    else
    {
        setSize(0.5F, 1.975F);
    }
}

void EntityArmorStand::setSmall(bool small)
{
    dataManager.set(STATUS, setBit((std::byte)dataManager.get(STATUS), 1, small));
    setSize(0.5F, 1.975F);
}

void EntityArmorStand::setShowArms(bool showArms)
{
    dataManager.set(STATUS, setBit((std::byte)dataManager.get(STATUS), 4, showArms));
}

void EntityArmorStand::setNoBasePlate(bool noBasePlate)
{
    dataManager.set(STATUS, setBit((std::byte)dataManager.get(STATUS), 8, noBasePlate));
}

void EntityArmorStand::setMarker(bool marker)
{
    dataManager.set(STATUS, setBit((std::byte)dataManager.get(STATUS), 16, marker));
    setSize(0.5F, 1.975F);
}

std::byte EntityArmorStand::setBit(std::byte p_184797_1_, uint8_t p_184797_2_, bool p_184797_3_)
{
    if (p_184797_3_)
    {
        p_184797_1_ = (p_184797_1_ | std::byte(p_184797_2_));
    }
    else
    {
        p_184797_1_ = (p_184797_1_ & ~std::byte(p_184797_2_));
    }

    return p_184797_1_;
}
