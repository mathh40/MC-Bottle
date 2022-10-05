#include "EntityAIAttackRangedBow.h"

#include "../../item/ItemBow.h"
#include "EntityAIAttackRanged.h"
#include "EnumHand.h"

EntityAIAttackRangedBow::EntityAIAttackRangedBow(EntityMob *mob, double moveSpeedAmpIn, int attackCooldownIn,
                                                 float maxAttackDistanceIn)
    : entity(mob), moveSpeedAmp(moveSpeedAmpIn), attackCooldown(attackCooldownIn),
      maxAttackDistance(maxAttackDistanceIn * maxAttackDistanceIn)
{
    setMutexBits(3);
}

void EntityAIAttackRangedBow::setAttackCooldown(int32_t p_189428_1_)
{
    attackCooldown = p_189428_1_;
}

bool EntityAIAttackRangedBow::shouldExecute()
{
    return entity->getAttackTarget() == nullptr ? false : isBowInMainhand();
}

bool EntityAIAttackRangedBow::shouldContinueExecuting()
{
    return (shouldExecute() || !entity->getNavigator().noPath()) && isBowInMainhand();
}

void EntityAIAttackRangedBow::startExecuting()
{
    EntityAIBase::startExecuting();
    ((IRangedAttackMob *)entity)->setSwingingArms(true);
}

void EntityAIAttackRangedBow::resetTask()
{
    EntityAIBase::resetTask();
    ((IRangedAttackMob *)entity)->setSwingingArms(false);
    seeTime    = 0;
    attackTime = -1;
    entity->resetActiveHand();
}

void EntityAIAttackRangedBow::updateTask()
{
    EntityLivingBase *entitylivingbase = entity->getAttackTarget();
    if (entitylivingbase != nullptr)
    {
        double d0  = entity->getDistanceSq(entitylivingbase->posX, entitylivingbase->getEntityBoundingBox().minY,
                                           entitylivingbase->posZ);
        bool flag  = entity->getEntitySenses().canSee(entitylivingbase);
        bool flag1 = seeTime > 0;
        if (flag != flag1)
        {
            seeTime = 0;
        }

        if (flag)
        {
            ++seeTime;
        }
        else
        {
            --seeTime;
        }

        if (d0 <= maxAttackDistance && seeTime >= 20)
        {
            entity->getNavigator().clearPath();
            ++strafingTime;
        }
        else
        {
            entity->getNavigator().tryMoveToEntityLiving(entitylivingbase, moveSpeedAmp);
            strafingTime = -1;
        }

        if (strafingTime >= 20)
        {
            if (entity->getRNG().nextFloat() < 0.3)
            {
                strafingClockwise = !strafingClockwise;
            }

            if (entity->getRNG().nextFloat() < 0.3)
            {
                strafingBackwards = !strafingBackwards;
            }

            strafingTime = 0;
        }

        if (strafingTime > -1)
        {
            if (d0 > maxAttackDistance * 0.75F)
            {
                strafingBackwards = false;
            }
            else if (d0 < maxAttackDistance * 0.25F)
            {
                strafingBackwards = true;
            }

            entity->getMoveHelper().strafe(strafingBackwards ? -0.5F : 0.5F, strafingClockwise ? 0.5F : -0.5F);
            entity->faceEntity(entitylivingbase, 30.0F, 30.0F);
        }
        else
        {
            entity->getLookHelper().setLookPositionWithEntity(entitylivingbase, 30.0F, 30.0F);
        }

        if (entity->isHandActive())
        {
            if (!flag && seeTime < -60)
            {
                entity->resetActiveHand();
            }
            else if (flag)
            {
                int32_t i = entity->getItemInUseMaxCount();
                if (i >= 20)
                {
                    entity->resetActiveHand();
                    ((IRangedAttackMob *)entity)
                        ->attackEntityWithRangedAttack(entitylivingbase, ItemBow::getArrowVelocity(i));
                    attackTime = attackCooldown;
                }
            }
        }
        else if (--attackTime <= 0 && seeTime >= -60)
        {
            entity->setActiveHand(EnumHand::MAIN_HAND);
        }
    }
}

bool EntityAIAttackRangedBow::isBowInMainhand()
{
    return !entity->getHeldItemMainhand().isEmpty() && entity->getHeldItemMainhand().getItem() == Items::BOW;
}
