#include "EntityEndermite.h"

#include "../SharedMonsterAttributes.h"
#include "../ai/EntityAIAttackMelee.h"
#include "../ai/EntityAIHurtByTarget.h"
#include "../ai/EntityAILookIdle.h"
#include "../ai/EntityAINearestAttackableTarget.h"
#include "../ai/EntityAISwimming.h"
#include "../ai/EntityAIWanderAvoidWater.h"
#include "../ai/EntityAIWatchClosest.h"
#include "../ai/attributes/IAttributeInstance.h"

EntityEndermite::EntityEndermite(World *worldIn) : EntityMob(worldIn)
{
    experienceValue = 3;
    setSize(0.4F, 0.3F);
}

float EntityEndermite::getEyeHeight() const
{
    return 0.1F;
}

void EntityEndermite::readEntityFromNBT(NBTTagCompound *compound)
{
    EntityMob::readEntityFromNBT(compound);
    lifetime      = compound->getInteger("Lifetime");
    playerSpawned = compound->getBoolean("PlayerSpawned");
}

void EntityEndermite::writeEntityToNBT(NBTTagCompound *compound)
{
    EntityMob::writeEntityToNBT(compound);
    compound->setInteger("Lifetime", lifetime);
    compound->setBoolean("PlayerSpawned", playerSpawned);
}

double EntityEndermite::getYOffset()
{
    return 0.1;
}

bool EntityEndermite::isSpawnedByPlayer() const
{
    return playerSpawned;
}

void EntityEndermite::setSpawnedByPlayer(bool spawnedByPlayer)
{
    playerSpawned = spawnedByPlayer;
}

void EntityEndermite::onLivingUpdate()
{
    EntityMob::onLivingUpdate();
    if (world->isRemote)
    {
        for (auto i = 0; i < 2; ++i)
        {
            world->spawnParticle(EnumParticleTypes::PORTAL, posX + (MathHelper::nextDouble(rand) - 0.5) * (double)width,
                                 posY + MathHelper::nextDouble(rand) * (double)height,
                                 posZ + (MathHelper::nextDouble(rand) - 0.5) * (double)width,
                                 (MathHelper::nextDouble(rand) - 0.5) * 2.0, -MathHelper::nextDouble(rand),
                                 (MathHelper::nextDouble(rand) - 0.5) * 2.0, {});
        }
    }
    else
    {
        if (!isNoDespawnRequired())
        {
            ++lifetime;
        }

        if (lifetime >= 2400)
        {
            setDead();
        }
    }
}

bool EntityEndermite::getCanSpawnHere()
{
    if (EntityMob::getCanSpawnHere())
    {
        EntityPlayer *entityplayer = world->getClosestPlayerToEntity(this, 5.0);
        return entityplayer == nullptr;
    }
    else
    {
        return false;
    }
}

EnumCreatureAttribute EntityEndermite::getCreatureAttribute()
{
    return EnumCreatureAttribute::ARTHROPOD;
}

void EntityEndermite::initEntityAI()
{
    tasks.addTask(1, new EntityAISwimming(this));
    tasks.addTask(2, new EntityAIAttackMelee(this, 1.0, false));
    tasks.addTask(3, new EntityAIWanderAvoidWater(this, 1.0));
    tasks.addTask(7, new EntityAIWatchClosest(this, EntityPlayer.class, 8.0F));
    tasks.addTask(8, new EntityAILookIdle(this));
    targetTasks.addTask(1, new EntityAIHurtByTarget(this, true, {}));
    targetTasks.addTask(2, new EntityAINearestAttackableTarget(this, EntityPlayer.class, true));
}

void EntityEndermite::applyEntityAttributes()
{
    EntityMob::applyEntityAttributes();
    getEntityAttribute(SharedMonsterAttributes::MAX_HEALTH.get())->setBaseValue(8.0);
    getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED.get())->setBaseValue(0.25);
    getEntityAttribute(SharedMonsterAttributes::ATTACK_DAMAGE.get())->setBaseValue(2.0);
}

bool EntityEndermite::canTriggerWalking()
{
    return false;
}

SoundEvent EntityEndermite::getAmbientSound()
{
    return SoundEvents::ENTITY_ENDERMITE_AMBIENT;
}

SoundEvent EntityEndermite::getHurtSound(DamageSource::DamageSource damageSourceIn)
{
    return SoundEvents::ENTITY_ENDERMITE_HURT;
}

SoundEvent EntityEndermite::getDeathSound()
{
    return SoundEvents::ENTITY_ENDERMITE_DEATH;
}

void EntityEndermite::playStepSound(BlockPos pos, Block *blockIn)
{
    playSound(SoundEvents::ENTITY_ENDERMITE_STEP, 0.15F, 1.0F);
}

std::optional<ResourceLocation> EntityEndermite::getLootTable()
{
    return LootTableList::ENTITIES_ENDERMITE;
}

void EntityEndermite::registerFixesEndermite(DataFixer fixer)
{
    EntityLiving::registerFixesMob(fixer, EntityEndermite.class);
}

void EntityEndermite::onUpdate()
{
    renderYawOffset = rotationYaw;
    EntityMob::onUpdate();
}

void EntityEndermite::setRenderYawOffset(float offset)
{
    rotationYaw = offset;
    EntityMob::setRenderYawOffset(offset);
}

bool EntityEndermite::isValidLightLevel()
{
    return true;
}
