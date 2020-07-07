#include "EntityAIFleeSun.h"



#include "../../../../../pcg-cpp/pcg_random.hpp"
#include "../../inventory/EntityEquipmentSlot.h"
#include "math/BlockPos.h"
#include "math/Vec3d.h"

EntityAIFleeSun::EntityAIFleeSun(EntityCreature *theCreatureIn, double movementSpeedIn)
    :creature(theCreatureIn),movementSpeed(movementSpeedIn),world(theCreatureIn->world)
{
    setMutexBits(1);
}

bool EntityAIFleeSun::shouldExecute()
{
    if (!world->isDaytime()) 
    {
        return false;
    }
    else if (!creature->isBurning()) 
    {
        return false;
    }
    else if (!world->canSeeSky(BlockPos(creature->posX, creature->getEntityBoundingBox().minY, creature->posZ))) 
    {
        return false;
    }
    else if (!creature->getItemStackFromSlot(EntityEquipmentSlot::HEAD).isEmpty()) 
    {
        return false;
    }
    else 
    {
        Vec3d vec3d = findPossibleShelter();
        if (vec3d == nullptr) 
        {
            return false;
        }
        else 
        {
            shelterX = vec3d.getx();
            shelterY = vec3d.gety();
            shelterZ = vec3d.getz();
            return true;
        }
    }
}

bool EntityAIFleeSun::shouldContinueExecuting()
{
    return !creature->getNavigator().noPath();
}

void EntityAIFleeSun::startExecuting()
{
    creature->getNavigator().tryMoveToXYZ(shelterX, shelterY, shelterZ, movementSpeed);
}

std::optional<Vec3d> EntityAIFleeSun::findPossibleShelter()
{
    pcg32 random = creature->getRNG();
    BlockPos blockpos = BlockPos(creature->posX, creature->getEntityBoundingBox().minY, creature->posZ);

    for(int i = 0; i < 10; ++i) 
    {
        BlockPos blockpos1 = blockpos.add(random(20) - 10, random(6) - 3, random(20) - 10);
        if (!world->canSeeSky(blockpos1) && creature->getBlockPathWeight(blockpos1) < 0.0F) 
        {
            return Vec3d(blockpos1.getx(), blockpos1.gety(), blockpos1.getz());
        }
    }

    return std::nullopt;
}
