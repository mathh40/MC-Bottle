#include "PhaseHoldingPattern.h"

#include "PhaseList.h"
#include "../../EntityDragon.h"
#include "gen/feature/WorldGenEndPodium.h"

PhaseHoldingPattern::PhaseHoldingPattern(EntityDragon *dragonIn)
    :PhaseBase(dragonIn){
}

PhaseList * PhaseHoldingPattern::getType() {
    return PhaseList::HOLDING_PATTERN.get();
}

void PhaseHoldingPattern::doLocalUpdate() {
    double d0 = targetLocation == std::nullopt ? 0.0 : targetLocation->squareDistanceTo(dragon->posX, dragon->posY, dragon->posZ);
    if (d0 < 100.0 || d0 > 22500.0 || dragon->collidedHorizontally || dragon->collidedVertically) {
        findNewTarget();
    }
}

void PhaseHoldingPattern::initPhase() {
    currentPath = std::nullopt;
    targetLocation = std::nullopt;
}

std::optional<Vec3d> PhaseHoldingPattern::getTargetLocation() {
    return targetLocation;
}

void PhaseHoldingPattern::findNewTarget() {
    int32_t k = 0;
      if (currentPath.has_value() && currentPath->isFinished()) {
         BlockPos blockpos = dragon->world->getTopSolidOrLiquidBlock(BlockPos(WorldGenEndPodium::END_PODIUM_LOCATION));
         k = dragon->getFightManager() == std::nullopt ? 0 : dragon->getFightManager()->getNumAliveCrystals();
         if (dragon->getRNG()(k + 3) == 0) {
            dragon->getPhaseManager().setPhase(PhaseList::LANDING_APPROACH.get());
            return;
         }

         double d0 = 64.0;
         EntityPlayer* entityplayer = dragon->world->getNearestAttackablePlayer(blockpos, d0, d0);
         if (entityplayer != nullptr) {
            d0 = entityplayer->getDistanceSqToCenter(blockpos) / 512.0;
         }

         if (entityplayer != nullptr && (dragon->getRNG()(MathHelper::abs(d0) + 2) == 0 || dragon->getRNG()(k + 2) == 0)) {
            strafePlayer(entityplayer);
            return;
         }
      }

      if (currentPath == std::nullopt || currentPath->isFinished()) {
         auto j = dragon->initPathPoints();
         k = j;
         if (dragon->getRNG()(8) == 0) {
            clockwise = !clockwise;
            k = j + 6;
         }

         if (clockwise) {
            ++k;
         } else {
            --k;
         }

         if (dragon->getFightManager() != std::nullopt && dragon->getFightManager()->getNumAliveCrystals() >= 0) {
            k %= 12;
            if (k < 0) {
               k += 12;
            }
         } else {
            k -= 12;
            k &= 7;
            k += 12;
         }

         currentPath = dragon->findPath(j, k, std::nullopt);
         if (currentPath != std::nullopt) {
            currentPath->incrementPathIndex();
         }
      }

      navigateToNextPathNode();
}

void PhaseHoldingPattern::strafePlayer(EntityPlayer *player) {
    dragon->getPhaseManager().setPhase(PhaseList::STRAFE_PLAYER.get());
    ((PhaseStrafePlayer)dragon->getPhaseManager().getPhase(PhaseList::STRAFE_PLAYER.get())).setTarget(player);
}
