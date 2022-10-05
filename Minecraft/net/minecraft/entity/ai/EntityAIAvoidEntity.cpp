#include "EntityAIAvoidEntity.h"

#include "../../pathfinding/PathNavigate.h"
#include "EntitySelectors.h"
#include "RandomPositionGenerator.h"
#include "math/Vec3d.h"

auto alwaysTrue = [](Entity *p_apply_1_) -> bool { return true; };

EntityAIAvoidEntity::EntityAIAvoidEntity(EntityCreature *entityIn, std::type_index classToAvoidIn,
                                         float avoidDistanceIn, double farSpeedIn, double nearSpeedIn)
    : EntityAIAvoidEntity(entityIn, classToAvoidIn, alwaysTrue, avoidDistanceIn, farSpeedIn, nearSpeedIn)
{
}

bool EntityAIAvoidEntity::shouldExecute()
{
    auto list = entity->world->getEntitiesWithinAABB(
        classToAvoid, entity->getEntityBoundingBox().grow(avoidDistance, 3.0, avoidDistance),
        {EntitySelectors::CAN_AI_TARGET, canBeSeenSelector, avoidTargetSelector});
    if (list.isEmpty())
    {
        return false;
    }
    else
    {
        closestLivingEntity = list.get(0);
        auto vec3d          = RandomPositionGenerator::findRandomTargetBlockAwayFrom(
                     entity, 16, 7, Vec3d(closestLivingEntity->posX, closestLivingEntity->posY, closestLivingEntity->posZ));
        if (!vec3d.has_value())
        {
            return false;
        }
        else if (closestLivingEntity->getDistanceSq(vec3d->getx(), vec3d->gety(), vec3d->getz()) <
                 closestLivingEntity->getDistanceSq((Entity)entity))
        {
            return false;
        }
        else
        {
            path = navigation->getPathToXYZ(vec3d->getx(), vec3d->gety(), vec3d->getz());
            return path != nullptr;
        }
    }
}

bool EntityAIAvoidEntity::shouldContinueExecuting()
{
    return !navigation->noPath();
}

void EntityAIAvoidEntity::startExecuting()
{
    navigation->setPath(path, farSpeed);
}

void EntityAIAvoidEntity::resetTask()
{
    closestLivingEntity = nullptr;
}

void EntityAIAvoidEntity::updateTask()
{
    if (entity->getDistanceSq(closestLivingEntity) < 49.0)
    {
        entity->getNavigator().setSpeed(nearSpeed);
    }
    else
    {
        entity->getNavigator().setSpeed(farSpeed);
    }
}

EntityAIAvoidEntity::EntityAIAvoidEntity(EntityCreature *entityIn, std::type_index classToAvoidIn,
                                         std::function<bool(Entity p_apply_1_ *)> avoidTargetSelectorIn,
                                         float avoidDistanceIn, double farSpeedIn, double nearSpeedIn)
    : canBeSeenSelector([&](Entity *p_apply_1_) -> bool {
          return p_apply_1_->isEntityAlive() && entity->getEntitySenses().canSee(p_apply_1_) &&
                 !entity->isOnSameTeam(p_apply_1_);
      }),
      entity(entityIn), classToAvoid(classToAvoidIn), avoidTargetSelector(avoidTargetSelectorIn),
      avoidDistance(avoidDistanceIn), farSpeed(farSpeedIn), nearSpeed(nearSpeedIn), navigation(entityIn->getNavigator())
{
    setMutexBits(1);
}
