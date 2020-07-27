#include "EntityLivingBase.h"

#include "DamageSource.h"
#include "EntityDamageSource.h"
#include "../inventory/EntityEquipmentSlot.h"
#include "../item/ItemArmor.h"
#include "../potion/PotionEffect.h"
#include "../potion/PotionUtils.h"
#include "../stats/StatList.h"
#include "../world/WorldServer.h"
#include "ai/EntityAIBeg.h"
xg::Guid EntityLivingBase::SPRINTING_SPEED_BOOST_ID = xg::Guid("662A6B8D-DA3E-4C1C-8813-96EA6097278D");
std::shared_ptr<spdlog::logger> EntityLivingBase::LOGGER = spdlog::get("Minecraft")->clone("EntityLivingBase");

EntityLivingBase::EntityLivingBase(World *worldIn) :
    Entity(worldIn), handInventory(2, ItemStack::EMPTY), armorArray(4, ItemStack::EMPTY), maxHurtResistantTime(20)
    , jumpMovementFactor(0.02F), potionsNeedUpdate(true), activeItemStack(ItemStack::EMPTY)
    , randomUnused1((MathHelper::random() + 1.0f) * 0.009999999776482582f),
    randomUnused2(MathHelper::random() * 12398.0F)
    , combatTracker(this)
{
    applyEntityAttributes();
    setHealth(getMaxHealth());

    setPosition(posX, posY, posZ);
    rotationYaw = MathHelper::random() * 6.283185307179586f;
    rotationYawHead = rotationYaw;
    stepHeight = 0.6f;
    preventEntitySpawning = true;
}

void EntityLivingBase::onKillCommand()
{
    attackEntityFrom(DamageSource::OUT_OF_WORLD, std::numeric_limits<float>::max());
}

bool EntityLivingBase::canBreatheUnderwater()
{
    return false;
}

void EntityLivingBase::onEntityUpdate()
{
    prevSwingProgress = swingProgress;
    Entity::onEntityUpdate();
    world->profiler.startSection("livingEntityBaseTick");
    bool flag = Util::instanceof<EntityPlayer>(this);
    if (isEntityAlive())
    {
        if (isEntityInsideOpaqueBlock())
        {
            attackEntityFrom(DamageSource::IN_WALL, 1.0F);
        }
        else if (flag && !world->getWorldBorder().contains(getEntityBoundingBox()))
        {
            double d0 = world->getWorldBorder().getClosestDistance(this) + world->getWorldBorder().getDamageBuffer();
            if (d0 < 0.0)
            {
                double d1 = world->getWorldBorder().getDamageAmount();
                if (d1 > 0.0)
                {
                    attackEntityFrom(DamageSource::IN_WALL, MathHelper::max(1, MathHelper::floor(-d0 * d1)));
                }
            }
        }
    }

    if (isImmuneToFire() || world->isRemote)
    {
        extinguish();
    }

    bool flag1 = flag && ((EntityPlayer *)this)->capabilities.disableDamage;
    if (isEntityAlive())
    {
        if (!isInsideOfMaterial(Material::WATER))
        {
            setAir(300);
        }
        else
        {
            if (!canBreatheUnderwater() && !isPotionActive(MobEffects::WATER_BREATHING) && !flag1)
            {
                setAir(decreaseAirSupply(getAir()));
                if (getAir() == -20)
                {
                    setAir(0);

                    for (auto i = 0; i < 8; ++i)
                    {
                        float f2 = MathHelper::nextFloat(rand) - MathHelper::nextFloat(rand);
                        float f = MathHelper::nextFloat(rand) - MathHelper::nextFloat(rand);
                        float f1 = MathHelper::nextFloat(rand) - MathHelper::nextFloat(rand);
                        world->spawnParticle(EnumParticleTypes::WATER_BUBBLE, posX + (double)f2, posY + (double)f,
                                             posZ + (double)f1, motionX, motionY, motionZ);
                    }

                    attackEntityFrom(DamageSource::DROWN, 2.0F);
                }
            }

            if (!world->isRemote && isRiding() && Util::instanceof<EntityLivingBase>(getRidingEntity()))
            {
                dismountRidingEntity();
            }
        }

        if (!world->isRemote)
        {
            BlockPos blockpos(this);
            if (!(prevBlockpos == blockpos))
            {
                prevBlockpos = blockpos;
                frostWalk(blockpos);
            }
        }
    }

    if (isEntityAlive() && isWet())
    {
        extinguish();
    }

    prevCameraPitch = cameraPitch;
    if (hurtTime > 0)
    {
        --hurtTime;
    }

    if (hurtResistantTime > 0 && !(Util::instanceof<EntityPlayerMP>(this)))
    {
        --hurtResistantTime;
    }

    if (getHealth() <= 0.0F)
    {
        onDeathUpdate();
    }

    if (recentlyHit > 0)
    {
        --recentlyHit;
    }
    else
    {
        attackingPlayer = nullptr;
    }

    if (lastAttackedEntity != nullptr && !lastAttackedEntity->isEntityAlive())
    {
        lastAttackedEntity = nullptr;
    }

    if (revengeTarget != nullptr)
    {
        if (!revengeTarget->isEntityAlive())
        {
            setRevengeTarget(nullptr);
        }
        else if (ticksExisted - revengeTimer > 100)
        {
            setRevengeTarget(nullptr);
        }
    }

    updatePotionEffects();
    prevMovedDistance = movedDistance;
    prevRenderYawOffset = renderYawOffset;
    prevRotationYawHead = rotationYawHead;
    prevRotationYaw = rotationYaw;
    prevRotationPitch = rotationPitch;
    world->profiler.endSection();
}

bool EntityLivingBase::isChild()
{
    return false;
}

pcg32 &EntityLivingBase::getRNG()
{
    return rand;
}

EntityLivingBase *EntityLivingBase::getRevengeTarget() const
{
    return revengeTarget;
}

void EntityLivingBase::setRevengeTarget(EntityLivingBase *livingBase)
{
    revengeTarget = livingBase;
    revengeTimer = ticksExisted;
}

EntityLivingBase *EntityLivingBase::getLastAttackedEntity() const
{
    return lastAttackedEntity;
}

int32_t EntityLivingBase::getLastAttackedEntityTime() const
{
    return lastAttackedEntityTime;
}

void EntityLivingBase::setLastAttackedEntity(Entity *entityIn)
{
    if (Util::instanceof<EntityLivingBase>(entityIn))
    {
        lastAttackedEntity = (EntityLivingBase *)entityIn;
    }
    else
    {
        lastAttackedEntity = nullptr;
    }

    lastAttackedEntityTime = ticksExisted;
}

int32_t EntityLivingBase::getIdleTime() const
{
    return idleTime;
}

void EntityLivingBase::writeEntityToNBT(NBTTagCompound *compound)
{
    compound->setFloat("Health", getHealth());
    compound->setShort("HurtTime", (short)hurtTime);
    compound->setInteger("HurtByTimestamp", revengeTimer);
    compound->setShort("DeathTime", (short)deathTime);
    compound->setFloat("AbsorptionAmount", getAbsorptionAmount());
    auto var2 = EntityEquipmentSlot::values();

    ItemStack itemstack1 = ItemStack::EMPTY;
    for (auto entityequipmentslot1 : var2)
    {
        itemstack1 = getItemStackFromSlot(entityequipmentslot1);
        if (!itemstack1.isEmpty())
        {
            getAttributeMap().removeAttributeModifiers(itemstack1.getAttributeModifiers(entityequipmentslot1));
        }
    }

    compound->setTag("Attributes", SharedMonsterAttributes::writeBaseAttributeMapToNBT(getAttributeMap()));
    var2 = EntityEquipmentSlot::values();
    for (auto entityequipmentslot1 : var2)
    {
        itemstack1 = getItemStackFromSlot(entityequipmentslot1);
        if (!itemstack1.isEmpty())
        {
            getAttributeMap().applyAttributeModifiers(itemstack1.getAttributeModifiers(entityequipmentslot1));
        }
    }

    if (!activePotionsMap.empty())
    {
        NBTTagList *nbttaglist = new NBTTagList();

        for (auto potioneffect : activePotionsMap)
        {
            nbttaglist->appendTag(potioneffect.writeCustomPotionEffectToNBT(new NBTTagCompound()));
        }

        compound->setTag("ActiveEffects", nbttaglist);
    }

    compound->setBoolean("FallFlying", isElytraFlying());
}

void EntityLivingBase::readEntityFromNBT(NBTTagCompound *compound)
{
    setAbsorptionAmount(compound->getFloat("AbsorptionAmount"));
    if (compound->hasKey("Attributes", 9) && world != nullptr && !world->isRemote)
    {
        SharedMonsterAttributes::setAttributeModifiers(getAttributeMap(), compound->getTagList("Attributes", 10));
    }

    if (compound->hasKey("ActiveEffects", 9))
    {
        NBTTagList *nbttaglist = compound->getTagList("ActiveEffects", 10);

        for (int i = 0; i < nbttaglist->tagCount(); ++i)
        {
            NBTTagCompound *nbttagcompound = nbttaglist->getCompoundTagAt(i);
            PotionEffect potioneffect = PotionEffect::readCustomPotionEffectFromNBT(nbttagcompound);
            if (potioneffect != nullptr)
            {
                activePotionsMap.emplace(potioneffect.getPotion(), potioneffect);
            }
        }
    }

    if (compound->hasKey("Health", 99))
    {
        setHealth(compound->getFloat("Health"));
    }

    hurtTime = compound->getShort("HurtTime");
    deathTime = compound->getShort("DeathTime");
    revengeTimer = compound->getInteger("HurtByTimestamp");
    if (compound->hasKey("Team", 8))
    {
        auto s = compound->getString("Team");
        bool flag = world->getScoreboard().addPlayerToTeam(getCachedUniqueIdString(), s);
        if (!flag)
        {
            LOGGER->warn("Unable to add mob to team \"" + s + "\" (that team probably doesn't exist)");
        }
    }

    if (compound->getBoolean("FallFlying"))
    {
        setFlag(7, true);
    }
}

bool EntityLivingBase::areAllPotionsAmbient(const std::vector<PotionEffect> &potionEffects)
{
    auto var1 = potionEffects.begin();
    do
    {
        if (var1 == potionEffects.end())
        {
            return true;
        }
    }
    while (var1->getIsAmbient());

    return false;
}

void EntityLivingBase::clearActivePotions()
{
    if (!world->isRemote)
    {
        auto iterator = activePotionsMap.begin();

        while (iterator != activePotionsMap.end())
        {
            onFinishedPotionEffect(*iterator);
            activePotionsMap.erase(iterator);
        }
    }
}

std::vector<PotionEffect> EntityLivingBase::getActivePotionEffects() const
{
    std::vector<PotionEffect> peffects;
    Util::Collection(peffects, activePotionsMap);
    return peffects;
}

std::unordered_map<Potion *, PotionEffect> EntityLivingBase::getActivePotionMap() const
{
    return activePotionsMap;
}

bool EntityLivingBase::isPotionActive(Potion potionIn) const
{
    return activePotionsMap.contains(&potionIn);
}

std::optional<PotionEffect> EntityLivingBase::getActivePotionEffect(Potion potionIn)
{
    auto ite = activePotionsMap.find(potionIn);
    return ite != activePotionsMap.end() ? std::optional<PotionEffect>(ite->second) : std::nullopt;
}

void EntityLivingBase::addPotionEffect(PotionEffect potioneffectIn)
{
    if (isPotionApplicable(potioneffectIn))
    {
        auto potioneffect = activePotionsMap.find(potioneffectIn.getPotion());
        if (potioneffect == nullptr)
        {
            activePotionsMap.emplace(potioneffectIn.getPotion(), potioneffectIn);
            onNewPotionEffect(potioneffectIn);
        }
        else
        {
            potioneffect.combine(potioneffectIn);
            onChangedPotionEffect(potioneffect, true);
        }
    }
}

bool EntityLivingBase::isPotionApplicable(PotionEffect potioneffectIn)
{
    if (getCreatureAttribute() == EnumCreatureAttribute::UNDEAD)
    {
        auto potion = potioneffectIn.getPotion();
        if (potion == MobEffects::REGENERATION || potion == MobEffects::POISON)
        {
            return false;
        }
    }

    return true;
}

bool EntityLivingBase::isEntityUndead()
{
    return getCreatureAttribute() == EnumCreatureAttribute::UNDEAD;
}

PotionEffect EntityLivingBase::removeActivePotionEffect(Potion *potioneffectin)
{
    return activePotionsMap.erase(potioneffectin);
}

void EntityLivingBase::removePotionEffect(Potion *potionIn)
{
    PotionEffect potioneffect = removeActivePotionEffect(potionIn);
    if (potioneffect != nullptr)
    {
        onFinishedPotionEffect(potioneffect);
    }
}

void EntityLivingBase::heal(float healAmount)
{
    float f = getHealth();
    if (f > 0.0F)
    {
        setHealth(f + healAmount);
    }
}

float EntityLivingBase::getHealth()
{
    return dataManager.get(HEALTH);
}

bool EntityLivingBase::attackEntityFrom(DamageSource source, float amount)
{
    if (isEntityInvulnerable(source))
    {
        return false;
    }
    else if (world->isRemote)
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
        else if (source.isFireDamage() && isPotionActive(MobEffects::FIRE_RESISTANCE))
        {
            return false;
        }
        else
        {
            float f = amount;
            if ((source == DamageSource::ANVIL || source == DamageSource::FALLING_BLOCK) && !getItemStackFromSlot(
                EntityEquipmentSlot::HEAD).empty())
            {
                getItemStackFromSlot(EntityEquipmentSlot::HEAD).damageItem(
                    (int)(amount * 4.0F + MathHelper::nextFloat(rand) * amount * 2.0F), this);
                amount *= 0.75F;
            }

            bool flag = false;
            if (amount > 0.0F && canBlockDamageSource(source))
            {
                damageShield(amount);
                amount = 0.0F;
                if (!source.isProjectile())
                {
                    Entity *entity = source.getImmediateSource();
                    if (Util::instanceof<EntityLivingBase>(entity))
                    {
                        blockUsingShield((EntityLivingBase *)entity);
                    }
                }

                flag = true;
            }

            limbSwingAmount = 1.5F;
            bool flag1 = true;
            if ((float)hurtResistantTime > (float)maxHurtResistantTime / 2.0F)
            {
                if (amount <= lastDamage)
                {
                    return false;
                }

                damageEntity(source, amount - lastDamage);
                lastDamage = amount;
                flag1 = false;
            }
            else
            {
                lastDamage = amount;
                hurtResistantTime = maxHurtResistantTime;
                damageEntity(source, amount);
                maxHurtTime = 10;
                hurtTime = maxHurtTime;
            }

            attackedAtYaw = 0.0F;
            Entity *entity1 = source.getTrueSource();
            if (entity1 != nullptr)
            {
                if (Util::instanceof<EntityLivingBase>(entity1))
                {
                    setRevengeTarget((EntityLivingBase *)entity1);
                }

                if (Util::instanceof<EntityPlayer>(entity1))
                {
                    recentlyHit = 100;
                    attackingPlayer = (EntityPlayer *)entity1;
                }
                else if (Util::instanceof<EntityWolf>(entity1))
                {
                    EntityWolf *entitywolf = (EntityWolf *)entity1;
                    if (entitywolf->isTamed())
                    {
                        recentlyHit = 100;
                        attackingPlayer = nullptr;
                    }
                }
            }

            if (flag1)
            {
                if (flag)
                {
                    world->setEntityState(this, std::byte{29});
                }
                else if (Util::instanceof<DamageSource::EntityDamageSource>(source) && ((DamageSource::EntityDamageSource *)source).getIsThornsDamage())
                {
                    world->setEntityState(this, std::byte{33});
                }
                else
                {
                    std::byte b0;
                    if (source == DamageSource::DROWN)
                    {
                        b0 = std::byte{36};
                    }
                    else if (source.isFireDamage())
                    {
                        b0 = std::byte{37};
                    }
                    else
                    {
                        b0 = std::byte{2};
                    }

                    world->setEntityState(this, b0);
                }

                if (source != DamageSource::DROWN && (!flag || amount > 0.0F))
                {
                    markVelocityChanged();
                }

                if (entity1 != nullptr)
                {
                    double d1 = entity1->posX - posX;

                    double d0;
                    for (d0 = entity1->posZ - posZ; d1 * d1 + d0 * d0 < 1.0E-4; d0 = (MathHelper::random() -
                             MathHelper::random()) * 0.01)
                    {
                        d1 = (MathHelper::random() - MathHelper::random()) * 0.01;
                    }

                    attackedAtYaw = (float)(MathHelper::atan2(d0, d1) * 57.29577951308232 - (double)rotationYaw);
                    knockBack(entity1, 0.4F, d1, d0);
                }
                else
                {
                    attackedAtYaw = (float)((int)(MathHelper::random() * 2.0) * 180);
                }
            }

            if (getHealth() <= 0.0F)
            {
                if (!checkTotemDeathProtection(source))
                {
                    SoundEvent soundevent = getDeathSound();
                    if (flag1 && soundevent != nullptr)
                    {
                        playSound(soundevent, getSoundVolume(), getSoundPitch());
                    }

                    onDeath(source);
                }
            }
            else if (flag1)
            {
                playHurtSound(source);
            }

            bool flag2 = !flag || amount > 0.0F;
            if (flag2)
            {
                lastDamageSource = source;
                lastDamageStamp = world->getTotalWorldTime();
            }

            if (Util::instanceof<EntityPlayerMP>(this))
            {
                CriteriaTriggers.ENTITY_HURT_PLAYER.trigger((EntityPlayerMP *)this, source, f, amount, flag);
            }

            if (Util::instanceof<EntityPlayerMP>(entity1))
            {
                CriteriaTriggers.PLAYER_HURT_ENTITY.trigger((EntityPlayerMP *)entity1, this, source, f, amount, flag);
            }

            return flag2;
        }
    }
}

void EntityLivingBase::setHealth(float health)
{
    dataManager.set(HEALTH, MathHelper::clamp(health, 0.0F, getMaxHealth()));
}

void EntityLivingBase::entityInit()
{
    dataManager.


    
    register
    (HAND_STATES, 0);
    dataManager.


    
    register
    (POTION_EFFECTS, 0);
    dataManager.


    
    register
    (HIDE_PARTICLES, false);
    dataManager.


    
    register
    (ARROW_COUNT_IN_ENTITY, 0);
    dataManager.


    
    register
    (HEALTH, 1.0F);
}

void EntityLivingBase::applyEntityAttributes()
{
    getAttributeMap().registerAttribute(SharedMonsterAttributes::MAX_HEALTH);
    getAttributeMap().registerAttribute(SharedMonsterAttributes::KNOCKBACK_RESISTANCE);
    getAttributeMap().registerAttribute(SharedMonsterAttributes::MOVEMENT_SPEED);
    getAttributeMap().registerAttribute(SharedMonsterAttributes::ARMOR);
    getAttributeMap().registerAttribute(SharedMonsterAttributes::ARMOR_TOUGHNESS);
}

void EntityLivingBase::updateFallState(double y, bool onGroundIn, IBlockState *state, const BlockPos &pos)
{
    if (!isInWater())
    {
        handleWaterMovement();
    }

    if (!world->isRemote && fallDistance > 3.0F && onGroundIn)
    {
        float f = (float)MathHelper::ceil(fallDistance - 3.0F);
        if (state->getMaterial() != Material::AIR)
        {
            double d0 = MathHelper::min((double)(0.2F + f / 15.0F), 2.5);
            int32_t i = 150.0 * d0;
            ((WorldServer *)world)->spawnParticle(EnumParticleTypes::BLOCK_DUST, posX, posY, posZ, i, 0.0, 0.0, 0.0,
                                                  0.15000000596046448, {Block::getStateId(state)});
        }
    }

    Entity::updateFallState(y, onGroundIn, state, pos);
}

void EntityLivingBase::frostWalk(const BlockPos &pos)
{
    int32_t i = EnchantmentHelper::getMaxEnchantmentLevel(Enchantments::FROST_WALKER, this);
    if (i > 0)
    {
        EnchantmentFrostWalker::freezeNearby(this, world, pos, i);
    }
}

void EntityLivingBase::onDeathUpdate()
{
    ++deathTime;
    if (deathTime == 20)
    {
        int32_t k;
        if (!world->isRemote && (isPlayer() || recentlyHit > 0 && canDropLoot() && world
                                                                                   ->getGameRules().getBoolean(
                                                                                       "doMobLoot")))
        {
            k = getExperiencePoints(attackingPlayer);

            while (k > 0)
            {
                int32_t j = EntityXPOrb::getXPSplit(k);
                k -= j;
                world->spawnEntity(new EntityXPOrb(world, posX, posY, posZ, j));
            }
        }

        setDead();

        for (k = 0; k < 20; ++k)
        {
            double d2 = MathHelper::nextGaussian(rand) * 0.02;
            double d0 = MathHelper::nextGaussian(rand) * 0.02;
            double d1 = MathHelper::nextGaussian(rand) * 0.02;
            world->spawnParticle(EnumParticleTypes::EXPLOSION_NORMAL,
                                 posX + (double)(MathHelper::nextFloat(rand) * width * 2.0F) - (double)
                                 width, posY + (double)(MathHelper::nextFloat(rand) * height),
                                 posZ + (double)(MathHelper::nextFloat(rand) * width * 2.0F) - (double)
                                 width, d2, d0, d1);
        }
    }
}

bool EntityLivingBase::canDropLoot()
{
    return !isChild();
}

int32_t EntityLivingBase::decreaseAirSupply(int32_t air)
{
    int32_t i = EnchantmentHelper::getRespirationModifier(this);
    return i > 0 && rand(i + 1) > 0 ? air : air - 1;
}

int32_t EntityLivingBase::getExperiencePoints(EntityPlayer *player)
{
    return 0;
}

bool EntityLivingBase::isPlayer()
{
    return false;
}

void EntityLivingBase::playEquipSound(ItemStack stack)
{
    if (!stack.isEmpty())
    {
        SoundEvent soundevent = SoundEvents::ITEM_ARMOR_EQUIP_GENERIC;
        auto item = stack.getItem();
        if (Util::instanceof<ItemArmor>(item))
        {
            soundevent = ((ItemArmor *)item)->getArmorMaterial().getSoundEvent();
        }
        else if (item == Items::ELYTRA)
        {
            soundevent = SoundEvents::ITEM_ARMOR_EQIIP_ELYTRA;
        }

        playSound(soundevent, 1.0F, 1.0F);
    }
}

void EntityLivingBase::updatePotionEffects()
{
    try
    {
        for (auto keySet : activePotionsMap)
        {
            Potion *potion = keySet.first;
            PotionEffect potioneffect = keySet.second;
            if (!potioneffect.onUpdate(this))
            {
                if (!world->isRemote)
                {
                    activePotionsMap.erase();
                    onFinishedPotionEffect(potioneffect);
                }
            }
            else if (potioneffect.getDuration() % 600 == 0)
            {
                onChangedPotionEffect(potioneffect, false);
            }
        }
    }
    catch (ConcurrentModificationException var11)
    {
    }

    if (potionsNeedUpdate)
    {
        if (!world->isRemote)
        {
            updatePotionMetadata();
        }

        potionsNeedUpdate = false;
    }

    int32_t i = dataManager.get(POTION_EFFECTS);
    bool flag1 = dataManager.get(HIDE_PARTICLES);
    if (i > 0)
    {
        bool flag;
        if (isInvisible())
        {
            flag = rand(15) == 0;
        }
        else
        {
            flag = nextBoolean(rand);
        }

        if (flag1)
        {
            flag &= rand(5) == 0;
        }

        if (flag && i > 0)
        {
            double d0 = (double)(i >> 16 & 255) / 255.0;
            double d1 = (double)(i >> 8 & 255) / 255.0;
            double d2 = (double)(i >> 0 & 255) / 255.0;
            world->spawnParticle(flag1 ? EnumParticleTypes::SPELL_MOB_AMBIENT : EnumParticleTypes::SPELL_MOB,
                                 posX + (MathHelper::nextDouble(rand) - 0.5) * (double)width,
                                 posY + MathHelper::nextDouble(rand) * (double)height,
                                 posZ + (MathHelper::nextDouble(rand) - 0.5) * (double)width, d0, d1, d2);
        }
    }
}

void EntityLivingBase::updatePotionMetadata()
{
    if (activePotionsMap.empty())
    {
        resetPotionEffectMetadata();
        setInvisible(false);
    }
    else
    {
        auto collection = activePotionsMap;
        dataManager.set(HIDE_PARTICLES, areAllPotionsAmbient(collection));
        dataManager.set(POTION_EFFECTS, PotionUtils::getPotionColorFromEffectList(collection));
        setInvisible(isPotionActive(MobEffects::INVISIBILITY));
    }
}

void EntityLivingBase::resetPotionEffectMetadata()
{
    dataManager.set(HIDE_PARTICLES, false);
    dataManager.set(POTION_EFFECTS, 0);
}

void EntityLivingBase::onNewPotionEffect(PotionEffect id)
{
    potionsNeedUpdate = true;
    if (!world->isRemote)
    {
        id.getPotion().applyAttributesModifiersToEntity(this, getAttributeMap(), id.getAmplifier());
    }
}

void EntityLivingBase::onChangedPotionEffect(PotionEffect id, bool p_70695_2_)
{
    potionsNeedUpdate = true;
    if (p_70695_2_ && !world->isRemote)
    {
        Potion potion = id.getPotion();
        potion.removeAttributesModifiersFromEntity(this, getAttributeMap(), id.getAmplifier());
        potion.applyAttributesModifiersToEntity(this, getAttributeMap(), id.getAmplifier());
    }
}

void EntityLivingBase::onFinishedPotionEffect(PotionEffect effect)
{
    potionsNeedUpdate = true;
    if (!world->isRemote)
    {
        effect.getPotion().removeAttributesModifiersFromEntity(this, getAttributeMap(), effect.getAmplifier());
    }
}

void EntityLivingBase::blockUsingShield(EntityLivingBase *p_190629_1_)
{
    p_190629_1_->knockBack(this, 0.5F, posX - p_190629_1_->posX, posZ - p_190629_1_->posZ);
}

bool EntityLivingBase::checkTotemDeathProtection(DamageSource p_190628_1_)
{
    if (p_190628_1_.canHarmInCreative()) {
         return false;
      } else {
         ItemStack itemstack = ItemStack::EMPTY;
         EnumHand[] var3 = EnumHand::values();
         int var4 = var3.length;

         for(int var5 = 0; var5 < var4; ++var5) {
            EnumHand enumhand = var3[var5];
            ItemStack itemstack1 = getHeldItem(enumhand);
            if (itemstack1.getItem() == Items::TOTEM_OF_UNDYING) {
               itemstack = itemstack1.copy();
               itemstack1.shrink(1);
               break;
            }
         }

         if (!itemstack.isEmpty()) {
            if (Util::instanceof<EntityPlayerMP>(this)) {
               EntityPlayerMP* entityplayermp = (EntityPlayerMP*)this;
               entityplayermp->addStat(StatList::getObjectUseStats(Items::TOTEM_OF_UNDYING));
               CriteriaTriggers::USED_TOTEM.trigger(entityplayermp, itemstack);
            }

            setHealth(1.0F);
            clearActivePotions();
            addPotionEffect(PotionEffect(MobEffects::REGENERATION, 900, 1));
            addPotionEffect(PotionEffect(MobEffects::ABSORPTION, 100, 1));
            world->setEntityState(this, std::byte{35});
         }

         return itemstack.isEmpty();
      }
}

DamageSource EntityLivingBase::getLastDamageSource()
{
    if (world->getTotalWorldTime() - lastDamageStamp > 40L) 
    {
        lastDamageSource = nullptr;
    }

    return lastDamageSource;
}

void EntityLivingBase::renderBrokenItemStack(const ItemStack &stack)
{
    playSound(SoundEvents::ENTITY_ITEM_BREAK, 0.8F, 0.8F + MathHelper::nextFloat(world->rand) * 0.4F);

    for(int i = 0; i < 5; ++i) 
    {
        Vec3d vec3d = Vec3d(((double)MathHelper::nextFloat(rand) - 0.5) * 0.1, MathHelper::random() * 0.1 + 0.1, 0.0);
        vec3d = vec3d.rotatePitch(-rotationPitch * 0.017453292F);
        vec3d = vec3d.rotateYaw(-rotationYaw * 0.017453292F);
        double d0 = (double)(-MathHelper::nextFloat(rand)) * 0.6 - 0.3;
        Vec3d vec3d1 = Vec3d(((double)MathHelper::nextFloat(rand) - 0.5) * 0.3, d0, 0.6);
        vec3d1 = vec3d1.rotatePitch(-rotationPitch * 0.017453292F);
        vec3d1 = vec3d1.rotateYaw(-rotationYaw * 0.017453292F);
        vec3d1 = vec3d1.add(posX, posY + (double)getEyeHeight(), posZ);
        world->spawnParticle(EnumParticleTypes::ITEM_CRACK, vec3d1.getx(), vec3d1.gety(), vec3d1.getz(), vec3d.getx(), vec3d.gety() + 0.05, vec3d.getz(), Item::getIdFromItem(stack.getItem()));
    }
}

void EntityLivingBase::playHurtSound(DamageSource source)
{
    SoundEvent soundevent = getHurtSound(source);
    if (soundevent != nullptr) 
    {
        playSound(soundevent, getSoundVolume(), getSoundPitch());
    }
}

void EntityLivingBase::onDeath(DamageSource cause)
{
    if (!dead) 
    {
        Entity* entity = cause.getTrueSource();
        EntityLivingBase* entitylivingbase = getAttackingEntity();
        if (scoreValue >= 0 && entitylivingbase != nullptr) 
        {
            entitylivingbase->awardKillScore(this, scoreValue, cause);
        }

        if (entity != nullptr) 
        {
            entity->onKillEntity(this);
        }

        dead = true;
        getCombatTracker().reset();
        if (!world->isRemote) 
        {
            int i = 0;
            if (Util::instanceof<EntityPlayer>(entity)) 
            {
                i = EnchantmentHelper::getLootingModifier((EntityLivingBase*)entity);
            }

            if (canDropLoot() && world->getGameRules().getBoolean("doMobLoot")) 
            {
                bool flag = recentlyHit > 0;
                dropLoot(flag, i, cause);
            }
        }

        world->setEntityState(this, std::byte{3});
    }
}

bool EntityLivingBase::canBlockDamageSource(DamageSource damageSourceIn)
{
    if (!damageSourceIn.isUnblockable() && this.isActiveItemStackBlocking()) 
    {
        Vec3d vec3d = damageSourceIn.getDamageLocation();
        if (vec3d != nullptr) 
        {
            Vec3d vec3d1 = getLook(1.0F);
            Vec3d vec3d2 = vec3d.subtractReverse(Vec3d(posX, posY, posZ)).normalize();
            vec3d2 = Vec3d(vec3d2.getx(), 0.0, vec3d2.getz());
            if (vec3d2.dotProduct(vec3d1) < 0.0) 
            {
                return true;
            }
        }
    }

    return false;
}