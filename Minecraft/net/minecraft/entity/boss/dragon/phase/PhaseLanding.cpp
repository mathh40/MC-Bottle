#include "PhaseLanding.h"
#include "../../EntityDragon.h"
#include "gen/feature/WorldGenEndPodium.h"

PhaseLanding::PhaseLanding(EntityDragon *dragonIn)
    :PhaseBase(dragonIn){
}

void PhaseLanding::doClientRenderEffects() {
    Vec3d vec3d = dragon->getHeadLookVec(1.0F).normalize();
      vec3d = vec3d.rotateYaw(-0.7853982F);
      double d0 = dragon->dragonPartHead.posX;
      double d1 = dragon->dragonPartHead.posY + (double)(dragon->dragonPartHead.height / 2.0F);
      double d2 = dragon->dragonPartHead.posZ;

      for(auto i = 0; i < 8; ++i) {
         double d3 = d0 + MathHelper::nextGaussian<double>(dragon->getRNG()) / 2.0;
         double d4 = d1 + MathHelper::nextGaussian<double>(dragon->getRNG()) / 2.0;
         double d5 = d2 + MathHelper::nextGaussian<double>(dragon->getRNG()) / 2.0;
         dragon->world->spawnParticle(EnumParticleTypes::DRAGON_BREATH, d3, d4, d5, -vec3d.getx() * 0.07999999821186066 + dragon->motionX, -vec3d.gety() * 0.30000001192092896 + dragon->motionY, -vec3d.z * 0.07999999821186066 + dragon->motionZ);
         vec3d = vec3d.rotateYaw(0.19634955F);
      }
}

void PhaseLanding::doLocalUpdate() {
    if (targetLocation == std::nullopt) {
         targetLocation = Vec3d(dragon->world->getTopSolidOrLiquidBlock(WorldGenEndPodium::END_PODIUM_LOCATION));
      }

      if (targetLocation->squareDistanceTo(dragon->posX, dragon->posY, dragon->posZ) < 1.0) {
         ((PhaseSittingFlaming*)dragon->getPhaseManager().getPhase(PhaseList::SITTING_FLAMING.get()))->resetFlameCount();
         dragon->getPhaseManager().setPhase(PhaseList::SITTING_SCANNING.get());
      }
}

float PhaseLanding::getMaxRiseOrFall() {
    return 1.5F;
}

float PhaseLanding::getYawFactor() {
    const float f = MathHelper::sqrt(dragon->motionX * dragon->motionX + dragon->motionZ * dragon->motionZ) + 1.0F;
    const float f1 = MathHelper::min(f, 40.0F);
    return f1 / f;
}

void PhaseLanding::initPhase() {
    targetLocation = std::nullopt;
}

std::optional<Vec3d> PhaseLanding::getTargetLocation() {
    return targetLocation;
}

PhaseList * PhaseLanding::getType() {
    return PhaseList::LANDING.get();
}
