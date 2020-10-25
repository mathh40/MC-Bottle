#include "PhaseLandingApproach.h"

#include "PhaseList.h"
#include "../../EntityDragon.h"
#include "gen/feature/WorldGenEndPodium.h"

PhaseLandingApproach::PhaseLandingApproach(EntityDragon *dragonIn)
    :PhaseBase(dragonIn){
}

PhaseList * PhaseLandingApproach::getType() {
    return PhaseList::LANDING_APPROACH.get();
}

void PhaseLandingApproach::initPhase() {
    currentPath = std::nullopt;
    targetLocation = std::nullopt;
}

void PhaseLandingApproach::doLocalUpdate() {
    double d0 = targetLocation == std::nullopt ? 0.0 : targetLocation->squareDistanceTo(dragon->posX, dragon->posY, dragon->posZ);
      if (d0 < 100.0 || d0 > 22500.0 || dragon->collidedHorizontally || dragon->collidedVertically) {
         findNewTarget();
      }
}

std::optional<Vec3d> PhaseLandingApproach::getTargetLocation() {
    return targetLocation;
}

void PhaseLandingApproach::findNewTarget() {
    if (currentPath == std::nullopt || currentPath->isFinished()) {
         auto i = dragon->initPathPoints();
         BlockPos blockpos = dragon->world->getTopSolidOrLiquidBlock(WorldGenEndPodium::END_PODIUM_LOCATION);
         EntityPlayer* entityplayer = dragon->world->getNearestAttackablePlayer(blockpos, 128.0, 128.0);
         auto j = 0;
         if (entityplayer != nullptr) {
            Vec3d vec3d = (Vec3d(entityplayer->posX, 0.0, entityplayer->posZ)).normalize();
            j = dragon->getNearestPpIdx(-vec3d.getx() * 40.0, 105.0, -vec3d.getz() * 40.0);
         } else {
            j = dragon->getNearestPpIdx(40.0, (double)blockpos.gety(), 0.0);
         }

         PathPoint pathpoint = PathPoint(blockpos.getx(), blockpos.gety(), blockpos.getz());
         currentPath = dragon->findPath(i, j, pathpoint);
         if (currentPath != std::nullopt) {
            currentPath->incrementPathIndex();
         }
      }

      navigateToNextPathNode();
      if (currentPath != std::nullopt && currentPath->isFinished()) {
         dragon->getPhaseManager().setPhase(PhaseList::LANDING.get());
      }
}

void PhaseLandingApproach::navigateToNextPathNode() {
    if (currentPath != std::nullopt && !currentPath->isFinished()) {
         Vec3d vec3d = currentPath->getCurrentPos();
         currentPath->incrementPathIndex();
         double d0 = vec3d.getx();
         double d1 = vec3d.getz();

         double d2;
         do {
            d2 = vec3d.gety() + (double)(MathHelper::nextFloat(dragon->getRNG()) * 20.0F);
         } while(d2 < vec3d.gety());

         targetLocation = Vec3d(d0, d2, d1);
      }
}
