#include "EntityAIMoveThroughVillage.h"

#include "RandomPositionGenerator.h"
#include "Util.h"
#include "../../pathfinding/PathNavigateGround.h"
#include "../../village/Village.h"

EntityAIMoveThroughVillage::EntityAIMoveThroughVillage(EntityCreature *entityIn, double movementSpeedIn,bool isNocturnalIn)
    :entity(entityIn),movementSpeed(movementSpeedIn),isNocturnal(isNocturnalIn)
{
    setMutexBits(1);
    if (!(Util::instanceof<PathNavigateGround>(entityIn->getNavigator()))) 
    {
        throw std::logic_error("Unsupported mob for MoveThroughVillageGoal");
    }
}

bool EntityAIMoveThroughVillage::shouldExecute()
{
    resizeDoorList();
    if (isNocturnal && entity->world.isDaytime()) 
    {
        return false;
    }
    else 
    {
        std::optional<Village> village = entity->world.getVillageCollection().getNearestVillage(BlockPos(entity), 0);
        if (!village.has_value()) 
        {
            return false;
        }
        else 
        {
            doorInfo = findNearestDoor(village);
            if (!doorInfo) 
            {
                return false;
            }
            else
            {
                PathNavigateGround pathnavigateground = (PathNavigateGround)entity->getNavigator();
                bool flag = pathnavigateground.getEnterDoors();
                pathnavigateground.setBreakDoors(false);
                path = pathnavigateground.getPathToPos(doorInfo.getDoorBlockPos());
                pathnavigateground.setBreakDoors(flag);
                if (path) 
                {
                    return true;
                }
                else 
                {
                    auto vec3d = RandomPositionGenerator::findRandomTargetBlockTowards(entity, 10, 7, Vec3d((double)doorInfo->getDoorBlockPos().getx(), (double)doorInfo->getDoorBlockPos().gety(), (double)doorInfo->getDoorBlockPos().getz()));
                    if (!vec3d) 
                    {
                        return false;
                    }
                    else 
                    {
                        pathnavigateground.setBreakDoors(false);
                        path = entity->getNavigator().getPathToXYZ(vec3d.x, vec3d.y, vec3d.z);
                        pathnavigateground.setBreakDoors(flag);
                        return path.has_value();
                    }
                }
            }
        }
    }
}

bool EntityAIMoveThroughVillage::shouldContinueExecuting()
{
    if (entity->getNavigator().noPath()) 
    {
         return false;
    }
    else 
    {
        float f = entity->width + 4.0F;
        return entity->getDistanceSq(doorInfo->getDoorBlockPos()) > (double)(f * f);
    }
}

void EntityAIMoveThroughVillage::startExecuting()
{
    entity->getNavigator().setPath(path, movementSpeed);
}

void EntityAIMoveThroughVillage::resetTask()
{
    if (entity->getNavigator().noPath() || entity->getDistanceSq(doorInfo->getDoorBlockPos()) < 16.0) 
    {
        doorList.add(doorInfo);
    }
}

std::optional<VillageDoorInfo> EntityAIMoveThroughVillage::findNearestDoor(const Village &villageIn)
{
    std::optional<VillageDoorInfo> villagedoorinfo;
    auto i = std::numeric_limits<int32_t>::max();

    for(auto villagedoorinfo1 : villageIn.getVillageDoorInfoList())
    {
        int32_t j = villagedoorinfo1.getDistanceSquared(MathHelper::floor(entity->posX), MathHelper::floor(entity->posY), MathHelper::floor(entity->posZ));
        if (j < i && !doesDoorListContain(villagedoorinfo1)) 
        {
            villagedoorinfo = villagedoorinfo1;
            i = j;
        }
    }

    return villagedoorinfo;
}

bool EntityAIMoveThroughVillage::doesDoorListContain(VillageDoorInfo doorInfoIn)
{
    Iterator var2 = doorList;

    VillageDoorInfo villagedoorinfo;
    do 
    {
        if (!var2.hasNext()) 
        {
            return false;
        }

        villagedoorinfo = (VillageDoorInfo)var2.next();
    }
    while(!doorInfoIn.getDoorBlockPos().equals(villagedoorinfo.getDoorBlockPos()));

    return true;
}

void EntityAIMoveThroughVillage::resizeDoorList()
{
    if (doorList.size() > 15) 
    {
        doorList.erase(doorList.begin());
    }
}
