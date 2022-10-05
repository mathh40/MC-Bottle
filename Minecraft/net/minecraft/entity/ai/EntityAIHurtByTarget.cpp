#include "EntityAIHurtByTarget.h"

#include "Util.h"
#include "math/AxisAlignedBB.h"

EntityAIHurtByTarget::EntityAIHurtByTarget(EntityCreature *creatureIn, bool entityCallsForHelpIn,
                                           std::initializer_list<std::type_index> excludedReinforcementTypes)
    : EntityAITarget(creatureIn, true), entityCallsForHelp(entityCallsForHelpIn),
      excludedReinforcementTypes(excludedReinforcementTypes)
{
    setMutexBits(1);
}

bool EntityAIHurtByTarget::shouldExecute()
{
    int32_t i                          = taskOwner->getRevengeTimer();
    EntityLivingBase *entitylivingbase = taskOwner->getRevengeTarget();
    return i != revengeTimerOld && entitylivingbase != nullptr && isSuitableTarget(entitylivingbase, false);
}

void EntityAIHurtByTarget::startExecuting()
{
    taskOwner->setAttackTarget(taskOwner->getRevengeTarget());
    target            = taskOwner->getAttackTarget();
    revengeTimerOld   = taskOwner->getRevengeTimer();
    unseenMemoryTicks = 300;
    if (entityCallsForHelp)
    {
        alertOthers();
    }

    EntityAITarget::startExecuting();
}

void EntityAIHurtByTarget::alertOthers()
{
    double d0 = getTargetDistance();
    auto var3 = taskOwner->world
                    ->getEntitiesWithinAABB(
                        taskOwner->getClass(),
                        (AxisAlignedBB(taskOwner->posX, taskOwner->posY, taskOwner->posZ, taskOwner->posX + 1.0D,
                                       taskOwner->posY + 1.0D, taskOwner->posZ + 1.0))
                            .grow(d0, 10.0, d0))
                    .iterator();

    while (true)
    {
        EntityCreature *entitycreature;
        do
        {
            do
            {
                do
                {
                    do
                    {
                        if (!var3.hasNext())
                        {
                            return;
                        }

                        entitycreature = (EntityCreature)var3.next();
                    } while (taskOwner == entitycreature);
                } while (entitycreature->getAttackTarget() != nullptr);
            } while (Util:: instanceof <EntityTameable>(taskOwner) &&
                                           ((EntityTameable *)taskOwner)->getOwner() !=
                                               ((EntityTameable *)entitycreature)->getOwner());
        } while (entitycreature->isOnSameTeam(taskOwner->getRevengeTarget()));

        bool flag    = false;
        Class[] var6 = excludedReinforcementTypes;
        int var7     = var6.length;

        for (int var8 = 0; var8 < var7; ++var8)
        {
            Class oclass = var6[var8];
            if (entitycreature->getClass() == oclass)
            {
                flag = true;
                break;
            }
        }

        if (!flag)
        {
            setEntityAttackTarget(entitycreature, taskOwner->getRevengeTarget());
        }
    }
}

void EntityAIHurtByTarget::setEntityAttackTarget(EntityCreature *creatureIn, EntityLivingBase *entityLivingBaseIn)
{
    creatureIn->setAttackTarget(entityLivingBaseIn);
}
