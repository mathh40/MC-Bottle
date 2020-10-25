#include "PhaseStrafePlayer.h"

#include "../../EntityDragon.h"

std::shared_ptr<spdlog::logger> PhaseStrafePlayer::LOGGER = spdlog::get("Minecraft")->clone("PhaseStrafePlayer");

PhaseStrafePlayer::PhaseStrafePlayer(EntityDragon *dragonIn)
    :PhaseBase(dragonIn){

}

void PhaseStrafePlayer::doLocalUpdate() {
    if (attackTarget == nullptr) {
         LOGGER->warn("Skipping player strafe phase because no player was found");
         dragon->getPhaseManager().setPhase(PhaseList::HOLDING_PATTERN.get());
      } else {
         double d12;
         double d13;
         double d14;
         if (currentPath.has_value() && currentPath->isFinished()) {
            d12 = attackTarget->posX;
            d13 = attackTarget->posZ;
            double d2 = d12 - dragon->posX;
            double d3 = d13 - dragon->posZ;
            d14 = (double)MathHelper::sqrt(d2 * d2 + d3 * d3);
            double d5 = MathHelper::min(0.4000000059604645 + d14 / 80.0 - 1.0, 10.0);
            targetLocation = Vec3d(d12, attackTarget->posY + d5, d13);
         }

         d12 = targetLocation == std::nullopt ? 0.0 : targetLocation->squareDistanceTo(dragon->posX, dragon->posY, dragon->posZ);
         if (d12 < 100.0 || d12 > 22500.0) {
            findNewTarget();
         }

         d13 = 64.0;
         if (attackTarget->getDistanceSq(dragon) < 4096.0) {
            if (dragon->canEntityBeSeen(attackTarget)) {
               ++fireballCharge;
               Vec3d vec3d1 = (Vec3d(attackTarget->posX - dragon->posX, 0.0, attackTarget->posZ - dragon->posZ)).normalize();
               Vec3d vec3d = (Vec3d((double)MathHelper::sin(dragon->rotationYaw * 0.017453292F), 0.0, (double)(-MathHelper::cos(dragon->rotationYaw * 0.017453292F)))).normalize();
               float f1 = (float)vec3d.dotProduct(vec3d1);
               float f = (float)(MathHelper::acos((double)f1) * 57.29577951308232);
               f += 0.5F;
               if (fireballCharge >= 5 && f >= 0.0F && f < 10.0F) {
                  d14 = 1.0;
                  Vec3d vec3d2 = dragon->getLook(1.0F);
                  double d6 = dragon->dragonPartHead.posX - vec3d2.getx() * 1.0;
                  double d7 = dragon->dragonPartHead.posY + (double)(dragon->dragonPartHead.height / 2.0F) + 0.5;
                  double d8 = dragon->dragonPartHead.posZ - vec3d2.getz() * 1.0;
                  double d9 = attackTarget->posX - d6;
                  double d10 = attackTarget->posY + (double)(attackTarget->height / 2.0F) - (d7 + (double)(dragon->dragonPartHead.height / 2.0F));
                  double d11 = attackTarget->posZ - d8;
                  dragon->world->playEvent(nullptr, 1017, BlockPos(dragon), 0);
                  EntityDragonFireball* entitydragonfireball = new EntityDragonFireball(dragon->world, dragon, d9, d10, d11);
                  entitydragonfireball->setLocationAndAngles(d6, d7, d8, 0.0F, 0.0F);
                  dragon->world->spawnEntity(entitydragonfireball);
                  fireballCharge = 0;
                  if (currentPath.has_value()) {
                     while(!currentPath->isFinished()) {
                        currentPath->incrementPathIndex();
                     }
                  }

                  dragon->getPhaseManager().setPhase(PhaseList::HOLDING_PATTERN.get());
               }
            } else if (fireballCharge > 0) {
               --fireballCharge;
            }
         } else if (fireballCharge > 0) {
            --fireballCharge;
         }
      }
}

void PhaseStrafePlayer::initPhase() {
    fireballCharge = 0;
    targetLocation = std::nullopt;
    currentPath = std::nullopt;
    attackTarget = nullptr;
}

void PhaseStrafePlayer::setTarget(EntityLivingBase *p_188686_1_) {
    attackTarget = p_188686_1_;
      auto i = dragon->initPathPoints();
      auto j = dragon->getNearestPpIdx(attackTarget->posX, attackTarget->posY, attackTarget->posZ);
      auto k = MathHelper::floor(attackTarget->posX);
      auto l = MathHelper::floor(attackTarget->posZ);
      auto d0 = (double)k - dragon->posX;
      auto d1 = (double)l - dragon->posZ;
      auto d2 = (double)MathHelper::sqrt(d0 * d0 + d1 * d1);
      auto d3 = MathHelper::min(0.4000000059604645 + d2 / 80.0 - 1.0, 10.0);
      auto i1 = MathHelper::floor(attackTarget->posY + d3);
      PathPoint pathpoint = PathPoint(k, i1, l);
      currentPath = dragon->findPath(i, j, pathpoint);
      if (currentPath.has_value()) {
         currentPath->incrementPathIndex();
         navigateToNextPathNode();
      }
}

std::optional<Vec3d> PhaseStrafePlayer::getTargetLocation() {
    return targetLocation;
}

PhaseList * PhaseStrafePlayer::getType() {
    return PhaseList::STRAFE_PLAYER.get();
}

void PhaseStrafePlayer::findNewTarget() {
    if (currentPath == std::nullopt || currentPath->isFinished()) {
         auto i = dragon->initPathPoints();
         auto j = i;
         if (dragon->getRNG()(8) == 0) {
            holdingPatternClockwise = !holdingPatternClockwise;
            j = i + 6;
         }

         if (holdingPatternClockwise) {
            ++j;
         } else {
            --j;
         }

         if (dragon->getFightManager().has_value() && dragon->getFightManager()->getNumAliveCrystals() > 0) {
            j %= 12;
            if (j < 0) {
               j += 12;
            }
         } else {
            j -= 12;
            j &= 7;
            j += 12;
         }

         currentPath = dragon->findPath(i, j, std::nullopt);
         if (currentPath.has_value()) {
            currentPath->incrementPathIndex();
         }
      }

      navigateToNextPathNode();
}

void PhaseStrafePlayer::navigateToNextPathNode() {
    if (currentPath.has_value() && !currentPath->isFinished()) {
         Vec3d vec3d = currentPath->getCurrentPos();
         currentPath->incrementPathIndex();
         double d0 = vec3d.getx();
         double d2 = vec3d.getz();

         double d1;
         do {
            d1 = vec3d.gety() + (double)(MathHelper::nextFloat(dragon->getRNG()) * 20.0F);
         } while(d1 < vec3d.gety());

         targetLocation = Vec3d(d0, d1, d2);
      }
}
