#include "EntityAIAttackMelee.h"



#include "EnumHand.h"
#include "Util.h"
#include "math/BlockPos.h"
#include "math/MathHelper.h"

EntityAIAttackMelee::EntityAIAttackMelee(EntityCreature *creature, double speedIn, bool useLongMemory)
    :attacker(creature),world(creature->world),speedTowardsTarget(speedIn),longMemory(useLongMemory)
{
    setMutexBits(3);
}

bool EntityAIAttackMelee::shouldExecute()
{
    EntityLivingBase* entitylivingbase = attacker->getAttackTarget();
    if (entitylivingbase == nullptr) 
    {
        return false;
    }
    else if (!entitylivingbase->isEntityAlive()) 
    {
        return false;
    }
    else
    {
        path = attacker->getNavigator().getPathToEntityLiving(entitylivingbase);
        if (path != nullptr) 
        {
            return true;
        }
        else 
        {
            return getAttackReachSqr(entitylivingbase) >= attacker->getDistanceSq(entitylivingbase.posX, entitylivingbase.getEntityBoundingBox().minY, entitylivingbase.posZ);
        }
    }
}

bool EntityAIAttackMelee::shouldContinueExecuting()
{
    EntityLivingBase* entitylivingbase = attacker->getAttackTarget();
    if (entitylivingbase == nullptr) 
    {
        return false;
    }
    else if (!entitylivingbase->isEntityAlive()) 
    {
        return false;
    }
    else if (!longMemory) 
    {
        return !attacker->getNavigator().noPath();
    }
    else if (!attacker->isWithinHomeDistanceFromPosition(BlockPos(entitylivingbase))) 
    {
        return false;
    }
    else 
    {
        return !(Util::instanceof<EntityPlayer>(entitylivingbase)) || !((EntityPlayer*)entitylivingbase)->isSpectator() && !((EntityPlayer*)entitylivingbase)->isCreative();
    }
}

void EntityAIAttackMelee::startExecuting()
{
    attacker->getNavigator().setPath(path, speedTowardsTarget);
    delayCounter = 0;
}

void EntityAIAttackMelee::resetTask()
{
    EntityLivingBase* entitylivingbase = attacker->getAttackTarget();
    if (Util::instanceof<EntityPlayer>(entitylivingbase) && (((EntityPlayer*)entitylivingbase)->isSpectator() || ((EntityPlayer*)entitylivingbase)->isCreative())) 
    {
        attacker->setAttackTarget(nullptr);
    }

    attacker->getNavigator().clearPath();
}

void EntityAIAttackMelee::updateTask()
{
    EntityLivingBase* entitylivingbase = attacker->getAttackTarget();
    attacker->getLookHelper().setLookPositionWithEntity(entitylivingbase, 30.0F, 30.0F);
    double d0 = attacker->getDistanceSq(entitylivingbase->posX, entitylivingbase->getEntityBoundingBox().minY, entitylivingbase->posZ);
    --delayCounter;
    if ((longMemory || attacker->getEntitySenses().canSee(entitylivingbase)) && delayCounter <= 0 && (targetX == 0.0 && targetY == 0.0 && targetZ == 0.0 || entitylivingbase->getDistanceSq(targetX, targetY, targetZ) >= 1.0 || attacker->getRNG().nextFloat() < 0.05F)) 
    {
        targetX = entitylivingbase->posX;
        targetY = entitylivingbase->getEntityBoundingBox().minY;
        targetZ = entitylivingbase->posZ;
        delayCounter = 4 + attacker->getRNG().nextInt(7);
        if (d0 > 1024.0) 
        {
            delayCounter += 10;
        }
        else if (d0 > 256.0) 
        {
            delayCounter += 5;
        }

        if (!attacker->getNavigator().tryMoveToEntityLiving(entitylivingbase, speedTowardsTarget)) 
        {
            delayCounter += 15;
        }
    }

    attackTick = MathHelper::max(attackTick - 1, 0);
    checkAndPerformAttack(entitylivingbase, d0);
}

void EntityAIAttackMelee::checkAndPerformAttack(EntityLivingBase *enemy, double distToEnemySqr)
{
    double d0 = getAttackReachSqr(enemy);
    if (distToEnemySqr <= d0 && attackTick <= 0) 
    {
        attackTick = 20;
        attacker->swingArm(EnumHand::MAIN_HAND);
        attacker->attackEntityAsMob(enemy);
    }
}

double EntityAIAttackMelee::getAttackReachSqr(EntityLivingBase *attackTarget) const
{
    return attacker->width * 2.0F * attacker->width * 2.0F + attackTarget->width;
}
