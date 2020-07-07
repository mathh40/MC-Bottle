#include "EntityAIFollow.h"



#include "EntityLookHelper.h"
#include "Util.h"
#include "../../pathfinding/PathNavigateFlying.h"
#include "../../pathfinding/PathNavigateGround.h"

EntityAIFollow::EntityAIFollow(EntityLiving *p_i47417_1_, double p_i47417_2_, float p_i47417_4_, float p_i47417_5_)
    :entity(p_i47417_1_),speedModifier(p_i47417_2_),navigation(p_i47417_1_->getNavigator())
    ,stopDistance(p_i47417_4_),areaSize(p_i47417_5_)
{
    followPredicate = [&](EntityLiving* p_apply_1_) -> bool
    {
        {
            return p_apply_1_ != nullptr && p_i47417_1_->getClass() != p_apply_1_->getClass();
        }
    };
    setMutexBits(3);
    if (!Util::instanceof<PathNavigateGround>(p_i47417_1_->getNavigator()) && !Util::instanceof<PathNavigateFlying>(p_i47417_1_->getNavigator()))
    {
        throw std::logic_error("Unsupported mob type for FollowMobGoal");
    }
}

bool EntityAIFollow::shouldExecute()
{
    std::vector<EntityLiving*> list = entity->world.getEntitiesWithinAABB(EntityLiving.class, entity->getEntityBoundingBox().grow(areaSize), followPredicate);
    if (!list.empty()) 
    {
        for(auto& entityliving : list)
        {
            if (!entityliving.isInvisible()) 
            {
                followingEntity = entityliving;
                return true;
            }
        }
    }

    return false;
}

bool EntityAIFollow::shouldContinueExecuting()
{
    return followingEntity != nullptr && !navigation->noPath() && entity->getDistanceSq(followingEntity) > (stopDistance * stopDistance);
}

void EntityAIFollow::startExecuting()
{
    timeToRecalcPath = 0;
    oldWaterCost = entity->getPathPriority(PathNodeType::WATER);
    entity->setPathPriority(PathNodeType::WATER, 0.0F);
}

void EntityAIFollow::resetTask()
{
    followingEntity = nullptr;
    navigation->clearPath();
    entity->setPathPriority(PathNodeType::WATER, oldWaterCost);
}

void EntityAIFollow::updateTask()
{
    if (followingEntity != nullptr && !entity->getLeashed()) 
    {
        entity->getLookHelper().setLookPositionWithEntity(followingEntity, 10.0F, entity->getVerticalFaceSpeed());
        if (--timeToRecalcPath <= 0) 
        {
            timeToRecalcPath = 10;
            double d0 = entity->posX - followingEntity->posX;
            double d1 = entity->posY - followingEntity->posY;
            double d2 = entity->posZ - followingEntity->posZ;
            double d3 = d0 * d0 + d1 * d1 + d2 * d2;
            if (d3 > stopDistance * stopDistance) 
            {
                navigation->tryMoveToEntityLiving(followingEntity, speedModifier);
            }
            else 
            {
                navigation->clearPath();
                EntityLookHelper entitylookhelper = followingEntity->getLookHelper();
                if (d3 <= stopDistance || entitylookhelper.getLookPosX() == entity->posX && entitylookhelper.getLookPosY() == entity->posY && entitylookhelper.getLookPosZ() == entity->posZ) 
                {
                    double d4 = followingEntity->posX - entity->posX;
                    double d5 = followingEntity->posZ - entity->posZ;
                    navigation->tryMoveToXYZ(entity->posX - d4, entity->posY, entity->posZ - d5, speedModifier);
                }
            }
        }
      }
}
