#include "EntityLivingBase.h"

#include <array>
#include "CombatRules.h"
#include "DamageSource.h"
#include "EntityDamageSource.h"
#include "EnumHandSide.h"
#include "../inventory/ContainerHorseInventory.h"
#include "../inventory/EntityEquipmentSlot.h"
#include "../item/ItemArmor.h"
#include "../item/ItemElytra.h"
#include "../potion/PotionEffect.h"
#include "../potion/PotionUtils.h"
#include "../stats/StatList.h"
#include "../world/WorldServer.h"
#include "ai/EntityAIBeg.h"
#include "ai/attributes/AbstractAttributeMap.h"
#include "ai/attributes/AttributeMap.h"
#include "ai/attributes/IAttributeInstance.h"
#include "spdlog/sinks/null_sink.h"
xg::Guid EntityLivingBase::SPRINTING_SPEED_BOOST_ID = xg::Guid("662A6B8D-DA3E-4C1C-8813-96EA6097278D");
std::shared_ptr<spdlog::logger> EntityLivingBase::LOGGER = spdlog::get("Minecraft")->clone("EntityLivingBase");

SPRINTING_SPEED_BOOST = (new AttributeModifier(EntityLivingBase::SPRINTING_SPEED_BOOST_ID, "Sprinting speed boost", 0.30000001192092896, 2)).setSaved(false);
HAND_STATES = EntityDataManager.createKey(EntityLivingBase.class, DataSerializers.BYTE);
HEALTH = EntityDataManager.createKey(EntityLivingBase.class, DataSerializers.FLOAT);
POTION_EFFECTS = EntityDataManager.createKey(EntityLivingBase.class, DataSerializers.VARINT);
HIDE_PARTICLES = EntityDataManager.createKey(EntityLivingBase.class, DataSerializers.BOOLEAN);
ARROW_COUNT_IN_ENTITY = EntityDataManager.createKey(EntityLivingBase.class, DataSerializers.VARINT);


EntityLivingBase::EntityLivingBase(World *worldIn) :
    Entity(worldIn), handInventory(2, ItemStack::EMPTY), armorArray(4, ItemStack::EMPTY), maxHurtResistantTime(20)
    , jumpMovementFactor(0.02F), potionsNeedUpdate(true), activeItemStack(ItemStack::EMPTY)
    , randomUnused1((MathHelper::random() + 1.0f) * 0.009999999776482582f),
    randomUnused2(MathHelper::random() * 12398.0F)
    , combatTracker(this) {
    applyEntityAttributes();
    setHealth(getMaxHealth());

    setPosition(posX, posY, posZ);
    rotationYaw = MathHelper::random() * 6.283185307179586f;
    rotationYawHead = rotationYaw;
    stepHeight = 0.6f;
    preventEntitySpawning = true;
}

void EntityLivingBase::onKillCommand() {
    attackEntityFrom(DamageSource::OUT_OF_WORLD, std::numeric_limits<float>::max());
}

bool EntityLivingBase::canBreatheUnderwater() {
    return false;
}

void EntityLivingBase::onEntityUpdate() {
    prevSwingProgress = swingProgress;
    Entity::onEntityUpdate();
    world->profiler.startSection("livingEntityBaseTick");
    bool flag = Util::instanceof<EntityPlayer>(this);
    if (isEntityAlive()) {
        if (isEntityInsideOpaqueBlock()) {
            attackEntityFrom(DamageSource::IN_WALL, 1.0F);
        } else if (flag && !world->getWorldBorder().contains(getEntityBoundingBox())) {
            double d0 = world->getWorldBorder().getClosestDistance(this) + world->getWorldBorder().getDamageBuffer();
            if (d0 < 0.0) {
                double d1 = world->getWorldBorder().getDamageAmount();
                if (d1 > 0.0) {
                    attackEntityFrom(DamageSource::IN_WALL, MathHelper::max(1, MathHelper::floor(-d0 * d1)));
                }
            }
        }
    }

    if (isImmuneToFire() || world->isRemote) {
        extinguish();
    }

    bool flag1 = flag && ((EntityPlayer *)this)->capabilities.disableDamage;
    if (isEntityAlive()) {
        if (!isInsideOfMaterial(Material::WATER)) {
            setAir(300);
        } else {
            if (!canBreatheUnderwater() && !isPotionActive(MobEffects::WATER_BREATHING) && !flag1) {
                setAir(decreaseAirSupply(getAir()));
                if (getAir() == -20) {
                    setAir(0);

                    for (auto i = 0; i < 8; ++i) {
                        float f2 = MathHelper::nextFloat(rand) - MathHelper::nextFloat(rand);
                        float f = MathHelper::nextFloat(rand) - MathHelper::nextFloat(rand);
                        float f1 = MathHelper::nextFloat(rand) - MathHelper::nextFloat(rand);
                        world->spawnParticle(EnumParticleTypes::WATER_BUBBLE, posX + (double)f2, posY + (double)f,
                                             posZ + (double)f1, motionX, motionY, motionZ);
                    }

                    attackEntityFrom(DamageSource::DROWN, 2.0F);
                }
            }

            if (!world->isRemote && isRiding() && Util::instanceof<EntityLivingBase>(getRidingEntity())) {
                dismountRidingEntity();
            }
        }

        if (!world->isRemote) {
            BlockPos blockpos(this);
            if (!(prevBlockpos == blockpos)) {
                prevBlockpos = blockpos;
                frostWalk(blockpos);
            }
        }
    }

    if (isEntityAlive() && isWet()) {
        extinguish();
    }

    prevCameraPitch = cameraPitch;
    if (hurtTime > 0) {
        --hurtTime;
    }

    if (hurtResistantTime > 0 && !(Util::instanceof<EntityPlayerMP>(this))) {
        --hurtResistantTime;
    }

    if (getHealth() <= 0.0F) {
        onDeathUpdate();
    }

    if (recentlyHit > 0) {
        --recentlyHit;
    } else {
        attackingPlayer = nullptr;
    }

    if (lastAttackedEntity != nullptr && !lastAttackedEntity->isEntityAlive()) {
        lastAttackedEntity = nullptr;
    }

    if (revengeTarget != nullptr) {
        if (!revengeTarget->isEntityAlive()) {
            setRevengeTarget(nullptr);
        } else if (ticksExisted - revengeTimer > 100) {
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

bool EntityLivingBase::isChild() {
    return false;
}

pcg32 &EntityLivingBase::getRNG() {
    return rand;
}

EntityLivingBase *EntityLivingBase::getRevengeTarget() const {
    return revengeTarget;
}

void EntityLivingBase::setRevengeTarget(EntityLivingBase *livingBase) {
    revengeTarget = livingBase;
    revengeTimer = ticksExisted;
}

EntityLivingBase *EntityLivingBase::getLastAttackedEntity() const {
    return lastAttackedEntity;
}

int32_t EntityLivingBase::getLastAttackedEntityTime() const {
    return lastAttackedEntityTime;
}

void EntityLivingBase::setLastAttackedEntity(Entity *entityIn) {
    if (Util::instanceof<EntityLivingBase>(entityIn)) {
        lastAttackedEntity = (EntityLivingBase *)entityIn;
    } else {
        lastAttackedEntity = nullptr;
    }

    lastAttackedEntityTime = ticksExisted;
}

int32_t EntityLivingBase::getIdleTime() const {
    return idleTime;
}

void EntityLivingBase::writeEntityToNBT(NBTTagCompound *compound) {
    compound->setFloat("Health", getHealth());
    compound->setShort("HurtTime", (short)hurtTime);
    compound->setInteger("HurtByTimestamp", revengeTimer);
    compound->setShort("DeathTime", (short)deathTime);
    compound->setFloat("AbsorptionAmount", getAbsorptionAmount());
    auto var2 = EntityEquipmentSlot::values();

    ItemStack itemstack1 = ItemStack::EMPTY;
    for (auto entityequipmentslot1 : var2) {
        itemstack1 = getItemStackFromSlot(entityequipmentslot1);
        if (!itemstack1.isEmpty()) {
            getAttributeMap()->removeAttributeModifiers(itemstack1.getAttributeModifiers(entityequipmentslot1));
        }
    }

    compound->setTag("Attributes", SharedMonsterAttributes::writeBaseAttributeMapToNBT(getAttributeMap()));
    var2 = EntityEquipmentSlot::values();
    for (auto entityequipmentslot1 : var2) {
        itemstack1 = getItemStackFromSlot(entityequipmentslot1);
        if (!itemstack1.isEmpty()) {
            getAttributeMap()->applyAttributeModifiers(itemstack1.getAttributeModifiers(entityequipmentslot1));
        }
    }

    if (!activePotionsMap.empty()) {
        NBTTagList *nbttaglist = new NBTTagList();

        for (auto potioneffect : activePotionsMap) {
            nbttaglist->appendTag(potioneffect.writeCustomPotionEffectToNBT(new NBTTagCompound()));
        }

        compound->setTag("ActiveEffects", nbttaglist);
    }

    compound->setBoolean("FallFlying", isElytraFlying());
}

void EntityLivingBase::readEntityFromNBT(NBTTagCompound *compound) {
    setAbsorptionAmount(compound->getFloat("AbsorptionAmount"));
    if (compound->hasKey("Attributes", 9) && world != nullptr && !world->isRemote) {
        SharedMonsterAttributes::setAttributeModifiers(getAttributeMap(), compound->getTagList("Attributes", 10));
    }

    if (compound->hasKey("ActiveEffects", 9)) {
        NBTTagList *nbttaglist = compound->getTagList("ActiveEffects", 10);

        for (int i = 0; i < nbttaglist->tagCount(); ++i) {
            NBTTagCompound *nbttagcompound = nbttaglist->getCompoundTagAt(i);
            PotionEffect potioneffect = PotionEffect::readCustomPotionEffectFromNBT(nbttagcompound);
            if (potioneffect != nullptr) {
                activePotionsMap.emplace(potioneffect.getPotion(), potioneffect);
            }
        }
    }

    if (compound->hasKey("Health", 99)) {
        setHealth(compound->getFloat("Health"));
    }

    hurtTime = compound->getShort("HurtTime");
    deathTime = compound->getShort("DeathTime");
    revengeTimer = compound->getInteger("HurtByTimestamp");
    if (compound->hasKey("Team", 8)) {
        auto s = compound->getString("Team");
        bool flag = world->getScoreboard().addPlayerToTeam(getCachedUniqueIdString(), s);
        if (!flag) {
            LOGGER->warn("Unable to add mob to team \"" + s + "\" (that team probably doesn't exist)");
        }
    }

    if (compound->getBoolean("FallFlying")) {
        setFlag(7, true);
    }
}

bool EntityLivingBase::areAllPotionsAmbient(const std::vector<PotionEffect> &potionEffects) {
    auto var1 = potionEffects.begin();
    do {
        if (var1 == potionEffects.end()) {
            return true;
        }
    }
    while (var1->getIsAmbient());

    return false;
}

void EntityLivingBase::clearActivePotions() {
    if (!world->isRemote) {
        auto iterator = activePotionsMap.begin();

        while (iterator != activePotionsMap.end()) {
            onFinishedPotionEffect(*iterator);
            activePotionsMap.erase(iterator);
        }
    }
}

std::vector<PotionEffect> EntityLivingBase::getActivePotionEffects() const {
    std::vector<PotionEffect> peffects;
    Util::Collection(peffects, activePotionsMap);
    return peffects;
}

std::unordered_map<Potion *, PotionEffect> EntityLivingBase::getActivePotionMap() const {
    return activePotionsMap;
}

bool EntityLivingBase::isPotionActive(Potion potionIn) const {
    return activePotionsMap.contains(&potionIn);
}

std::optional<PotionEffect> EntityLivingBase::getActivePotionEffect(Potion potionIn) {
    auto ite = activePotionsMap.find(potionIn);
    return ite != activePotionsMap.end() ? std::optional<PotionEffect>(ite->second) : std::nullopt;
}

void EntityLivingBase::addPotionEffect(PotionEffect potioneffectIn) {
    if (isPotionApplicable(potioneffectIn)) {
        auto potioneffect = activePotionsMap.find(potioneffectIn.getPotion());
        if (potioneffect == nullptr) {
            activePotionsMap.emplace(potioneffectIn.getPotion(), potioneffectIn);
            onNewPotionEffect(potioneffectIn);
        } else {
            potioneffect.combine(potioneffectIn);
            onChangedPotionEffect(potioneffect, true);
        }
    }
}

bool EntityLivingBase::isPotionApplicable(PotionEffect potioneffectIn) {
    if (getCreatureAttribute() == EnumCreatureAttribute::UNDEAD) {
        auto potion = potioneffectIn.getPotion();
        if (potion == MobEffects::REGENERATION || potion == MobEffects::POISON) {
            return false;
        }
    }

    return true;
}

bool EntityLivingBase::isEntityUndead() {
    return getCreatureAttribute() == EnumCreatureAttribute::UNDEAD;
}

PotionEffect EntityLivingBase::removeActivePotionEffect(Potion *potioneffectin) {
    return activePotionsMap.erase(potioneffectin);
}

void EntityLivingBase::removePotionEffect(Potion *potionIn) {
    PotionEffect potioneffect = removeActivePotionEffect(potionIn);
    if (potioneffect != nullptr) {
        onFinishedPotionEffect(potioneffect);
    }
}

void EntityLivingBase::heal(float healAmount) {
    float f = getHealth();
    if (f > 0.0F) {
        setHealth(f + healAmount);
    }
}

float EntityLivingBase::getHealth() {
    return dataManager.get(HEALTH);
}

bool EntityLivingBase::attackEntityFrom(DamageSource::DamageSource source, float amount) {
    if (isEntityInvulnerable(source)) {
        return false;
    } else if (world->isRemote) {
        return false;
    } else {
        idleTime = 0;
        if (getHealth() <= 0.0F) {
            return false;
        } else if (source.isFireDamage() && isPotionActive(MobEffects::FIRE_RESISTANCE)) {
            return false;
        } else {
            float f = amount;
            if ((source == DamageSource::ANVIL || source == DamageSource::FALLING_BLOCK) && !getItemStackFromSlot(
                    EntityEquipmentSlot::HEAD).empty()) {
                getItemStackFromSlot(EntityEquipmentSlot::HEAD).damageItem(
                    (int)(amount * 4.0F + MathHelper::nextFloat(rand) * amount * 2.0F), this);
                amount *= 0.75F;
            }

            bool flag = false;
            if (amount > 0.0F && canBlockDamageSource(source)) {
                damageShield(amount);
                amount = 0.0F;
                if (!source.isProjectile()) {
                    Entity *entity = source.getImmediateSource();
                    if (Util::instanceof<EntityLivingBase>(entity)) {
                        blockUsingShield((EntityLivingBase *)entity);
                    }
                }

                flag = true;
            }

            limbSwingAmount = 1.5F;
            bool flag1 = true;
            if ((float)hurtResistantTime > (float)maxHurtResistantTime / 2.0F) {
                if (amount <= lastDamage) {
                    return false;
                }

                damageEntity(source, amount - lastDamage);
                lastDamage = amount;
                flag1 = false;
            } else {
                lastDamage = amount;
                hurtResistantTime = maxHurtResistantTime;
                damageEntity(source, amount);
                maxHurtTime = 10;
                hurtTime = maxHurtTime;
            }

            attackedAtYaw = 0.0F;
            Entity *entity1 = source.getTrueSource();
            if (entity1 != nullptr) {
                if (Util::instanceof<EntityLivingBase>(entity1)) {
                    setRevengeTarget((EntityLivingBase *)entity1);
                }

                if (Util::instanceof<EntityPlayer>(entity1)) {
                    recentlyHit = 100;
                    attackingPlayer = (EntityPlayer *)entity1;
                } else if (Util::instanceof<EntityWolf>(entity1)) {
                    EntityWolf *entitywolf = (EntityWolf *)entity1;
                    if (entitywolf->isTamed()) {
                        recentlyHit = 100;
                        attackingPlayer = nullptr;
                    }
                }
            }

            if (flag1) {
                if (flag) {
                    world->setEntityState(this, std::byte{29});
                } else if (Util::instanceof<DamageSource::EntityDamageSource>(source) && ((
                               DamageSource::EntityDamageSource *)source).getIsThornsDamage()) {
                    world->setEntityState(this, std::byte{33});
                } else {
                    std::byte b0;
                    if (source == DamageSource::DROWN) {
                        b0 = std::byte{36};
                    } else if (source.isFireDamage()) {
                        b0 = std::byte{37};
                    } else {
                        b0 = std::byte{2};
                    }

                    world->setEntityState(this, b0);
                }

                if (source != DamageSource::DROWN && (!flag || amount > 0.0F)) {
                    markVelocityChanged();
                }

                if (entity1 != nullptr) {
                    double d1 = entity1->posX - posX;

                    double d0;
                    for (d0 = entity1->posZ - posZ; d1 * d1 + d0 * d0 < 1.0E-4; d0 = (MathHelper::random() -
                                                                                      MathHelper::random()) * 0.01) {
                        d1 = (MathHelper::random() - MathHelper::random()) * 0.01;
                    }

                    attackedAtYaw = (float)(MathHelper::atan2(d0, d1) * 57.29577951308232 - (double)rotationYaw);
                    knockBack(entity1, 0.4F, d1, d0);
                } else {
                    attackedAtYaw = (float)((int)(MathHelper::random() * 2.0) * 180);
                }
            }

            if (getHealth() <= 0.0F) {
                if (!checkTotemDeathProtection(source)) {
                    SoundEvent soundevent = getDeathSound();
                    if (flag1 && soundevent != nullptr) {
                        playSound(soundevent, getSoundVolume(), getSoundPitch());
                    }

                    onDeath(source);
                }
            } else if (flag1) {
                playHurtSound(source);
            }

            bool flag2 = !flag || amount > 0.0F;
            if (flag2) {
                lastDamageSource = source;
                lastDamageStamp = world->getTotalWorldTime();
            }

            if (Util::instanceof<EntityPlayerMP>(this)) {
                CriteriaTriggers.ENTITY_HURT_PLAYER.trigger((EntityPlayerMP *)this, source, f, amount, flag);
            }

            if (Util::instanceof<EntityPlayerMP>(entity1)) {
                CriteriaTriggers.PLAYER_HURT_ENTITY.trigger((EntityPlayerMP *)entity1, this, source, f, amount, flag);
            }

            return flag2;
        }
    }
}

void EntityLivingBase::setHealth(float health) {
    dataManager.set(HEALTH, MathHelper::clamp(health, 0.0F, getMaxHealth()));
}

void EntityLivingBase::entityInit() {
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

void EntityLivingBase::applyEntityAttributes() {
    getAttributeMap()->registerAttribute(SharedMonsterAttributes::MAX_HEALTH);
    getAttributeMap()->registerAttribute(SharedMonsterAttributes::KNOCKBACK_RESISTANCE);
    getAttributeMap()->registerAttribute(SharedMonsterAttributes::MOVEMENT_SPEED);
    getAttributeMap()->registerAttribute(SharedMonsterAttributes::ARMOR);
    getAttributeMap()->registerAttribute(SharedMonsterAttributes::ARMOR_TOUGHNESS);
}

void EntityLivingBase::updateFallState(double y, bool onGroundIn, IBlockState *state, const BlockPos &pos) {
    if (!isInWater()) {
        handleWaterMovement();
    }

    if (!world->isRemote && fallDistance > 3.0F && onGroundIn) {
        float f = (float)MathHelper::ceil(fallDistance - 3.0F);
        if (state->getMaterial() != Material::AIR) {
            double d0 = MathHelper::min((double)(0.2F + f / 15.0F), 2.5);
            int32_t i = 150.0 * d0;
            ((WorldServer *)world)->spawnParticle(EnumParticleTypes::BLOCK_DUST, posX, posY, posZ, i, 0.0, 0.0, 0.0,
                                                  0.15000000596046448, {Block::getStateId(state)});
        }
    }

    Entity::updateFallState(y, onGroundIn, state, pos);
}

void EntityLivingBase::frostWalk(const BlockPos &pos) {
    int32_t i = EnchantmentHelper::getMaxEnchantmentLevel(Enchantments::FROST_WALKER, this);
    if (i > 0) {
        EnchantmentFrostWalker::freezeNearby(this, world, pos, i);
    }
}

void EntityLivingBase::onDeathUpdate() {
    ++deathTime;
    if (deathTime == 20) {
        int32_t k;
        if (!world->isRemote && (isPlayer() || recentlyHit > 0 && canDropLoot() && world
                                                                                   ->getGameRules().getBoolean(
                                                                                       "doMobLoot"))) {
            k = getExperiencePoints(attackingPlayer);

            while (k > 0) {
                int32_t j = EntityXPOrb::getXPSplit(k);
                k -= j;
                world->spawnEntity(new EntityXPOrb(world, posX, posY, posZ, j));
            }
        }

        setDead();

        for (k = 0; k < 20; ++k) {
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

bool EntityLivingBase::canDropLoot() {
    return !isChild();
}

int32_t EntityLivingBase::decreaseAirSupply(int32_t air) {
    int32_t i = EnchantmentHelper::getRespirationModifier(this);
    return i > 0 && rand(i + 1) > 0 ? air : air - 1;
}

int32_t EntityLivingBase::getExperiencePoints(EntityPlayer *player) {
    return 0;
}

bool EntityLivingBase::isPlayer() {
    return false;
}

void EntityLivingBase::playEquipSound(ItemStack stack) {
    if (!stack.isEmpty()) {
        SoundEvent soundevent = SoundEvents::ITEM_ARMOR_EQUIP_GENERIC;
        auto item = stack.getItem();
        if (Util::instanceof<ItemArmor>(item)) {
            soundevent = ((ItemArmor *)item)->getArmorMaterial().getSoundEvent();
        } else if (item == Items::ELYTRA) {
            soundevent = SoundEvents::ITEM_ARMOR_EQIIP_ELYTRA;
        }

        playSound(soundevent, 1.0F, 1.0F);
    }
}

void EntityLivingBase::updatePotionEffects() {
    try {
        for (auto keySet : activePotionsMap) {
            Potion *potion = keySet.first;
            PotionEffect potioneffect = keySet.second;
            if (!potioneffect.onUpdate(this)) {
                if (!world->isRemote) {
                    activePotionsMap.erase();
                    onFinishedPotionEffect(potioneffect);
                }
            } else if (potioneffect.getDuration() % 600 == 0) {
                onChangedPotionEffect(potioneffect, false);
            }
        }
    } catch (ConcurrentModificationException var11) {
    }

    if (potionsNeedUpdate) {
        if (!world->isRemote) {
            updatePotionMetadata();
        }

        potionsNeedUpdate = false;
    }

    int32_t i = dataManager.get(POTION_EFFECTS);
    bool flag1 = dataManager.get(HIDE_PARTICLES);
    if (i > 0) {
        bool flag;
        if (isInvisible()) {
            flag = rand(15) == 0;
        } else {
            flag = nextBoolean(rand);
        }

        if (flag1) {
            flag &= rand(5) == 0;
        }

        if (flag && i > 0) {
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

void EntityLivingBase::updatePotionMetadata() {
    if (activePotionsMap.empty()) {
        resetPotionEffectMetadata();
        setInvisible(false);
    } else {
        auto collection = activePotionsMap;
        dataManager.set(HIDE_PARTICLES, areAllPotionsAmbient(collection));
        dataManager.set(POTION_EFFECTS, PotionUtils::getPotionColorFromEffectList(collection));
        setInvisible(isPotionActive(MobEffects::INVISIBILITY));
    }
}

void EntityLivingBase::resetPotionEffectMetadata() {
    dataManager.set(HIDE_PARTICLES, false);
    dataManager.set(POTION_EFFECTS, 0);
}

void EntityLivingBase::onNewPotionEffect(PotionEffect id) {
    potionsNeedUpdate = true;
    if (!world->isRemote) {
        id.getPotion().applyAttributesModifiersToEntity(this, getAttributeMap(), id.getAmplifier());
    }
}

void EntityLivingBase::onChangedPotionEffect(PotionEffect id, bool p_70695_2_) {
    potionsNeedUpdate = true;
    if (p_70695_2_ && !world->isRemote) {
        Potion potion = id.getPotion();
        potion.removeAttributesModifiersFromEntity(this, getAttributeMap(), id.getAmplifier());
        potion.applyAttributesModifiersToEntity(this, getAttributeMap(), id.getAmplifier());
    }
}

void EntityLivingBase::onFinishedPotionEffect(PotionEffect effect) {
    potionsNeedUpdate = true;
    if (!world->isRemote) {
        effect.getPotion().removeAttributesModifiersFromEntity(this, getAttributeMap(), effect.getAmplifier());
    }
}

void EntityLivingBase::blockUsingShield(EntityLivingBase *p_190629_1_) {
    p_190629_1_->knockBack(this, 0.5F, posX - p_190629_1_->posX, posZ - p_190629_1_->posZ);
}

bool EntityLivingBase::checkTotemDeathProtection(DamageSource::DamageSource p_190628_1_) {
    if (p_190628_1_.canHarmInCreative()) {
        return false;
    } else {
        ItemStack itemstack = ItemStack::EMPTY;
        EnumHand [] var3 = EnumHand::values();
        int var4 = var3.length;

        for (int var5 = 0; var5 < var4; ++var5) {
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
                EntityPlayerMP *entityplayermp = (EntityPlayerMP *)this;
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

DamageSource::DamageSource EntityLivingBase::getLastDamageSource() {
    if (world->getTotalWorldTime() - lastDamageStamp > 40L) {
        lastDamageSource = nullptr;
    }

    return lastDamageSource;
}

void EntityLivingBase::renderBrokenItemStack(const ItemStack &stack) {
    playSound(SoundEvents::ENTITY_ITEM_BREAK, 0.8F, 0.8F + MathHelper::nextFloat(world->rand) * 0.4F);

    for (int i = 0; i < 5; ++i) {
        Vec3d vec3d = Vec3d(((double)MathHelper::nextFloat(rand) - 0.5) * 0.1, MathHelper::random() * 0.1 + 0.1, 0.0);
        vec3d = vec3d.rotatePitch(-rotationPitch * 0.017453292F);
        vec3d = vec3d.rotateYaw(-rotationYaw * 0.017453292F);
        double d0 = (double)(-MathHelper::nextFloat(rand)) * 0.6 - 0.3;
        Vec3d vec3d1 = Vec3d(((double)MathHelper::nextFloat(rand) - 0.5) * 0.3, d0, 0.6);
        vec3d1 = vec3d1.rotatePitch(-rotationPitch * 0.017453292F);
        vec3d1 = vec3d1.rotateYaw(-rotationYaw * 0.017453292F);
        vec3d1 = vec3d1.add(posX, posY + (double)getEyeHeight(), posZ);
        world->spawnParticle(EnumParticleTypes::ITEM_CRACK, vec3d1.getx(), vec3d1.gety(), vec3d1.getz(), vec3d.getx(),
                             vec3d.gety() + 0.05, vec3d.getz(), Item::getIdFromItem(stack.getItem()));
    }
}

void EntityLivingBase::playHurtSound(DamageSource::DamageSource source) {
    SoundEvent soundevent = getHurtSound(source);
    if (soundevent != nullptr) {
        playSound(soundevent, getSoundVolume(), getSoundPitch());
    }
}

void EntityLivingBase::dropLoot(bool wasRecentlyHit, int32_t lootingModifier, DamageSource::DamageSource source) {
    dropFewItems(wasRecentlyHit, lootingModifier);
    dropEquipment(wasRecentlyHit, lootingModifier);
}

void EntityLivingBase::dropEquipment(bool wasRecentlyHit, int32_t lootingModifier) {

}

SoundEvent EntityLivingBase::getHurtSound(DamageSource::DamageSource damageSourceIn) {
    return SoundEvents::ENTITY_GENERIC_HURT;
}

SoundEvent EntityLivingBase::getDeathSound() {
    return SoundEvents::ENTITY_GENERIC_DEATH;
}

SoundEvent EntityLivingBase::getFallSound(int32_t heightIn) {
    return heightIn > 4 ? SoundEvents::ENTITY_GENERIC_BIG_FALL : SoundEvents::ENTITY_GENERIC_SMALL_FALL;
}

void EntityLivingBase::dropFewItems(bool wasRecentlyHit, int32_t lootingModifier) {

}

void EntityLivingBase::damageArmor(float damage) {

}

void EntityLivingBase::damageShield(float damage) {
}

float EntityLivingBase::applyArmorCalculations(DamageSource::DamageSource source, float damage) {
    if (!source.isUnblockable()) {
        damageArmor(damage);
        damage = CombatRules::getDamageAfterAbsorb(damage, (float)getTotalArmorValue(),
                                                   (float)getEntityAttribute(SharedMonsterAttributes::ARMOR_TOUGHNESS).
                                                   getAttributeValue());
    }

    return damage;
}

float EntityLivingBase::applyPotionDamageCalculations(DamageSource::DamageSource source, float damage) {
    if (source.isDamageAbsolute()) {
        return damage;
    } else {
        if (isPotionActive(MobEffects::RESISTANCE) && source != DamageSource::OUT_OF_WORLD) {
            int32_t k = (getActivePotionEffect(MobEffects::RESISTANCE).getAmplifier() + 1) * 5;
            auto j = 25 - k;
            float f = damage * (float)j;
            damage = f / 25.0F;
        }

        if (damage <= 0.0F) {
            return 0.0F;
        } else {
            int32_t k = EnchantmentHelper::getEnchantmentModifierDamage(getArmorInventoryList(), source);
            if (k > 0) {
                damage = CombatRules::getDamageAfterMagicAbsorb(damage, (float)k);
            }

            return damage;
        }
    }
}

void EntityLivingBase::damageEntity(DamageSource::DamageSource damageSrc, float damageAmount) {
    if (!isEntityInvulnerable(damageSrc)) {
        damageAmount = applyArmorCalculations(damageSrc, damageAmount);
        damageAmount = applyPotionDamageCalculations(damageSrc, damageAmount);
        float f = damageAmount;
        damageAmount = MathHelper::max(damageAmount - getAbsorptionAmount(), 0.0F);
        setAbsorptionAmount(getAbsorptionAmount() - (f - damageAmount));
        if (damageAmount != 0.0F) {
            float f1 = getHealth();
            setHealth(f1 - damageAmount);
            getCombatTracker().trackDamage(damageSrc, f1, damageAmount);
            setAbsorptionAmount(getAbsorptionAmount() - damageAmount);
        }
    }
}

void EntityLivingBase::outOfWorld() {
    attackEntityFrom(DamageSource::OUT_OF_WORLD, 4.0F);
}

void EntityLivingBase::updateArmSwingProgress() {
    auto i = getArmSwingAnimationEnd();
    if (isSwingInProgress) {
        ++swingProgressInt;
        if (swingProgressInt >= i) {
            swingProgressInt = 0;
            isSwingInProgress = false;
        }
    } else {
        swingProgressInt = 0;
    }

    swingProgress = (float)swingProgressInt / (float)i;
}

float EntityLivingBase::getSoundVolume() {
    return 1.0F;
}

float EntityLivingBase::getSoundPitch() {
    return isChild()
               ? (MathHelper::nextFloat(rand) - MathHelper::nextFloat(rand)) * 0.2F + 1.5F
               : (MathHelper::nextFloat(rand) - MathHelper::nextFloat(rand)) * 0.2F + 1.0F;
}

bool EntityLivingBase::isMovementBlocked() {
    return getHealth() <= 0.0F;
}

float EntityLivingBase::getJumpUpwardsMotion() {
     return 0.42F;
}

void EntityLivingBase::jump() {
    motionY = (double)getJumpUpwardsMotion();
    if (isPotionActive(MobEffects::JUMP_BOOST)) {
        motionY += (double)((float)(getActivePotionEffect(MobEffects::JUMP_BOOST).getAmplifier() + 1) * 0.1F);
    }

    if (isSprinting()) {
        float f = rotationYaw * 0.017453292F;
        motionX -= (double)(MathHelper::sin(f) * 0.2F);
        motionZ += (double)(MathHelper::cos(f) * 0.2F);
    }

    isAirBorne = true;
}

void EntityLivingBase::handleJumpWater() {
    motionY += 0.03999999910593033;
}

void EntityLivingBase::handleJumpLava() {
    motionY += 0.03999999910593033;
}

float EntityLivingBase::getWaterSlowDown() {
    return 0.8F;
}

float EntityLivingBase::updateDistance(float p_110146_1_, float p_110146_2_) {
    float f = MathHelper::wrapDegrees(p_110146_1_ - renderYawOffset);
    renderYawOffset += f * 0.3F;
    float f1 = MathHelper::wrapDegrees(rotationYaw - renderYawOffset);
    bool flag = f1 < -90.0F || f1 >= 90.0F;
    if (f1 < -75.0F) {
        f1 = -75.0F;
    }

    if (f1 >= 75.0F) {
        f1 = 75.0F;
    }

    renderYawOffset = rotationYaw - f1;
    if (f1 * f1 > 2500.0F) {
        renderYawOffset += f1 * 0.2F;
    }

    if (flag) {
        p_110146_2_ *= -1.0F;
    }

    return p_110146_2_;
}

void EntityLivingBase::updateEntityActionState() {

}

void EntityLivingBase::collideWithNearbyEntities() {
    auto list = world->getEntitiesInAABBexcluding(this, getEntityBoundingBox(), EntitySelectors::getTeamCollisionPredicate(this));
      if (!list.isEmpty()) {
         auto i = world->getGameRules().getInt("maxEntityCramming");
         if (i > 0 && list.size() > i - 1 && rand(4) == 0) {
            auto j = 0;

            for(auto k = 0; k < list.size(); ++k) {
               if (!((Entity)list.get(k)).isRiding()) {
                  ++j;
               }
            }

            if (j > i - 1) {
               attackEntityFrom(DamageSource::CRAMMING, 6.0F);
            }
         }

         for(auto j = 0; j < list.size(); ++j) {
            Entity* entity = (Entity*)list.get(j);
            collideWithEntity(entity);
         }
      }
}

void EntityLivingBase::collideWithEntity(Entity *entityIn) {
    entityIn->applyEntityCollision(this);
}

void EntityLivingBase::markVelocityChanged() {
    velocityChanged = MathHelper::nextDouble(rand) >= getEntityAttribute(SharedMonsterAttributes::KNOCKBACK_RESISTANCE).getAttributeValue();
}

void EntityLivingBase::markPotionsDirty() {
    potionsNeedUpdate = true;
}

void EntityLivingBase::updateActiveHand() {
    if (isHandActive()) {
        ItemStack itemstack = getHeldItem(getActiveHand());
        if (itemstack == activeItemStack) {
            if (getItemInUseCount() <= 25 && getItemInUseCount() % 4 == 0) {
                updateItemUse(activeItemStack, 5);
            }

            if (--activeItemStackUseCount == 0 && !world->isRemote) {
                onItemUseFinish();
            }
        } else {
            resetActiveHand();
        }
    }
}

void EntityLivingBase::updateItemUse(ItemStack stack, int32_t eatingParticleCount) {
    if (!stack.isEmpty() && isHandActive()) {
         if (stack.getItemUseAction() == EnumAction::DRINK) {
            playSound(SoundEvents::ENTITY_GENERIC_DRINK, 0.5F, MathHelper::nextFloat(world->rand) * 0.1F + 0.9F);
         }

         if (stack.getItemUseAction() == EnumAction::EAT) {
            for(auto i = 0; i < eatingParticleCount; ++i) {
               Vec3d vec3d(((double)MathHelper::nextFloat(rand) - 0.5) * 0.1, MathHelper::random() * 0.1 + 0.1, 0.0);
               vec3d = vec3d.rotatePitch(-rotationPitch * 0.017453292F);
               vec3d = vec3d.rotateYaw(-rotationYaw * 0.017453292F);
               double d0 = (double)(-MathHelper::nextFloat(rand)) * 0.6 - 0.3;
               Vec3d vec3d1(((double)MathHelper::nextFloat(rand) - 0.5) * 0.3, d0, 0.6);
               vec3d1 = vec3d1.rotatePitch(-rotationPitch * 0.017453292F);
               vec3d1 = vec3d1.rotateYaw(-rotationYaw * 0.017453292F);
               vec3d1 = vec3d1.add(posX, posY + (double)getEyeHeight(), posZ);
               if (stack.getHasSubtypes()) {
                  world->spawnParticle(EnumParticleTypes::ITEM_CRACK, vec3d1.getx(), vec3d1.gety(), vec3d1.getz(), vec3d.getx(), vec3d.gety() + 0.05, vec3d.getz(), Item::getIdFromItem(stack.getItem()), stack.getMetadata());
               } else {
                  world->spawnParticle(EnumParticleTypes::ITEM_CRACK, vec3d1.getx(), vec3d1.gety(), vec3d1.getz(), vec3d.getx(), vec3d.gety() + 0.05, vec3d.getz(), Item::getIdFromItem(stack.getItem()));
               }
            }

        playSound(SoundEvents::ENTITY_GENERIC_EAT, 0.5F + 0.5F * (float)rand(2), (MathHelper::nextFloat(rand) - MathHelper::nextFloat(rand)) * 0.2F + 1.0F);
         }
    }
}

void EntityLivingBase::onItemUseFinish() {
    if (!activeItemStack.isEmpty() && isHandActive()) {
        updateItemUse(activeItemStack, 16);
        setHeldItem(getActiveHand(), activeItemStack.onItemUseFinish(world, this));
        resetActiveHand();
    }
}

void EntityLivingBase::onDeath(DamageSource::DamageSource cause) {
    if (!dead) {
        Entity *entity = cause.getTrueSource();
        EntityLivingBase *entitylivingbase = getAttackingEntity();
        if (scoreValue >= 0 && entitylivingbase != nullptr) {
            entitylivingbase->awardKillScore(this, scoreValue, cause);
        }

        if (entity != nullptr) {
            entity->onKillEntity(this);
        }

        dead = true;
        getCombatTracker().reset();
        if (!world->isRemote) {
            int i = 0;
            if (Util::instanceof<EntityPlayer>(entity)) {
                i = EnchantmentHelper::getLootingModifier((EntityLivingBase *)entity);
            }

            if (canDropLoot() && world->getGameRules().getBoolean("doMobLoot")) {
                bool flag = recentlyHit > 0;
                dropLoot(flag, i, cause);
            }
        }

        world->setEntityState(this, std::byte{3});
    }
}

void EntityLivingBase::knockBack(Entity *entityIn, float strength, double xRatio, double zRatio) {
    if (MathHelper::nextDouble(rand) >= getEntityAttribute(SharedMonsterAttributes::KNOCKBACK_RESISTANCE).
        getAttributeValue()) {
        isAirBorne = true;
        float f = MathHelper::sqrt(xRatio * xRatio + zRatio * zRatio);
        motionX /= 2.0;
        motionZ /= 2.0;
        motionX -= xRatio / (double)f * (double)strength;
        motionZ -= zRatio / (double)f * (double)strength;
        if (onGround) {
            motionY /= 2.0;
            motionY += (double)strength;
            if (motionY > 0.4000000059604645) {
                motionY = 0.4000000059604645;
            }
        }
    }
}

bool EntityLivingBase::isOnLadder() {
    auto i = MathHelper::floor(posX);
    auto j = MathHelper::floor(getEntityBoundingBox().getminY());
    auto k = MathHelper::floor(posZ);
    if (Util::instanceof<EntityPlayer>(this) && ((EntityPlayer *)this)->isSpectator()) {
        return false;
    } else {
        BlockPos blockpos(i, j, k);
        IBlockState *iblockstate = world->getBlockState(blockpos);
        Block *block = iblockstate->getBlock();
        if (block != Blocks::LADDER && block != Blocks::VINE) {
            return Util::instanceof<BlockTrapDoor>(block) && canGoThroughtTrapDoorOnLadder(blockpos, iblockstate);
        } else {
            return true;
        }
    }
}

bool EntityLivingBase::isEntityAlive() {
    return !isDead && getHealth() > 0.0F;
}

void EntityLivingBase::fall(float distance, float damageMultiplier) {
    Entity::fall(distance, damageMultiplier);
    PotionEffect potioneffect = getActivePotionEffect(MobEffects::JUMP_BOOST);
    float f = potioneffect == nullptr ? 0.0F : (float)(potioneffect.getAmplifier() + 1);
    auto i = MathHelper::ceil((distance - 3.0F - f) * damageMultiplier);
    if (i > 0) {
        playSound(getFallSound(i), 1.0F, 1.0F);
        attackEntityFrom(DamageSource::FALL, (float)i);
        auto j = MathHelper::floor(posX);
        auto k = MathHelper::floor(posY - 0.20000000298023224);
        auto l = MathHelper::floor(posZ);
        IBlockState *iblockstate = world->getBlockState(BlockPos(j, k, l));
        if (iblockstate->getMaterial() != Material::AIR) {
            SoundType soundtype = iblockstate->getBlock()->getSoundType();
            playSound(soundtype.getFallSound(), soundtype.getVolume() * 0.5F, soundtype.getPitch() * 0.75F);
        }
    }
}

void EntityLivingBase::performHurtAnimation() {
    maxHurtTime = 10;
    hurtTime = maxHurtTime;
    attackedAtYaw = 0.0F;
}

int32_t EntityLivingBase::getTotalArmorValue() {
    IAttributeInstance *iattributeinstance = getEntityAttribute(SharedMonsterAttributes::ARMOR);
    return MathHelper::floor(iattributeinstance->getAttributeValue());
}

CombatTracker EntityLivingBase::getCombatTracker() const {
    return combatTracker;
}

EntityLivingBase *EntityLivingBase::getAttackingEntity() {
    if (combatTracker.getBestAttacker() != nullptr) {
        return combatTracker.getBestAttacker();
    } else if (attackingPlayer != nullptr) {
        return attackingPlayer;
    } else {
        return revengeTarget != nullptr ? revengeTarget : nullptr;
    }
}

float EntityLivingBase::getMaxHealth() {
    return (float)getEntityAttribute(SharedMonsterAttributes::MAX_HEALTH).getAttributeValue();
}

int32_t EntityLivingBase::getArrowCountInEntity() {
    return dataManager.get(ARROW_COUNT_IN_ENTITY);
}

void EntityLivingBase::setArrowCountInEntity(int32_t count) {
    dataManager.set(ARROW_COUNT_IN_ENTITY, count);
}

void EntityLivingBase::swingArm(EnumHand hand) {
    if (!isSwingInProgress || swingProgressInt >= getArmSwingAnimationEnd() / 2 || swingProgressInt < 0) {
        swingProgressInt = -1;
        isSwingInProgress = true;
        swingingHand = hand;
        if (Util::instanceof<WorldServer>(world)) {
            ((WorldServer *)world)->getEntityTracker().sendToTracking(
                this, new SPacketAnimation(this, hand == EnumHand::MAIN_HAND ? 0 : 3));
        }
    }
}

void EntityLivingBase::handleStatusUpdate(std::byte id) {
    bool flag = id == std::byte{33};
    bool flag1 = id == std::byte{36};
    bool flag2 = id == std::byte{37};
    if (id != std::byte{2} && !flag && !flag1 && !flag2) {
        if (id == std::byte{3}) {
            SoundEvent soundevent1 = getDeathSound();
            if (soundevent1 != nullptr) {
                playSound(soundevent1, getSoundVolume(),
                          (MathHelper::nextFloat(rand) - MathHelper::nextFloat(rand)) * 0.2F + 1.0F);
            }

            setHealth(0.0F);
            onDeath(DamageSource::GENERIC);
        } else if (id == std::byte{30}) {
            playSound(SoundEvents::ITEM_SHIELD_BREAK, 0.8F, 0.8F + MathHelper::nextFloat(world->rand) * 0.4F);
        } else if (id == std::byte{29}) {
            playSound(SoundEvents::ITEM_SHIELD_BLOCK, 1.0F, 0.8F + MathHelper::nextFloat(world->rand) * 0.4F);
        } else {
            Entity::handleStatusUpdate(id);
        }
    } else {
        limbSwingAmount = 1.5F;
        hurtResistantTime = maxHurtResistantTime;
        maxHurtTime = 10;
        hurtTime = maxHurtTime;
        attackedAtYaw = 0.0F;
        if (flag) {
            playSound(SoundEvents::ENCHANT_THORNS_HIT, getSoundVolume(),
                      (MathHelper::nextFloat(rand) - MathHelper::nextFloat(rand)) * 0.2F + 1.0F);
        }

        DamageSource::DamageSource damagesource;
        if (flag2) {
            damagesource = DamageSource::ON_FIRE;
        } else if (flag1) {
            damagesource = DamageSource::DROWN;
        } else {
            damagesource = DamageSource::GENERIC;
        }

        SoundEvent soundevent = getHurtSound(damagesource);
        if (soundevent != nullptr) {
            playSound(soundevent, getSoundVolume(),
                      (MathHelper::nextFloat(rand) - MathHelper::nextFloat(rand)) * 0.2F + 1.0F);
        }

        attackEntityFrom(DamageSource::GENERIC, 0.0F);
    }
}

AbstractAttributeMap *EntityLivingBase::getAttributeMap() {
    if (attributeMap == nullptr) {
        attributeMap = new AttributeMap();
    }

    return attributeMap;
}

EnumCreatureAttribute EntityLivingBase::getCreatureAttribute() {
    return EnumCreatureAttribute::UNDEFINED;
}

ItemStack EntityLivingBase::getHeldItemMainhand() {
    return getItemStackFromSlot(EntityEquipmentSlot::MAINHAND);
}

ItemStack EntityLivingBase::getHeldItemOffhand() {
    return getItemStackFromSlot(EntityEquipmentSlot::OFFHAND);
}

ItemStack EntityLivingBase::getHeldItem(EnumHand hand) {
    if (hand == EnumHand::MAIN_HAND) {
        return getItemStackFromSlot(EntityEquipmentSlot::MAINHAND);
    } else if (hand == EnumHand::OFF_HAND) {
        return getItemStackFromSlot(EntityEquipmentSlot::OFFHAND);
    } else {
        throw std::logic_error("Invalid hand " + std::to_string(static_cast<int32_t>(hand)));
    }
}

void EntityLivingBase::setHeldItem(EnumHand hand, ItemStack stack) {
    if (hand == EnumHand::MAIN_HAND) {
        setItemStackToSlot(EntityEquipmentSlot::MAINHAND, stack);
    } else {
        if (hand != EnumHand::OFF_HAND) {
            throw std::logic_error("Invalid hand " + std::to_string(static_cast<int32_t>(hand)));
        }

        setItemStackToSlot(EntityEquipmentSlot::OFFHAND, stack);
    }
}

bool EntityLivingBase::hasItemInSlot(EntityEquipmentSlot p_190630_1_) {
    return !getItemStackFromSlot(p_190630_1_).isEmpty();
}

void EntityLivingBase::setSprinting(bool sprinting) {
    Entity::setSprinting(sprinting);
    IAttributeInstance *iattributeinstance = getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED);
    if (iattributeinstance->getModifier(SPRINTING_SPEED_BOOST_ID) != nullptr) {
        iattributeinstance->removeModifier(SPRINTING_SPEED_BOOST);
    }

    if (sprinting) {
        iattributeinstance->applyModifier(SPRINTING_SPEED_BOOST);
    }
}

void EntityLivingBase::dismountEntity(Entity *entityIn) {
    double d1;
    double d4;
    if (!(Util::instanceof<EntityBoat>(entityIn)) && !(Util::instanceof<AbstractHorse>(entityIn)))
    {
        d1 = entityIn->posX;
        double d13 = entityIn->getEntityBoundingBox().getminY() + (double)entityIn->height;
        double d14 = entityIn->posZ;
        EnumFacing enumfacing1 = entityIn->getAdjustedHorizontalFacing();
        if (enumfacing1 != nullptr) {
            EnumFacing enumfacing = enumfacing1.rotateY();
            std::array<std::array<int32_t,2>,9> aint1{{{0, 1}, {0, -1}, {-1, 1}, {-1, -1}, {1, 1}, {1, -1}, {-1, 0}, {1, 0}, {0, 1}}};
            d4 = MathHelper::floor(posX) + 0.5;
            double d6 = MathHelper::floor(posZ) + 0.5;
            double d7 = getEntityBoundingBox().getmaxX() - getEntityBoundingBox().getminX();
            double d8 = getEntityBoundingBox().getmaxZ() - getEntityBoundingBox().getminZ();
            AxisAlignedBB axisalignedbb(d4 - d7 / 2.0, entityIn->getEntityBoundingBox().getminY(),
                                        d6 - d8 / 2.0, d4 + d7 / 2.0,
                                        MathHelper::floor(entityIn->getEntityBoundingBox().getminY()) + (double)
                                        height, d6 + d8 / 2.0);

            for(auto aint : aint1){
                double d9 = (double)(enumfacing1.getXOffset() * aint[0] + enumfacing.getXOffset() * aint[1]);
                double d10 = (double)(enumfacing1.getZOffset() * aint[0] + enumfacing.getZOffset() * aint[1]);
                double d11 = d4 + d9;
                double d12 = d6 + d10;
                AxisAlignedBB axisalignedbb1 = axisalignedbb.offset(d9, 0.0, d10);
                if (!world->collidesWithAnyBlock(axisalignedbb1)) {
                    if (world->getBlockState(BlockPos(d11, posY, d12))->isTopSolid()) {
                        setPositionAndUpdate(d11, posY + 1.0, d12);
                        return;
                    }

                    BlockPos blockpos(d11, posY - 1.0, d12);
                    if (world->getBlockState(blockpos)->isTopSolid() || world->getBlockState(blockpos)->getMaterial() == Material::WATER) {
                        d1 = d11;
                        d13 = posY + 1.0;
                        d14 = d12;
                    }
                }
                else if (!world->collidesWithAnyBlock(axisalignedbb1.offset(0.0, 1.0, 0.0)) && world->getBlockState(BlockPos(d11,posY +1.0,d12))->isTopSolid()) {
                    d1 = d11;
                    d13 = posY + 2.0;
                    d14 = d12;
                }
            }
        }

        setPositionAndUpdate(d1, d13, d14);
    }
    else
    {
        d1 = (double)(width / 2.0F + entityIn->width / 2.0F) + 0.4;
        float f;
        if (Util::instanceof<EntityBoat>(entityIn)) {
            f = 0.0F;
        } else {
            f = 1.5707964F * (float)(getPrimaryHand() == EnumHandSide::RIGHT ? -1 : 1);
        }

        float f1 = -MathHelper::sin(-rotationYaw * 0.017453292F - 3.1415927F + f);
        float f2 = -MathHelper::cos(-rotationYaw * 0.017453292F - 3.1415927F + f);
        double d2 = MathHelper::abs(f1) > MathHelper::abs(f2) ? d1 / (double)MathHelper::abs(f1) : d1 / (double)MathHelper::abs(f2);
        double d3 = posX + (double)f1 * d2;
        d4 = posZ + (double)f2 * d2;
        setPosition(d3, entityIn->posY + (double)entityIn->height + 0.001, d4);
        if (world->collidesWithAnyBlock(getEntityBoundingBox())) {
            setPosition(d3, entityIn->posY + (double)entityIn->height + 1.001, d4);
            if (world->collidesWithAnyBlock(getEntityBoundingBox())) {
                setPosition(entityIn->posX, entityIn->posY + (double)height + 0.001, entityIn->posZ);
            }
        }
    }
}

bool EntityLivingBase::getAlwaysRenderNameTagForRender()
{
    return getAlwaysRenderNameTag();
}

void EntityLivingBase::travel(float strafe, float vertical, float forward) {
    double d0;
      double d6;
      double d8;
      if (isServerWorld() || canPassengerSteer()) {
         float f8;
         float f7;
         float f9;
         if (isInWater() && (!(Util::instanceof<EntityPlayer>(this)) || !((EntityPlayer*)this)->capabilities.isFlying)) {
            d0 = posY;
            f7 = getWaterSlowDown();
            f8 = 0.02F;
            f9 = (float)EnchantmentHelper::getDepthStriderModifier(this);
            if (f9 > 3.0F) {
               f9 = 3.0F;
            }

            if (!onGround) {
               f9 *= 0.5F;
            }

            if (f9 > 0.0F) {
               f7 += (0.54600006F - f7) * f9 / 3.0F;
               f8 += (getAIMoveSpeed() - f8) * f9 / 3.0F;
            }

            moveRelative(strafe, vertical, forward, f8);
            move(MoverType::SELF, motionX, motionY, motionZ);
            motionX *= (double)f7;
            motionY *= 0.800000011920929;
            motionZ *= (double)f7;
            if (!hasNoGravity()) {
               motionY -= 0.02;
            }

            if (collidedHorizontally && isOffsetPositionInLiquid(motionX, motionY + 0.6000000238418579 - posY + d0, motionZ)) {
               motionY = 0.30000001192092896;
            }
         } else if (!isInLava() || Util::instanceof<EntityPlayer>(this) && ((EntityPlayer*)this)->capabilities.isFlying) {
            if (isElytraFlying()) {
               if (motionY > -0.5) {
                  fallDistance = 1.0F;
               }

               Vec3d vec3d = getLookVec();
               float f = rotationPitch * 0.017453292F;
               d6 = MathHelper::sqrt(vec3d.getx() * vec3d.getx() + vec3d.getz() * vec3d.getz());
               d8 = MathHelper::sqrt(motionX * motionX + motionZ * motionZ);
               double d1 = vec3d.length();
               float f4 = MathHelper::cos(f);
               f4 = (float)((double)f4 * (double)f4 * MathHelper::min(1.0, d1 / 0.4));
               motionY += -0.08 + (double)f4 * 0.06;
               double d11;
               if (motionY < 0.0 && d6 > 0.0) {
                  d11 = motionY * -0.1 * (double)f4;
                  motionY += d11;
                  motionX += vec3d.getx() * d11 / d6;
                  motionZ += vec3d.getz() * d11 / d6;
               }

               if (f < 0.0F) {
                  d11 = d8 * (double)(-MathHelper::sin(f)) * 0.04;
                  motionY += d11 * 3.2;
                  motionX -= vec3d.getx() * d11 / d6;
                  motionZ -= vec3d.getz() * d11 / d6;
               }

               if (d6 > 0.0) {
                  motionX += (vec3d.getx() / d6 * d8 - motionX) * 0.1;
                  motionZ += (vec3d.getz() / d6 * d8 - motionZ) * 0.1;
               }

               motionX *= 0.9900000095367432;
               motionY *= 0.9800000190734863;
               motionZ *= 0.9900000095367432;
               move(MoverType::SELF, motionX, motionY, motionZ);
               if (collidedHorizontally && !world->isRemote) {
                  d11 = MathHelper::sqrt(motionX * motionX + motionZ * motionZ);
                  double d3 = d8 - d11;
                  float f5 = (float)(d3 * 10.0 - 3.0);
                  if (f5 > 0.0F) {
                     playSound(getFallSound((int)f5), 1.0F, 1.0F);
                     attackEntityFrom(DamageSource::FLY_INTO_WALL, f5);
                  }
               }

               if (onGround && !world->isRemote) {
                  setFlag(7, false);
               }
            } else {
               float f6 = 0.91F;
               PooledMutableBlockPos blockpos$pooledmutableblockpos = PooledMutableBlockPos::retain(posX, getEntityBoundingBox().getminY() - 1.0, posZ);
               if (onGround) {
                  f6 = world->getBlockState(blockpos$pooledmutableblockpos)->getBlock()->slipperiness * 0.91F;
               }

               f7 = 0.16277136F / (f6 * f6 * f6);
               if (onGround) {
                  f8 = getAIMoveSpeed() * f7;
               } else {
                  f8 = jumpMovementFactor;
               }

               moveRelative(strafe, vertical, forward, f8);
               f6 = 0.91F;
               if (onGround) {
                  f6 = world->getBlockState(blockpos$pooledmutableblockpos.setPos(posX, getEntityBoundingBox().getminY() - 1.0, posZ))->getBlock()->slipperiness * 0.91F;
               }

               if (isOnLadder()) {
                  f9 = 0.15F;
                  motionX = MathHelper::clamp(motionX, -0.15000000596046448, 0.15000000596046448);
                  motionZ = MathHelper::clamp(motionZ, -0.15000000596046448, 0.15000000596046448);
                  fallDistance = 0.0F;
                  if (motionY < -0.15) {
                     motionY = -0.15;
                  }

                  bool flag = isSneaking() && Util::instanceof<EntityPlayer>(this);
                  if (flag && motionY < 0.0) {
                     motionY = 0.0;
                  }
               }

               move(MoverType::SELF, motionX, motionY, motionZ);
               if (collidedHorizontally && isOnLadder()) {
                  motionY = 0.2;
               }

               if (isPotionActive(MobEffects::LEVITATION)) {
                  motionY += (0.05 * (double)(getActivePotionEffect(MobEffects::LEVITATION).getAmplifier() + 1) - motionY) * 0.2;
               } else {
                  blockpos$pooledmutableblockpos.setPos(posX, 0.0, posZ);
                  if (!world->isRemote || world->isBlockLoaded(blockpos$pooledmutableblockpos) && world->getChunk(blockpos$pooledmutableblockpos).isLoaded()) {
                     if (!hasNoGravity()) {
                        motionY -= 0.08;
                     }
                  } else if (posY > 0.0) {
                     motionY = -0.1;
                  } else {
                     motionY = 0.0;
                  }
               }

               motionY *= 0.9800000190734863;
               motionX *= (double)f6;
               motionZ *= (double)f6;
               blockpos$pooledmutableblockpos.release();
            }
         } else {
            d0 = posY;
            moveRelative(strafe, vertical, forward, 0.02F);
            move(MoverType::SELF, motionX, motionY, motionZ);
            motionX *= 0.5;
            motionY *= 0.5;
            motionZ *= 0.5;
            if (!hasNoGravity()) {
               motionY -= 0.02;
            }

            if (collidedHorizontally && isOffsetPositionInLiquid(motionX, motionY + 0.6000000238418579 - posY + d0, motionZ)) {
               motionY = 0.30000001192092896;
            }
         }
      }

      prevLimbSwingAmount = limbSwingAmount;
      d0 = posX - prevPosX;
      d6 = posZ - prevPosZ;
      d8 = Util::instanceof<EntityFlying>(this) ? posY - prevPosY : 0.0;
      float f10 = MathHelper::sqrt(d0 * d0 + d8 * d8 + d6 * d6) * 4.0F;
      if (f10 > 1.0F) {
         f10 = 1.0F;
      }

      limbSwingAmount += (f10 - limbSwingAmount) * 0.4F;
      limbSwing += limbSwingAmount;
}

float EntityLivingBase::getAIMoveSpeed() const {
    return landMovementFactor;
}

void EntityLivingBase::setAIMoveSpeed(float speedIn) {
    landMovementFactor = speedIn;
}

bool EntityLivingBase::attackEntityAsMob(Entity *entityIn) {
    setLastAttackedEntity(entityIn);
    return false;
}

bool EntityLivingBase::isPlayerSleeping() {
    return false;
}

void EntityLivingBase::onUpdate() {
    Entity::onUpdate();
      updateActiveHand();
      if (!world->isRemote) {
         auto i = getArrowCountInEntity();
         if (i > 0) {
            if (arrowHitTimer <= 0) {
               arrowHitTimer = 20 * (30 - i);
            }

            --arrowHitTimer;
            if (arrowHitTimer <= 0) {
               setArrowCountInEntity(i - 1);
            }
         }

         auto var2 = EntityEquipmentSlot::values();

         for(auto entityequipmentslot : var2) {
            ItemStack itemstack = ItemStack::EMPTY;
            switch(entityequipmentslot->getSlotType()) {
            case EquipmentType::HAND:
               itemstack = handInventory[entityequipmentslot->getIndex()];
               break;
            case EquipmentType::ARMOR:
               itemstack = armorArray[entityequipmentslot->getIndex()];
               break;
            default:
               continue;
            }

            ItemStack itemstack1 = getItemStackFromSlot(entityequipmentslot);
            if (!ItemStack::areItemStacksEqual(itemstack1, itemstack)) {
               ((WorldServer*)world).getEntityTracker().sendToTracking(this, new SPacketEntityEquipment(getEntityId(), entityequipmentslot, itemstack1));
               if (!itemstack.isEmpty()) {
                  getAttributeMap()->removeAttributeModifiers(itemstack.getAttributeModifiers(entityequipmentslot));
               }

               if (!itemstack1.isEmpty()) {
                  getAttributeMap()->applyAttributeModifiers(itemstack1.getAttributeModifiers(entityequipmentslot));
               }

               switch(entityequipmentslot->getSlotType()) {
               case EquipmentType::HAND:
                  handInventory.emplace_back(entityequipmentslot->getIndex(), itemstack1.isEmpty() ? ItemStack::EMPTY : itemstack1.copy());
                  break;
               case EquipmentType::ARMOR:
                  armorArray.emplace_back(entityequipmentslot->getIndex(), itemstack1.isEmpty() ? ItemStack::EMPTY : itemstack1.copy());
               }
            }
         }

         if (ticksExisted % 20 == 0) {
            getCombatTracker().reset();
         }

         if (!glowing) {
            bool flag = isPotionActive(MobEffects::GLOWING);
            if (getFlag(6) != flag) {
               setFlag(6, flag);
            }
         }
      }

      onLivingUpdate();
      double d0 = posX - prevPosX;
      double d1 = posZ - prevPosZ;
      float f3 = (float)(d0 * d0 + d1 * d1);
      float f4 = renderYawOffset;
      float f5 = 0.0F;
      prevOnGroundSpeedFactor = onGroundSpeedFactor;
      float f = 0.0F;
      if (f3 > 0.0025000002F) {
         f = 1.0F;
         f5 = (float)MathHelper::sqrt((double)f3) * 3.0F;
         float f1 = (float)MathHelper::atan2(d1, d0) * 57.295776F - 90.0F;
         float f2 = MathHelper::abs(MathHelper::wrapDegrees(rotationYaw) - f1);
         if (95.0F < f2 && f2 < 265.0F) {
            f4 = f1 - 180.0F;
         } else {
            f4 = f1;
         }
      }

      if (swingProgress > 0.0F) {
         f4 = rotationYaw;
      }

      if (!onGround) {
         f = 0.0F;
      }

      onGroundSpeedFactor += (f - onGroundSpeedFactor) * 0.3F;
      world->profiler.startSection("headTurn");
      f5 = updateDistance(f4, f5);
      world->profiler.endSection();
      world->profiler.startSection("rangeChecks");

      while(rotationYaw - prevRotationYaw < -180.0F) {
         prevRotationYaw -= 360.0F;
      }

      while(rotationYaw - prevRotationYaw >= 180.0F) {
         prevRotationYaw += 360.0F;
      }

      while(renderYawOffset - prevRenderYawOffset < -180.0F) {
         prevRenderYawOffset -= 360.0F;
      }

      while(renderYawOffset - prevRenderYawOffset >= 180.0F) {
         prevRenderYawOffset += 360.0F;
      }

      while(rotationPitch - prevRotationPitch < -180.0F) {
         prevRotationPitch -= 360.0F;
      }

      while(rotationPitch - prevRotationPitch >= 180.0F) {
         prevRotationPitch += 360.0F;
      }

      while(rotationYawHead - prevRotationYawHead < -180.0F) {
         prevRotationYawHead -= 360.0F;
      }

      while(rotationYawHead - prevRotationYawHead >= 180.0F) {
         prevRotationYawHead += 360.0F;
      }

      world->profiler.endSection();
      movedDistance += f5;
      if (isElytraFlying()) {
         ++ticksElytraFlying;
      } else {
         ticksElytraFlying = 0;
      }
}

void EntityLivingBase::onLivingUpdate() {
    if (jumpTicks > 0) {
        --jumpTicks;
    }

    if (newPosRotationIncrements > 0 && !canPassengerSteer()) {
        double d0 = posX + (interpTargetX - posX) / (double)newPosRotationIncrements;
        double d1 = posY + (interpTargetY - posY) / (double)newPosRotationIncrements;
        double d2 = posZ + (interpTargetZ - posZ) / (double)newPosRotationIncrements;
        double d3 = MathHelper::wrapDegrees(interpTargetYaw - (double)rotationYaw);
        rotationYaw = (float)((double)rotationYaw + d3 / (double)newPosRotationIncrements);
        rotationPitch = (float)((double)rotationPitch + (interpTargetPitch - (double)rotationPitch) / (double)newPosRotationIncrements);
        --newPosRotationIncrements;
        setPosition(d0, d1, d2);
        setRotation(rotationYaw, rotationPitch);
    } else if (!isServerWorld()) {
        motionX *= 0.98;
        motionY *= 0.98;
        motionZ *= 0.98;
    }

    if (MathHelper::abs(motionX) < 0.003) {
        motionX = 0.0;
    }

    if (MathHelper::abs(motionY) < 0.003) {
        motionY = 0.0;
    }

    if (MathHelper::abs(motionZ) < 0.003) {
        motionZ = 0.0;
    }

    world->profiler.startSection("ai");
    if (isMovementBlocked()) {
        isJumping = false;
        moveStrafing = 0.0F;
        moveForward = 0.0F;
        randomYawVelocity = 0.0F;
    } else if (isServerWorld()) {
        world->profiler.startSection("newAi");
        updateEntityActionState();
        world->profiler.endSection();
    }

    world->profiler.endSection();
    world->profiler.startSection("jump");
    if (isJumping) {
        if (isInWater()) {
            handleJumpWater();
        } else if (isInLava()) {
            handleJumpLava();
        } else if (onGround && jumpTicks == 0) {
            jump();
            jumpTicks = 10;
        }
    } else {
        jumpTicks = 0;
    }

    world->profiler.endSection();
    world->profiler.startSection("travel");
    moveStrafing *= 0.98F;
    moveForward *= 0.98F;
    randomYawVelocity *= 0.9F;
    updateElytra();
    travel(moveStrafing, moveVertical, moveForward);
    world->profiler.endSection();
    world->profiler.startSection("push");
    collideWithNearbyEntities();
    world->profiler.endSection();
}

void EntityLivingBase::dismountRidingEntity() {
    Entity* entity = getRidingEntity();
    Entity::dismountRidingEntity();
    if (entity != nullptr && entity != getRidingEntity() && !world->isRemote) {
        dismountEntity(entity);
    }
}

void EntityLivingBase::updateRidden() {
    Entity::updateRidden();
    prevOnGroundSpeedFactor = onGroundSpeedFactor;
    onGroundSpeedFactor = 0.0F;
    fallDistance = 0.0F;
}

void EntityLivingBase::setPositionAndRotationDirect(double x, double y, double z, float yaw, float pitch,
    int32_t posRotationIncrements, bool teleport) {
    interpTargetX = x;
    interpTargetY = y;
    interpTargetZ = z;
    interpTargetYaw = (double)yaw;
    interpTargetPitch = (double)pitch;
    newPosRotationIncrements = posRotationIncrements;
}

void EntityLivingBase::setJumping(bool jumping) {
    isJumping = jumping;
}

void EntityLivingBase::onItemPickup(Entity *entityIn, int32_t quantity) {
    if (!entityIn->isDead && !world->isRemote) {
        EntityTracker* entitytracker = ((WorldServer*)world)->getEntityTracker();
        if (Util::instanceof<EntityItem>(entityIn) || Util::instanceof<EntityArrow>(entityIn) || Util::instanceof<EntityXPOrb>(entityIn)) {
            entitytracker->sendToTracking(entityIn, new SPacketCollectItem(entityIn->getEntityId(), getEntityId(), quantity));
        }
    }
}

bool EntityLivingBase::canEntityBeSeen(Entity *entityIn) {
    return world->rayTraceBlocks(Vec3d(posX, posY + (double)getEyeHeight(), posZ), Vec3d(entityIn->posX, entityIn->posY + (double)entityIn->getEyeHeight(), entityIn->posZ), false, true, false) == nullptr;
}

Vec3d EntityLivingBase::getLook(float partialTicks) {
    if (partialTicks == 1.0F) {
        return getVectorForRotation(rotationPitch, rotationYawHead);
    }
    else {
        float f = prevRotationPitch + (rotationPitch - prevRotationPitch) * partialTicks;
        float f1 = prevRotationYawHead + (rotationYawHead - prevRotationYawHead) * partialTicks;
        return getVectorForRotation(f, f1);
    }
}

float EntityLivingBase::getSwingProgress(float partialTickTime) const {
    float f = swingProgress - prevSwingProgress;
    if (f < 0.0F) {
        ++f;
    }

    return prevSwingProgress + f * partialTickTime;
}

bool EntityLivingBase::isServerWorld() const {
    return !world->isRemote;
}

bool EntityLivingBase::canBeCollidedWith() {
    return !isDead;
}

bool EntityLivingBase::canBePushed() {
    return isEntityAlive() && !isOnLadder();
}

float EntityLivingBase::getRotationYawHead() {
    return rotationYawHead;
}

void EntityLivingBase::setRotationYawHead(float rotation) {
    rotationYawHead = rotation;
}

void EntityLivingBase::setRenderYawOffset(float offset) {
    renderYawOffset = offset;
}

float EntityLivingBase::getAbsorptionAmount() const {
    return absorptionAmount;
}

void EntityLivingBase::setAbsorptionAmount(float amount) {
    if (amount < 0.0F) {
        amount = 0.0F;
    }

    absorptionAmount = amount;
}

void EntityLivingBase::sendEnterCombat() {

}

void EntityLivingBase::sendEndCombat() {

}

bool EntityLivingBase::isHandActive() {
    return (dataManager.get(HAND_STATES) & 1) > 0;
}

EnumHand EntityLivingBase::getActiveHand() {
    return (dataManager.get(HAND_STATES) & 2) > 0 ? EnumHand::OFF_HAND : EnumHand::MAIN_HAND;
}

void EntityLivingBase::setActiveHand(EnumHand hand) {
    ItemStack itemstack = getHeldItem(hand);
    if (!itemstack.isEmpty() && !isHandActive()) {
        activeItemStack = itemstack;
        activeItemStackUseCount = itemstack.getMaxItemUseDuration();
        if (!world->isRemote) {
            auto i = 1;
            if (hand == EnumHand::OFF_HAND) {
                i |= 2;
            }

        dataManager.set(HAND_STATES, (std::byte)i);
        }
    }
}

void EntityLivingBase::notifyDataManagerChange(DataParameter key) {
    Entity::notifyDataManagerChange(key);
    if (HAND_STATES.equals(key) && world->isRemote) {
        if (isHandActive() && activeItemStack.isEmpty()) {
            activeItemStack = getHeldItem(getActiveHand());
            if (!activeItemStack.isEmpty()) {
                activeItemStackUseCount = activeItemStack.getMaxItemUseDuration();
            }
        } else if (!isHandActive() && !activeItemStack.isEmpty()) {
            activeItemStack = ItemStack::EMPTY;
            activeItemStackUseCount = 0;
        }
    }
}

ItemStack EntityLivingBase::getActiveItemStack() const {
    return activeItemStack;
}

int32_t EntityLivingBase::getItemInUseCount() const {
    return activeItemStackUseCount;
}

int32_t EntityLivingBase::getItemInUseMaxCount() {
    return isHandActive() ? activeItemStack.getMaxItemUseDuration() - getItemInUseCount() : 0;
}

void EntityLivingBase::stopActiveHand() {
    if (!activeItemStack.isEmpty()) {
        activeItemStack.onPlayerStoppedUsing(world, this, getItemInUseCount());
    }

    resetActiveHand();
}

void EntityLivingBase::resetActiveHand() {
    if (!world->isRemote) {
        dataManager.set(HAND_STATES, 0);
    }

    activeItemStack = ItemStack::EMPTY;
    activeItemStackUseCount = 0;
}

bool EntityLivingBase::isActiveItemStackBlocking() {
    if (isHandActive() && !activeItemStack.isEmpty()) {
        Item* item = activeItemStack.getItem();
        if (item->getItemUseAction(activeItemStack) == EnumAction::BLOCK) {
            return false;
        } else {
            return item->getMaxItemUseDuration(activeItemStack) - activeItemStackUseCount >= 5;
        }
    } else {
        return false;
    }
}

bool EntityLivingBase::isElytraFlying() {
    return getFlag(7);
}

int32_t EntityLivingBase::getTicksElytraFlying() const {
    return ticksElytraFlying;
}

bool EntityLivingBase::attemptTeleport(double x, double y, double z) {
    double d0 = posX;
    double d1 = posY;
    double d2 = posZ;
    posX = x;
    posY = y;
    posZ = z;
    bool flag = false;
    BlockPos blockpos(this);
    World* world = world;
    auto random = getRNG();
    boolean flag1;
    if (world->isBlockLoaded(blockpos)) {
        flag1 = false;

        while(!flag1 && blockpos.gety() > 0) {
            BlockPos blockpos1 = blockpos.down();
            IBlockState* iblockstate = world->getBlockState(blockpos1);
            if (iblockstate->getMaterial()->blocksMovement()) {
               flag1 = true;
            } else {
               --posY;
               blockpos = blockpos1;
            }
        }

        if (flag1) {
            setPositionAndUpdate(posX, posY, posZ);
            if (world->getCollisionBoxes(this, getEntityBoundingBox()).isEmpty() && !world->containsAnyLiquid(getEntityBoundingBox())) {
                flag = true;
            }
        }
    }

    if (!flag) {
        setPositionAndUpdate(d0, d1, d2);
        return false;
    } else {
        flag1 = true;

        for(auto j = 0; j < 128; ++j) {
            double d6 = (double)j / 127.0;
            float f = (MathHelper::nextFloat(random) - 0.5F) * 0.2F;
            float f1 = (MathHelper::nextFloat(random) - 0.5F) * 0.2F;
            float f2 = (MathHelper::nextFloat(random) - 0.5F) * 0.2F;
            double d3 = d0 + (posX - d0) * d6 + (MathHelper::nextDouble(random) - 0.5) * (double)width * 2.0;
            double d4 = d1 + (posY - d1) * d6 + MathHelper::nextDouble(random) * (double)height;
            double d5 = d2 + (posZ - d2) * d6 + (MathHelper::nextDouble(random) - 0.5) * (double)width * 2.0;
            world->spawnParticle(EnumParticleTypes::PORTAL, d3, d4, d5, (double)f, (double)f1, (double)f2);
        }

        if (Util::instanceof<EntityCreature>(this)) {
            ((EntityCreature*)this)->getNavigator().clearPath();
        }

        return true;
    }
}

bool EntityLivingBase::canBeHitWithPotion() {
    return true;
}

bool EntityLivingBase::attackable() {
    return true;
}

void EntityLivingBase::setPartying(BlockPos pos, bool isPartying) {

}

bool EntityLivingBase::canBlockDamageSource(DamageSource::DamageSource damageSourceIn) {
    if (!damageSourceIn.isUnblockable() && isActiveItemStackBlocking()) {
        auto vec3d = damageSourceIn.getDamageLocation();
        if (vec3d) {
            Vec3d vec3d1 = getLook(1.0F);
            Vec3d vec3d2 = vec3d->subtractReverse(Vec3d(posX, posY, posZ)).normalize();
            vec3d2 = Vec3d(vec3d2.getx(), 0.0, vec3d2.getz());
            if (vec3d2.dotProduct(vec3d1) < 0.0) {
                return true;
            }
        }
    }

    return false;
}

bool EntityLivingBase::canGoThroughtTrapDoorOnLadder(BlockPos pos, IBlockState *state) {
    if (state->getValue(BlockTrapDoor::OPEN)) {
        IBlockState *iblockstate = world->getBlockState(pos.down());
        if (iblockstate->getBlock() == Blocks::LADDER && iblockstate->getValue(BlockLadder::FACING) == state->getValue(
                BlockTrapDoor::FACING)) {
            return true;
        }
    }

    return false;
}

int32_t EntityLivingBase::getArmSwingAnimationEnd() {
    if (isPotionActive(MobEffects::HASTE)) {
        return 6 - (1 + getActivePotionEffect(MobEffects::HASTE).getAmplifier());
    } else {
        return isPotionActive(MobEffects::MINING_FATIGUE)
                   ? 6 + (1 + getActivePotionEffect(MobEffects::MINING_FATIGUE).getAmplifier()) * 2
                   : 6;
    }
}

void EntityLivingBase::updateElytra() {
    bool flag = getFlag(7);
    if (flag && !onGround && !isRiding()) {
        ItemStack itemstack = getItemStackFromSlot(EntityEquipmentSlot::CHEST);
        if (itemstack.getItem() == Items::ELYTRA && ItemElytra::isUsable(itemstack)) {
            flag = true;
        if (!world->isRemote && (ticksElytraFlying + 1) % 20 == 0) {
           itemstack.damageItem(1, this);
        }
    } else {
        flag = false;
    }
    } else {
        flag = false;
    }

    if (!world->isRemote) {
        setFlag(7, flag);
    }
}
