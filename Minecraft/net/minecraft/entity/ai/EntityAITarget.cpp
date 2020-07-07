#include "EntityAITarget.h"


#include "EntityAIBeg.h"
#include "Util.h"
#include "../../pathfinding/Path.h"
#include "../../pathfinding/PathPoint.h"
#include "../../scoreboard/Team.h"
#include "attributes/IAttributeInstance.h"
#include "biome/Biome.h"
#include "math/BlockPos.h"

EntityAITarget::EntityAITarget(EntityCreature *creature, bool checkSight)
    :EntityAITarget(creature, checkSight, false)
{
}

EntityAITarget::EntityAITarget(EntityCreature *creature, bool checkSight, bool onlyNearby)
    :unseenMemoryTicks(60),taskOwner(creature),shouldCheckSight(checkSight),nearbyOnly(onlyNearby)
{

}

bool EntityAITarget::shouldContinueExecuting()
{
    EntityLivingBase* entitylivingbase = taskOwner->getAttackTarget();
    if (entitylivingbase == nullptr) 
    {
        entitylivingbase = target;
    }

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
        Team* team = taskOwner->getTeam();
        Team* team1 = entitylivingbase->getTeam();
        if (team != nullptr && team1 == team) 
        {
            return false;
        }
        else 
        {
            double d0 = getTargetDistance();
            if (taskOwner->getDistanceSq(entitylivingbase) > d0 * d0) 
            {
                return false;
            }
            else 
            {
                if (shouldCheckSight) 
                {
                    if (taskOwner->getEntitySenses().canSee(entitylivingbase)) 
                    {
                        targetUnseenTicks = 0;
                    }
                    else if (++targetUnseenTicks > unseenMemoryTicks) 
                    {
                        return false;
                    }
                }

                if (Util::instanceof<EntityPlayer>(entitylivingbase) && ((EntityPlayer*)entitylivingbase)->capabilities.disableDamage) 
                {
                    return false;
                }
                else 
                {
                    taskOwner->setAttackTarget(entitylivingbase);
                    return true;
                }
            }
        }
    }
}

void EntityAITarget::startExecuting()
{
    targetSearchStatus = 0;
    targetSearchDelay = 0;
    targetUnseenTicks = 0;
}

void EntityAITarget::resetTask()
{
    taskOwner->setAttackTarget(nullptr);
    target = nullptr;
}

bool EntityAITarget::isSuitableTarget(EntityLiving *attacker, EntityLivingBase *target, bool includeInvincibles,
    bool checkSight)
{
    if (target == nullptr) 
    {
        return false;
    }
    else if (target == attacker) 
    {
        return false;
    }
    else if (!target->isEntityAlive()) 
    {
        return false;
    }
    else if (!attacker->canAttackClass(target->getClass())) 
    {
        return false;
    }
    else if (attacker->isOnSameTeam(target)) 
    {
        return false;
    }
    else 
    {
        if (Util::instanceof<IEntityOwnable>(attacker) && ((IEntityOwnable*)attacker)->getOwnerId() != nullptr) 
        {
            if (Util::instanceof<IEntityOwnable>(target) && ((IEntityOwnable*)attacker)->getOwnerId().equals(((IEntityOwnable*)target)->getOwnerId())) 
            {
                return false;
            }

            if (target == ((IEntityOwnable*)attacker)->getOwner()) 
            {
                return false;
            }
        }
        else if (Util::instanceof<EntityPlayer>(target) && !includeInvincibles && ((EntityPlayer*)target)->capabilities.disableDamage) 
        {
            return false;
        }

        return !checkSight || attacker->getEntitySenses().canSee(target);
    }
}

EntityAITarget * EntityAITarget::setUnseenMemoryTicks(int32_t p_190882_1_)
{
    unseenMemoryTicks = p_190882_1_;
    return this;
}

double EntityAITarget::getTargetDistance()
{
    IAttributeInstance* iattributeinstance = taskOwner->getEntityAttribute(SharedMonsterAttributes::FOLLOW_RANGE);
    return iattributeinstance == nullptr ? 16.0 : iattributeinstance.getAttributeValue();
}

bool EntityAITarget::isSuitableTarget(EntityLivingBase *target, bool includeInvincibles)
{
    if (!isSuitableTarget(taskOwner, target, includeInvincibles, shouldCheckSight)) 
    {
        return false;
    }
    else if (!taskOwner->isWithinHomeDistanceFromPosition(BlockPos(target))) 
    {
        return false;
    }
    else 
    {
        if (nearbyOnly) 
        {
            if (--targetSearchDelay <= 0) 
            {
                targetSearchStatus = 0;
            }

            if (targetSearchStatus == 0) 
            {
                targetSearchStatus = canEasilyReach(target) ? 1 : 2;
            }

            if (targetSearchStatus == 2) 
            {
                return false;
            }
        }

        return true;
    }
}

bool EntityAITarget::canEasilyReach(EntityLivingBase *target)
{
    targetSearchDelay = 10 + taskOwner->getRNG().nextInt(5);
    auto path = taskOwner->getNavigator().getPathToEntityLiving(target);
    if (!path.has_value()) 
    {
        return false;
    }
    else 
    {
        PathPoint pathpoint = path->getFinalPathPoint();
        if (pathpoint == nullptr) 
        {
            return false;
        }
        else 
        {
            auto i = pathpoint.x - MathHelper::floor(target->posX);
            auto j = pathpoint.z - MathHelper::floor(target->posZ);
            return i * i + j * j <= 2.25;
        }
    }
}

