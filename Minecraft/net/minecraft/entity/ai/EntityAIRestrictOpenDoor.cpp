#include "EntityAIRestrictOpenDoor.h"

#include "../../pathfinding/PathNavigateGround.h"
#include "../../village/Village.h"
#include "Util.h"

EntityAIRestrictOpenDoor::EntityAIRestrictOpenDoor(EntityCreature *creatureIn) : entity(creatureIn)
{
    if (!Util:: instanceof <PathNavigateGround>(creatureIn->getNavigator()))
    {
        throw std::logic_error("Unsupported mob type for RestrictOpenDoorGoal");
    }
}

bool EntityAIRestrictOpenDoor::shouldExecute()
{
    if (entity->world.isDaytime())
    {
        return false;
    }
    else
    {
        BlockPos blockpos(entity);
        Village village = entity->world->getVillageCollection().getNearestVillage(blockpos, 16);
        if (village == nullptr)
        {
            return false;
        }
        else
        {
            frontDoor = village.getNearestDoor(blockpos);
            if (!frontDoor)
            {
                return false;
            }
            else
            {
                return frontDoor->getDistanceToInsideBlockSq(blockpos) < 2.25;
            }
        }
    }
}

bool EntityAIRestrictOpenDoor::shouldContinueExecuting()
{
    if (entity->world.isDaytime())
    {
        return false;
    }
    else
    {
        return !frontDoor->getIsDetachedFromVillageFlag() && frontDoor->isInsideSide(BlockPos(entity));
    }
}

void EntityAIRestrictOpenDoor::startExecuting()
{
    ((PathNavigateGround)entity->getNavigator())->setBreakDoors(false);
    ((PathNavigateGround)entity->getNavigator())->setEnterDoors(false);
}

void EntityAIRestrictOpenDoor::resetTask()
{
    ((PathNavigateGround)entity->getNavigator())->setBreakDoors(true);
    ((PathNavigateGround)entity->getNavigator())->setEnterDoors(true);
    frontDoor = nullptr;
}

void EntityAIRestrictOpenDoor::updateTask()
{
    frontDoor->incrementDoorOpeningRestrictionCounter();
}
