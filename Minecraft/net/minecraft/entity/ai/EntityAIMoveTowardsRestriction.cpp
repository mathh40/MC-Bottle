#include "EntityAIMoveTowardsRestriction.h"



#include "EntityCreature.h"
#include "RandomPositionGenerator.h"
#include "math/BlockPos.h"
#include "math/Vec3d.h"

EntityAIMoveTowardsRestriction::EntityAIMoveTowardsRestriction(EntityCreature *creatureIn, double speedIn)
    :creature(creatureIn),movementSpeed(speedIn){
    setMutexBits(1);
}

bool EntityAIMoveTowardsRestriction::shouldExecute() {
    if (creature->isWithinHomeDistanceCurrentPosition()) {
         return false;
      } else {
         BlockPos blockpos = creature->getHomePosition();
         auto vec3d = RandomPositionGenerator::findRandomTargetBlockTowards(creature, 16, 7, Vec3d((double)blockpos.getx(), (double)blockpos.gety(), (double)blockpos.getz()));
         if (!vec3d.has_value()) {
            return false;
         } else {
            movePosX = vec3d->getx();
            movePosY = vec3d->gety();
            movePosZ = vec3d->getz();
            return true;
         }
      }
}

bool EntityAIMoveTowardsRestriction::shouldContinueExecuting() {
    return !creature->getNavigator()->noPath();
}

void EntityAIMoveTowardsRestriction::startExecuting() {
    creature->getNavigator()->tryMoveToXYZ(movePosX, movePosY, movePosZ, movementSpeed);
}
