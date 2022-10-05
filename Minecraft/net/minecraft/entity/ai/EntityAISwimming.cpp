#include "EntityAISwimming.h"

#include "../../pathfinding/PathNavigateFlying.h"
#include "../../pathfinding/PathNavigateGround.h"
#include "Util.h"

EntityAISwimming::EntityAISwimming(EntityLiving *entityIn) : entity(entityIn)
{
    setMutexBits(4);
    if (Util:: instanceof <PathNavigateGround>(entityIn->getNavigator()))
    {
        ((PathNavigateGround)entityIn->getNavigator())->setCanSwim(true);
    }
    else if (Util:: instanceof <PathNavigateFlying>(entityIn->getNavigator()))
    {
        ((PathNavigateFlying)entityIn->getNavigator()).setCanFloat(true);
    }
}

bool EntityAISwimming::shouldExecute()
{
    return entity->isInWater() || entity->isInLava();
}

void EntityAISwimming::updateTask()
{
    if (entity->getRNG().nextFloat() < 0.8F)
    {
        entity->getJumpHelper().setJumping();
    }
}
