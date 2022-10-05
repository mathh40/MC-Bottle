#include "EntityBlaze.h"

#include "../SharedMonsterAttributes.h"
#include "../ai/EntityAIHurtByTarget.h"
#include "../ai/EntityAILookIdle.h"
#include "../ai/EntityAIMoveTowardsRestriction.h"
#include "../ai/EntityAINearestAttackableTarget.h"
#include "../ai/EntityAIWanderAvoidWater.h"
#include "../ai/EntityAIWatchClosest.h"
#include "../ai/attributes/IAttributeInstance.h"
#include "datafix/DataFixer.h"

class AIFireballAttack : public EntityAIBase
{
  public:
    AIFireballAttack(EntityBlaze *blazeIn);

    bool shouldExecute() override;

    void startExecuting() override;

    void resetTask() override;

    void updateTask() override;

  private:
    double getFollowDistance() const;

    EntityBlaze *blaze;
    int32_t attackStep;
    int32_t attackTime;
};

AIFireballAttack::AIFireballAttack(EntityBlaze *blazeIn) : blaze(blazeIn)
{
    setMutexBits(3);
}

bool AIFireballAttack::shouldExecute()
{
    EntityLivingBase *entitylivingbase = blaze->getAttackTarget();
    return entitylivingbase != nullptr && entitylivingbase->isEntityAlive();
}

void AIFireballAttack::startExecuting()
{
    attackStep = 0;
}

void AIFireballAttack::resetTask()
{
    blaze->setOnFire(false);
}

void AIFireballAttack::updateTask()
{
    --attackTime;
    EntityLivingBase *entitylivingbase = blaze->getAttackTarget();
    double d0                          = blaze->getDistanceSq(entitylivingbase);
    if (d0 < 4.0)
    {
        if (attackTime <= 0)
        {
            attackTime = 20;
            blaze->attackEntityAsMob(entitylivingbase);
        }

        blaze->getMoveHelper().setMoveTo(entitylivingbase->posX, entitylivingbase->posY, entitylivingbase->posZ, 1.0);
    }
    else if (d0 < getFollowDistance() * getFollowDistance())
    {
        double d1 = entitylivingbase->posX - blaze->posX;
        double d2 = entitylivingbase->getEntityBoundingBox().getminY() + (double)(entitylivingbase->height / 2.0F) -
                    (blaze->posY + (double)(blaze->height / 2.0F));
        double d3 = entitylivingbase->posZ - blaze->posZ;
        if (attackTime <= 0)
        {
            ++attackStep;
            if (attackStep == 1)
            {
                attackTime = 60;
                blaze->setOnFire(true);
            }
            else if (attackStep <= 4)
            {
                attackTime = 6;
            }
            else
            {
                attackTime = 100;
                attackStep = 0;
                blaze->setOnFire(false);
            }

            if (attackStep > 1)
            {
                float f = MathHelper::sqrt(MathHelper::sqrt(d0)) * 0.5F;
                blaze->world->playEvent(nullptr, 1018, BlockPos(blaze->posX, blaze->posY, blaze->posZ), 0);

                for (int i = 0; i < 1; ++i)
                {
                    EntitySmallFireball *entitysmallfireball = new EntitySmallFireball(
                        blaze->world, blaze, d1 + MathHelper::nextGaussian<double>(blaze->getRNG()) * (double)f, d2,
                        d3 + MathHelper::nextGaussian<double>(blaze->getRNG()) * (double)f);
                    entitysmallfireball->posY = blaze->posY + (double)(blaze->height / 2.0F) + 0.5;
                    blaze->world->spawnEntity(entitysmallfireball);
                }
            }
        }

        blaze->getLookHelper().setLookPositionWithEntity(entitylivingbase, 10.0F, 10.0F);
    }
    else
    {
        blaze->getNavigator()->clearPath();
        blaze->getMoveHelper().setMoveTo(entitylivingbase->posX, entitylivingbase->posY, entitylivingbase->posZ, 1.0);
    }

    EntityAIBase::updateTask();
}

double AIFireballAttack::getFollowDistance() const
{
    IAttributeInstance *iattributeinstance = blaze->getEntityAttribute(SharedMonsterAttributes::FOLLOW_RANGE.get());
    return iattributeinstance == nullptr ? 16.0 : iattributeinstance->getAttributeValue();
}

DataParameter EntityBlaze::ON_FIRE = EntityDataManager.createKey(EntityBlaze.class, DataSerializers::BYTE);

EntityBlaze::EntityBlaze(World *worldIn) : EntityMob(worldIn)
{
    setPathPriority(PathNodeType::WATER, -1.0F);
    setPathPriority(PathNodeType::LAVA, 8.0F);
    setPathPriority(PathNodeType::DANGER_FIRE, 0.0F);
    setPathPriority(PathNodeType::DAMAGE_FIRE, 0.0F);
    bisImmuneToFire = true;
    experienceValue = 10;
}

void EntityBlaze::registerFixesBlaze(DataFixer fixer)
{
    EntityLiving::registerFixesMob(fixer, EntityBlaze.class);
}

void EntityBlaze::initEntityAI()
{
    tasks.addTask(4, new AIFireballAttack(this));
    tasks.addTask(5, new EntityAIMoveTowardsRestriction(this, 1.0));
    tasks.addTask(7, new EntityAIWanderAvoidWater(this, 1.0, 0.0F));
    tasks.addTask(8, new EntityAIWatchClosest(this, EntityPlayer.class, 8.0F));
    tasks.addTask(8, new EntityAILookIdle(this));
    targetTasks.addTask(1, new EntityAIHurtByTarget(this, true, new Class[0]));
    targetTasks.addTask(2, new EntityAINearestAttackableTarget(this, EntityPlayer.class, true));
}

void EntityBlaze::applyEntityAttributes()
{
    EntityLiving::applyEntityAttributes();
    getEntityAttribute(SharedMonsterAttributes::ATTACK_DAMAGE.get())->setBaseValue(6.0);
    getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED.get())->setBaseValue(0.23000000417232513);
    getEntityAttribute(SharedMonsterAttributes::FOLLOW_RANGE.get())->setBaseValue(48.0);
}

void EntityBlaze::entityInit()
{
    EntityLiving::entityInit();
    dataManager.registe(ON_FIRE, 0);
}

SoundEvent EntityBlaze::getAmbientSound()
{
    return SoundEvents::ENTITY_BLAZE_AMBIENT;
}

SoundEvent EntityBlaze::getHurtSound(DamageSource::DamageSource damageSourceIn)
{
    return SoundEvents::ENTITY_BLAZE_HURT;
}

SoundEvent EntityBlaze::getDeathSound()
{
    return SoundEvents::ENTITY_BLAZE_DEATH;
}

void EntityBlaze::updateAITasks()
{
    if (isWet())
    {
        attackEntityFrom(DamageSource::DROWN, 1.0F);
    }

    --heightOffsetUpdateTime;
    if (heightOffsetUpdateTime <= 0)
    {
        heightOffsetUpdateTime = 100;
        heightOffset           = 0.5F + MathHelper::nextGaussian<float>(rand) * 3.0F;
    }

    EntityLivingBase *entitylivingbase = getAttackTarget();
    if (entitylivingbase != nullptr && entitylivingbase->posY + (double)entitylivingbase->getEyeHeight() >
                                           posY + (double)getEyeHeight() + (double)heightOffset)
    {
        motionY += (0.30000001192092896 - motionY) * 0.30000001192092896;
        isAirBorne = true;
    }

    EntityLiving::updateAITasks();
}

void EntityBlaze::fall(float distance, float damageMultiplier)
{
}

std::optional<ResourceLocation> EntityBlaze::getLootTable()
{
    return LootTableList::ENTITIES_BLAZE;
}

bool EntityBlaze::isValidLightLevel()
{
    return true;
}

int32_t EntityBlaze::getBrightnessForRender()
{
    return 15728880;
}

float EntityBlaze::getBrightness()
{
    return 1.0F;
}

void EntityBlaze::onLivingUpdate()
{
    if (!onGround && motionY < 0.0)
    {
        motionY *= 0.6;
    }

    if (world->isRemote)
    {
        if (rand(24) == 0 && !isSilent())
        {
            world->playSound(posX + 0.5, posY + 0.5, posZ + 0.5, SoundEvents::ENTITY_BLAZE_BURN, getSoundCategory(),
                             1.0F + MathHelper::nextFloat(rand), MathHelper::nextFloat(rand) * 0.7F + 0.3F, false);
        }

        for (auto i = 0; i < 2; ++i)
        {
            world->spawnParticle(EnumParticleTypes::SMOKE_LARGE,
                                 posX + (MathHelper::nextDouble(rand) - 0.5) * (double)width,
                                 posY + MathHelper::nextDouble(rand) * (double)height,
                                 posZ + (MathHelper::nextDouble(rand) - 0.5) * (double)width, 0.0, 0.0, 0.0, {});
        }
    }

    EntityLiving::onLivingUpdate();
}

bool EntityBlaze::isBurning()
{
    return isCharged();
}

bool EntityBlaze::isCharged()
{
    return dataManager.get(ON_FIRE) & 1) != 0;
}

void EntityBlaze::setOnFire(bool onFire)
{
    std::byte b0 = dataManager.get(ON_FIRE);
    if (onFire)
    {
        b0 = (b0 | 1);
    }
    else
    {
        b0 &= -2;
    }

    dataManager.set(ON_FIRE, b0);
}
