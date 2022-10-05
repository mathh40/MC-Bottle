#include "EntityCreeper.h"

#include "../EntityAreaEffectCloud.h"
#include "../SharedMonsterAttributes.h"
#include "../ai/EntityAIAttackMelee.h"
#include "../ai/EntityAIAvoidEntity.h"
#include "../ai/EntityAICreeperSwell.h"
#include "../ai/EntityAIHurtByTarget.h"
#include "../ai/EntityAILookIdle.h"
#include "../ai/EntityAINearestAttackableTarget.h"
#include "../ai/EntityAIOcelotSit.h"
#include "../ai/EntityAISkeletonRiders.h"
#include "../ai/EntityAISwimming.h"
#include "../ai/EntityAIWanderAvoidWater.h"
#include "../ai/EntityAIWatchClosest.h"
#include "../ai/attributes/IAttributeInstance.h"
#include "datafix/DataFixer.h"

DataParameter EntityCreeper::STATE   = EntityDataManager.createKey(EntityCreeper.class, DataSerializers::VARINT);
DataParameter EntityCreeper::POWERED = EntityDataManager.createKey(EntityCreeper.class, DataSerializers::BOOLEAN);
DataParameter EntityCreeper::IGNITED = EntityDataManager.createKey(EntityCreeper.class, DataSerializers::BOOLEAN);

EntityCreeper::EntityCreeper(World *worldIn) : EntityMob(worldIn)
{
    setSize(0.6F, 1.7F);
}

int32_t EntityCreeper::getMaxFallHeight()
{
    return getAttackTarget() == nullptr ? 3 : 3 + (getHealth() - 1.0F);
}

void EntityCreeper::fall(float distance, float damageMultiplier)
{
    EntityMob::fall(distance, damageMultiplier);
    timeSinceIgnited = timeSinceIgnited + distance * 1.5F;
    if (timeSinceIgnited > fuseTime - 5)
    {
        timeSinceIgnited = fuseTime - 5;
    }
}

void EntityCreeper::registerFixesCreeper(DataFixer fixer)
{
    EntityLiving::registerFixesMob(fixer, EntityCreeper.class);
}

void EntityCreeper::writeEntityToNBT(NBTTagCompound *compound)
{
    EntityLiving::writeEntityToNBT(compound);
    if (dataManager.get(POWERED))
    {
        compound->setBoolean("powered", true);
    }

    compound->setShort("Fuse", fuseTime);
    compound->setByte("ExplosionRadius", explosionRadius);
    compound->setBoolean("ignited", hasIgnited());
}

void EntityCreeper::readEntityFromNBT(NBTTagCompound *compound)
{
    EntityLiving::readEntityFromNBT(compound);
    dataManager.set(POWERED, compound->getBoolean("powered"));
    if (compound->hasKey("Fuse", 99))
    {
        fuseTime = compound->getShort("Fuse");
    }

    if (compound->hasKey("ExplosionRadius", 99))
    {
        explosionRadius = compound->getByte("ExplosionRadius");
    }

    if (compound->getBoolean("ignited"))
    {
        ignite();
    }
}

void EntityCreeper::onUpdate()
{
    if (isEntityAlive())
    {
        lastActiveTime = timeSinceIgnited;
        if (hasIgnited())
        {
            setCreeperState(1);
        }

        auto i = getCreeperState();
        if (i > 0 && timeSinceIgnited == 0)
        {
            playSound(SoundEvents.ENTITY_CREEPER_PRIMED, 1.0F, 0.5F);
        }

        timeSinceIgnited += i;
        if (timeSinceIgnited < 0)
        {
            timeSinceIgnited = 0;
        }

        if (timeSinceIgnited >= fuseTime)
        {
            timeSinceIgnited = fuseTime;
            explode();
        }
    }

    EntityLiving::onUpdate();
}

void EntityCreeper::onDeath(DamageSource::DamageSource cause)
{
    EntityMob::onDeath(cause);
    if (world->getGameRules().getBoolean("doMobLoot"))
    {
        if (Util:: instanceof <EntitySkeleton>(cause.getTrueSource()))
        {
            auto i = Item::getIdFromItem(Items::RECORD_13);
            auto j = Item::getIdFromItem(Items::RECORD_WAIT);
            auto k = i + rand(j - i + 1);
            dropItem(Item::getItemById(k), 1);
        }
        else if (Util:: instanceof <EntityCreeper>(cause.getTrueSource()) && cause.getTrueSource() != this &&
                                       ((EntityCreeper *)cause.getTrueSource())->getPowered() &&
                                       ((EntityCreeper *)cause.getTrueSource())->ableToCauseSkullDrop())
        {
            ((EntityCreeper *)cause.getTrueSource())->incrementDroppedSkulls();
            entityDropItem(ItemStack(Items::SKULL, 1, 4), 0.0F);
        }
    }
}

bool EntityCreeper::attackEntityAsMob(Entity *entityIn)
{
    return true;
}

bool EntityCreeper::getPowered() const
{
    return dataManager.get(POWERED);
}

float EntityCreeper::getCreeperFlashIntensity(float p_70831_1_) const
{
    return (lastActiveTime + (timeSinceIgnited - lastActiveTime) * p_70831_1_) / (fuseTime - 2);
}

int32_t EntityCreeper::getCreeperState()
{
    return dataManager.get(STATE);
}

void EntityCreeper::setCreeperState(int32_t state)
{
    dataManager.set(STATE, state);
}

void EntityCreeper::onStruckByLightning(EntityLightningBolt *lightningBolt)
{
    EntityMob::onStruckByLightning(lightningBolt);
    dataManager.set(POWERED, true);
}

bool EntityCreeper::hasIgnited()
{
    return dataManager.get(IGNITED);
}

void EntityCreeper::ignite()
{
    dataManager.set(IGNITED, true);
}

bool EntityCreeper::ableToCauseSkullDrop() const
{
    return droppedSkulls < 1 && world->getGameRules().getBoolean("doMobLoot");
}

void EntityCreeper::incrementDroppedSkulls()
{
    ++droppedSkulls;
}

void EntityCreeper::initEntityAI()
{
    tasks.addTask(1, new EntityAISwimming(this));
    tasks.addTask(2, new EntityAICreeperSwell(this));
    tasks.addTask(3, new EntityAIAvoidEntity(this, typeid(EntityOcelot), 6.0F, 1.0, 1.2));
    tasks.addTask(4, new EntityAIAttackMelee(this, 1.0, false));
    tasks.addTask(5, new EntityAIWanderAvoidWater(this, 0.8));
    tasks.addTask(6, new EntityAIWatchClosest(this, typeid(EntityPlayer), 8.0F));
    tasks.addTask(6, new EntityAILookIdle(this));
    targetTasks.addTask(1, new EntityAINearestAttackableTarget(this, typeid(EntityPlayer), true));
    targetTasks.addTask(2, new EntityAIHurtByTarget(this, false, {});
}

void EntityCreeper::applyEntityAttributes()
{
    EntityMob::applyEntityAttributes();
    getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED.get())->setBaseValue(0.25);
}

void EntityCreeper::entityInit()
{
    EntityMob::entityInit();
    dataManager.registe(STATE, -1);
    dataManager.registe(POWERED, false);
    dataManager.registe(IGNITED, false);
}

SoundEvent EntityCreeper::getHurtSound(DamageSource::DamageSource damageSourceIn)
{
    return SoundEvents::ENTITY_CREEPER_HURT;
}

SoundEvent EntityCreeper::getDeathSound()
{
    return SoundEvents::ENTITY_CREEPER_DEATH;
}

std::optional<ResourceLocation> EntityCreeper::getLootTable()
{
    return LootTableList::ENTITIES_CREEPER;
}

bool EntityCreeper::processInteract(EntityPlayer *player, EnumHand hand)
{
    ItemStack itemstack = player->getHeldItem(hand);
    if (itemstack.getItem() == Items::FLINT_AND_STEEL)
    {
        world->playSound(player, posX, posY, posZ, SoundEvents::ITEM_FLINTANDSTEEL_USE, getSoundCategory(), 1.0F,
                         MathHelper::nextFloat(rand) * 0.4F + 0.8F);
        player->swingArm(hand);
        if (!world->isRemote)
        {
            ignite();
            itemstack.damageItem(1, player);
            return true;
        }
    }

    return EntityMob::processInteract(player, hand);
}

void EntityCreeper::explode()
{
    if (!world->isRemote)
    {
        bool flag = world->getGameRules().getBoolean("mobGriefing");
        float f   = getPowered() ? 2.0F : 1.0F;
        dead      = true;
        world->createExplosion(this, posX, posY, posZ, explosionRadius * f, flag);
        setDead();
        spawnLingeringCloud();
    }
}

void EntityCreeper::spawnLingeringCloud() const
{
    auto collection = getActivePotionEffects();
    if (!collection.empty())
    {
        EntityAreaEffectCloud *entityareaeffectcloud = new EntityAreaEffectCloud(world, posX, posY, posZ);
        entityareaeffectcloud->setRadius(2.5F);
        entityareaeffectcloud->setRadiusOnUse(-0.5F);
        entityareaeffectcloud->setWaitTime(10);
        entityareaeffectcloud->setDuration(entityareaeffectcloud->getDuration() / 2);
        entityareaeffectcloud->setRadiusPerTick(-entityareaeffectcloud->getRadius() /
                                                entityareaeffectcloud->getDuration());

        for (auto potioneffect : collection)
        {
            entityareaeffectcloud->addEffect(PotionEffect(potioneffect));
        }

        world->spawnEntity(entityareaeffectcloud);
    }
}
