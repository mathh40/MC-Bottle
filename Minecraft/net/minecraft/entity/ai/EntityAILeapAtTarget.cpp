#include "EntityAILeapAtTarget.h"

#include "math/MathHelper.h"

EntityAILeapAtTarget::EntityAILeapAtTarget(EntityLiving* leapingEntity, float leapMotionYIn)
    :leaper(leapingEntity),leapMotionY(leapMotionYIn)
{
    setMutexBits(5);
}

bool EntityAILeapAtTarget::shouldExecute()
{
    leapTarget = leaper->getAttackTarget();
    if (leapTarget == nullptr) 
    {
        return false;
    }
    else 
    {
        double d0 = leaper->getDistanceSq(leapTarget);
        if (d0 >= 4.0 && d0 <= 16.0) 
        {
            if (!leaper->onGround) 
            {
                return false;
            }
            else 
            {
                return leaper->getRNG().nextInt(5) == 0;
            }
        }
        else 
        {
            return false;
        }
    }
}

bool EntityAILeapAtTarget::shouldContinueExecuting()
{
    return !leaper->onGround;
}

void EntityAILeapAtTarget::startExecuting()
{
    double d0 = leapTarget->posX - leaper->posX;
    double d1 = leapTarget->posZ - leaper->posZ;
    float f = MathHelper::sqrt(d0 * d0 + d1 * d1);
    if ((double)f >= 1.0E-4) 
    {
        leaper->motionX += d0 / (double)f * 0.5 * 0.800000011920929 + leaper->motionX * 0.20000000298023224;
        leaper->motionZ += d1 / (double)f * 0.5 * 0.800000011920929 + leaper->motionZ * 0.20000000298023224;
    }

    leaper->motionY = (double)leapMotionY;
}
