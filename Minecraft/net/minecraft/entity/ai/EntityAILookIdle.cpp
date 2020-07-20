#include "EntityAILookIdle.h"

#include "math/MathHelper.h"

EntityAILookIdle::EntityAILookIdle(EntityLiving *entitylivingIn)
    :idleEntity(entitylivingIn)
{
    setMutexBits(3);
}

bool EntityAILookIdle::shouldExecute()
{
    return idleEntity->getRNG().nextFloat() < 0.02F;
}

bool EntityAILookIdle::shouldContinueExecuting()
{
    return idleTime >= 0;
}

void EntityAILookIdle::startExecuting()
{
    double d0 = 6.283185307179586 * idleEntity->getRNG().nextDouble();
    lookX = MathHelper::cos(d0);
    lookZ = MathHelper::sin(d0);
    idleTime = 20 + idleEntity->getRNG().nextInt(20);
}

void EntityAILookIdle::updateTask()
{
    --idleTime;
    idleEntity->getLookHelper().setLookPosition(idleEntity->posX + lookX, idleEntity->posY + (double)idleEntity->getEyeHeight(), idleEntity->posZ + lookZ, (float)idleEntity->getHorizontalFaceSpeed(), (float)idleEntity->getVerticalFaceSpeed());
}
