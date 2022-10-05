#include "EntityAIDoorInteract.h"

#include "../../pathfinding/PathNavigateGround.h"
#include "Util.h"

EntityAIDoorInteract::EntityAIDoorInteract(EntityLiving *entityIn) : doorPosition(BlockPos::ORIGIN), entity(entityIn)
{
    if (!Util:: instanceof <PathNavigateGround>(entityIn->getNavigator()))
    {
        throw std::logic_error("Unsupported mob type for DoorInteractGoal");
    }
}

bool EntityAIDoorInteract::shouldExecute()
{
    if (!entity->collidedHorizontally)
    {
        return false;
    }
    else
    {
        PathNavigateGround pathnavigateground = entity->getNavigator();
        auto path                             = pathnavigateground.getPath();
        if (path.has_value() && !path->isFinished() && pathnavigateground.getEnterDoors())
        {
            for (auto i = 0; i < MathHelper::min(path->getCurrentPathIndex() + 2, path->getCurrentPathLength()); ++i)
            {
                const PathPoint pathpoint = path->getPathPointFromIndex(i);
                doorPosition              = BlockPos(pathpoint.x, pathpoint.y + 1, pathpoint.z);
                if (entity->getDistanceSq(doorPosition.getx(), entity->posY, doorPosition.getz()) <= 2.25)
                {
                    doorBlock = getBlockDoor(doorPosition);
                    if (doorBlock != nullptr)
                    {
                        return true;
                    }
                }
            }

            doorPosition = BlockPos(entity).up();
            doorBlock    = getBlockDoor(doorPosition);
            return doorBlock != nullptr;
        }
        else
        {
            return false;
        }
    }
}

bool EntityAIDoorInteract::shouldContinueExecuting()
{
    return !hasStoppedDoorInteraction;
}

void EntityAIDoorInteract::startExecuting()
{
    hasStoppedDoorInteraction = false;
    entityPositionX           = doorPosition.getx() + 0.5F - entity->posX;
    entityPositionZ           = doorPosition.getz() + 0.5F - entity->posZ;
}

void EntityAIDoorInteract::updateTask()
{
    float f  = doorPosition.getx() + 0.5F - entity->posX;
    float f1 = doorPosition.getz() + 0.5F - entity->posZ;
    float f2 = entityPositionX * f + entityPositionZ * f1;
    if (f2 < 0.0F)
    {
        hasStoppedDoorInteraction = true;
    }
}

BlockDoor *EntityAIDoorInteract::getBlockDoor(BlockPos pos)
{
    IBlockState *iblockstate = entity->world.getBlockState(pos);
    Block *block             = iblockstate->getBlock();
    return Util:: instanceof
        <BlockDoor>(block) && iblockstate->getMaterial() == Material::WOOD ? (BlockDoor *)block : nullptr;
}
