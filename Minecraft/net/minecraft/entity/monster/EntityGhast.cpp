#include "EntityGhast.h"

#include "SharedMonsterAttributes.h"
#include "ai/EntityAIFindEntityNearestPlayer.h"
#include "ai/attributes/IAttributeInstance.h"

DataParameter EntityGhast::ATTACKING = EntityDataManager::createKey(EntityGhast.class, DataSerializers::BOOLEAN);

EntityGhast::GhastMoveHelper::GhastMoveHelper(EntityGhast *ghast) : EntityMoveHelper(ghast), parentEntity(ghast)
{
}

void EntityGhast::GhastMoveHelper::onUpdateMoveHelper()
{
    if (action == EntityMoveHelper::Action::MOVE_TO)
    {
        double d0 = posX - parentEntity->posX;
        double d1 = posY - parentEntity->posY;
        double d2 = posZ - parentEntity->posZ;
        double d3 = d0 * d0 + d1 * d1 + d2 * d2;
        if (courseChangeCooldown-- <= 0)
        {
            courseChangeCooldown += parentEntity->getRNG()(5) + 2;
            d3 = (double)MathHelper::sqrt(d3);
            if (isNotColliding(posX, posY, posZ, d3))
            {
                parentEntity->motionX += d0 / d3 * 0.1;
                parentEntity->motionY += d1 / d3 * 0.1;
                parentEntity->motionZ += d2 / d3 * 0.1;
            }
            else
            {
                action = EntityMoveHelper::Action::WAIT;
            }
        }
    }
}

bool EntityGhast::GhastMoveHelper::isNotColliding(double x, double y, double z, double p_179926_7_) const
{
    double d0                   = (x - parentEntity->posX) / p_179926_7_;
    double d1                   = (y - parentEntity->posY) / p_179926_7_;
    double d2                   = (z - parentEntity->posZ) / p_179926_7_;
    AxisAlignedBB axisalignedbb = parentEntity->getEntityBoundingBox();

    for (int i = 1; (double)i < p_179926_7_; ++i)
    {
        axisalignedbb = axisalignedbb.offset(d0, d1, d2);
        if (!parentEntity->world->getCollisionBoxes(parentEntity, axisalignedbb)->empty())
        {
            return false;
        }
    }

    return true;
}

EntityGhast::AIRandomFly::AIRandomFly(EntityGhast *ghast) : parentEntity(ghast)
{
    setMutexBits(1);
}

bool EntityGhast::AIRandomFly::shouldExecute()
{
    EntityMoveHelper entitymovehelper = parentEntity->getMoveHelper();
    if (!entitymovehelper.isUpdating())
    {
        return true;
    }
    else
    {
        double d0 = entitymovehelper.getX() - parentEntity->posX;
        double d1 = entitymovehelper.getY() - parentEntity->posY;
        double d2 = entitymovehelper.getZ() - parentEntity->posZ;
        double d3 = d0 * d0 + d1 * d1 + d2 * d2;
        return d3 < 1.0 || d3 > 3600.0;
    }
}

bool EntityGhast::AIRandomFly::shouldContinueExecuting()
{
    return false;
}

void EntityGhast::AIRandomFly::startExecuting()
{
    auto &random = parentEntity->getRNG();
    double d0    = parentEntity->posX + (double)((MathHelper::nextFloat(random) * 2.0F - 1.0F) * 16.0F);
    double d1    = parentEntity->posY + (double)((MathHelper::nextFloat(random) * 2.0F - 1.0F) * 16.0F);
    double d2    = parentEntity->posZ + (double)((MathHelper::nextFloat(random) * 2.0F - 1.0F) * 16.0F);
    parentEntity->getMoveHelper()->setMoveTo(d0, d1, d2, 1.0);
}

EntityGhast::AILookAround::AILookAround(EntityGhast *ghast) : parentEntity(ghast)
{
    setMutexBits(2);
}

bool EntityGhast::AILookAround::shouldExecute()
{
    return true;
}

void EntityGhast::AILookAround::updateTask()
{
    if (parentEntity->getAttackTarget() == nullptr)
    {
        parentEntity->rotationYaw =
            -((float)MathHelper::atan2(parentEntity->motionX, parentEntity->motionZ)) * 57.295776F;
        parentEntity->renderYawOffset = parentEntity->rotationYaw;
    }
    else
    {
        EntityLivingBase *entitylivingbase = parentEntity->getAttackTarget();
        double d0                          = 64.0;
        if (entitylivingbase->getDistanceSq(parentEntity) < 4096.0)
        {
            double d1                     = entitylivingbase->posX - parentEntity->posX;
            double d2                     = entitylivingbase->posZ - parentEntity->posZ;
            parentEntity->rotationYaw     = -((float)MathHelper::atan2(d1, d2)) * 57.295776F;
            parentEntity->renderYawOffset = parentEntity->rotationYaw;
        }
    }
}

EntityGhast::AIFireballAttack::AIFireballAttack(EntityGhast *ghast)
{
    parentEntity = ghast;
}

bool EntityGhast::AIFireballAttack::shouldExecute()
{
    return parentEntity->getAttackTarget() != nullptr;
}

void EntityGhast::AIFireballAttack::startExecuting()
{
    attackTimer = 0;
}

void EntityGhast::AIFireballAttack::resetTask()
{
    parentEntity->setAttacking(false);
}

void EntityGhast::AIFireballAttack::updateTask()
{
    EntityLivingBase *entitylivingbase = parentEntity->getAttackTarget();
    double d0                          = 64.0;
    if (entitylivingbase->getDistanceSq(parentEntity) < 4096.0 && parentEntity->canEntityBeSeen(entitylivingbase))
    {
        World *world = parentEntity->world;
        ++attackTimer;
        if (attackTimer == 10)
        {
            world->playEvent(nullptr, 1015, BlockPos(parentEntity), 0);
        }

        if (attackTimer == 20)
        {
            double d1   = 4.0;
            Vec3d vec3d = parentEntity->getLook(1.0F);
            double d2   = entitylivingbase->posX - (parentEntity->posX + vec3d.getx() * 4.0);
            double d3 = entitylivingbase->getEntityBoundingBox().getminY() + (double)(entitylivingbase->height / 2.0F) -
                        (0.5 + parentEntity->posY + (double)(parentEntity->height / 2.0F));
            double d4 = entitylivingbase->posZ - (parentEntity->posZ + vec3d.getz() * 4.0);
            world->playEvent(nullptr, 1016, BlockPos(parentEntity), 0);
            EntityLargeFireball *entitylargefireball = new EntityLargeFireball(world, parentEntity, d2, d3, d4);
            entitylargefireball->explosionPower      = parentEntity->getFireballStrength();
            entitylargefireball->posX                = parentEntity->posX + vec3d.getx() * 4.0;
            entitylargefireball->posY                = parentEntity->posY + (double)(parentEntity->height / 2.0F) + 0.5;
            entitylargefireball->posZ                = parentEntity->posZ + vec3d.getz() * 4.0;
            world->spawnEntity(entitylargefireball);
            attackTimer = -40;
        }
    }
    else if (attackTimer > 0)
    {
        --attackTimer;
    }

    parentEntity->setAttacking(attackTimer > 10);
}

EntityGhast::EntityGhast(World *worldIn) : EntityFlying(worldIn)
{
    setSize(4.0F, 4.0F);
    bisImmuneToFire = true;
    experienceValue = 5;
    moveHelper      = std::make_unique<GhastMoveHelper>(this);
}

bool EntityGhast::isAttacking()
{
    return dataManager.get(ATTACKING);
}

void EntityGhast::setAttacking(bool attacking)
{
    dataManager.set(ATTACKING, attacking);
}

int32_t EntityGhast::getFireballStrength() const
{
    return explosionStrength;
}

void EntityGhast::onUpdate()
{
    EntityFlying::onUpdate();
    if (!world->isRemote && world->getDifficulty() == EnumDifficulty::PEACEFUL)
    {
        setDead();
    }
}

bool EntityGhast::attackEntityFrom(DamageSource::DamageSource source, float amount)
{
    if (isEntityInvulnerable(source))
    {
        return false;
    }
    else if (Util:: instanceof <EntityLargeFireball>(source.getImmediateSource()) && Util:: instanceof
             <EntityPlayer>(source.getTrueSource()))
    {
        EntityFlying::attackEntityFrom(source, 1000.0F);
        return true;
    }
    else
    {
        return EntityFlying::attackEntityFrom(source, amount);
    }
}

SoundCategory EntityGhast::getSoundCategory()
{
    return SoundCategory::HOSTILE;
}

bool EntityGhast::getCanSpawnHere()
{
    return rand(20) == 0 && EntityFlying::getCanSpawnHere() && world->getDifficulty() != EnumDifficulty::PEACEFUL;
}

int32_t EntityGhast::getMaxSpawnedInChunk()
{
    return 1;
}

void EntityGhast::registerFixesGhast(DataFixer fixer)
{
    EntityLiving::registerFixesMob(fixer, EntityGhast.class);
}

void EntityGhast::writeEntityToNBT(NBTTagCompound *compound)
{
    EntityFlying::writeEntityToNBT(compound);
    compound->setInteger("ExplosionPower", explosionStrength);
}

void EntityGhast::readEntityFromNBT(NBTTagCompound *compound)
{
    EntityFlying::readEntityFromNBT(compound);
    if (compound->hasKey("ExplosionPower", 99))
    {
        explosionStrength = compound->getInteger("ExplosionPower");
    }
}

float EntityGhast::getEyeHeight() const
{
    return 2.6F;
}

void EntityGhast::initEntityAI()
{
    tasks.addTask(5, new EntityGhast::AIRandomFly(this));
    tasks.addTask(7, new EntityGhast::AILookAround(this));
    tasks.addTask(7, new EntityGhast::AIFireballAttack(this));
    targetTasks.addTask(1, new EntityAIFindEntityNearestPlayer(this));
}

void EntityGhast::entityInit()
{
    EntityFlying::entityInit();
    dataManager.registe(ATTACKING, false);
}

void EntityGhast::applyEntityAttributes()
{
    EntityFlying::applyEntityAttributes();
    getEntityAttribute(SharedMonsterAttributes::MAX_HEALTH.get())->setBaseValue(10.0);
    getEntityAttribute(SharedMonsterAttributes::FOLLOW_RANGE.get())->setBaseValue(100.0);
}

SoundEvent EntityGhast::getAmbientSound()
{
    return SoundEvents::ENTITY_GHAST_AMBIENT;
}

SoundEvent EntityGhast::getHurtSound(DamageSource::DamageSource damageSourceIn)
{
    return SoundEvents::ENTITY_GHAST_HURT;
}

SoundEvent EntityGhast::getDeathSound()
{
    return SoundEvents::ENTITY_GHAST_DEATH;
}

std::optional<ResourceLocation> EntityGhast::getLootTable()
{
    return LootTableList::ENTITIES_GHAST;
}

float EntityGhast::getSoundVolume()
{
    return 10.0F;
}
