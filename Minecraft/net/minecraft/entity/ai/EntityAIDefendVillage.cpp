#include "EntityAIDefendVillage.h"

#include "../../village/Village.h"

EntityAIDefendVillage::EntityAIDefendVillage(EntityIronGolem *ironGolemIn)
    : EntityAITarget(ironGolemIn, false, true), irongolem(ironGolemIn)
{
    setMutexBits(1);
}

bool EntityAIDefendVillage::shouldExecute()
{
    Village *village = irongolem->getVillage();
    if (village == nullptr)
    {
        return false;
    }
    else
    {
        villageAgressorTarget = village->findNearestVillageAggressor(irongolem);
        if (Util:: instanceof <EntityCreeper>(villageAgressorTarget))
        {
            return false;
        }
        else if (isSuitableTarget(villageAgressorTarget, false))
        {
            return true;
        }
        else if (taskOwner->getRNG().nextInt(20) == 0)
        {
            villageAgressorTarget = village.getNearestTargetPlayer(irongolem);
            return isSuitableTarget(villageAgressorTarget, false);
        }
        else
        {
            return false;
        }
    }
}

void EntityAIDefendVillage::startExecuting()
{
    irongolem->setAttackTarget(villageAgressorTarget);
    EntityAITarget::startExecuting();
}
