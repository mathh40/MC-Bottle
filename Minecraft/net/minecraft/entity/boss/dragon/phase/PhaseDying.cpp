#include "PhaseDying.h"

#include "../../EntityDragon.h"
#include "gen/feature/WorldGenEndPodium.h"

PhaseDying::PhaseDying(EntityDragon *dragonIn)
    :PhaseBase(dragonIn){
}

void PhaseDying::doClientRenderEffects() {
    if (time++ % 10 == 0) {
        float f = (MathHelper::nextFloat(dragon->getRNG()) - 0.5F) * 8.0F;
        float f1 = (MathHelper::nextFloat(dragon->getRNG()) - 0.5F) * 4.0F;
        float f2 = (MathHelper::nextFloat(dragon->getRNG()) - 0.5F) * 8.0F;
        dragon->world->spawnParticle(EnumParticleTypes::EXPLOSION_HUGE, dragon->posX + (double)f, dragon->posY + 2.0 + (double)f1, dragon->posZ + (double)f2, 0.0, 0.0, 0.0,{});
    }
}

void PhaseDying::doLocalUpdate() {
    ++time;
      if (!targetLocation.has_value()) {
          const BlockPos blockpos = dragon->world->getHeight(WorldGenEndPodium::END_PODIUM_LOCATION);
         targetLocation = Vec3d(blockpos.getx(), blockpos.gety(), blockpos.getz());
      }

      double d0 = targetLocation->squareDistanceTo(dragon->posX, dragon->posY, dragon->posZ);
      if (d0 >= 100.0 && d0 <= 22500.0 && !dragon->collidedHorizontally && !dragon->collidedVertically) {
         dragon->setHealth(1.0F);
      } else {
         dragon->setHealth(0.0F);
      }
}

void PhaseDying::initPhase() {
    targetLocation = std::nullopt;
    time = 0;
}

float PhaseDying::getMaxRiseOrFall() {
    return 3.0F;
}

std::optional<Vec3d> PhaseDying::getTargetLocation() {
    return targetLocation;
}

PhaseList * PhaseDying::getType() {
    return PhaseList::DYING.get();
}
