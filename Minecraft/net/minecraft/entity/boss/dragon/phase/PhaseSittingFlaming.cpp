#include "PhaseSittingFlaming.h"

#include "../../EntityDragon.h"
#include "../../../EntityAreaEffectCloud.h"

PhaseSittingFlaming::PhaseSittingFlaming(EntityDragon *dragonIn)
    :PhaseSittingBase(dragonIn){
}

void PhaseSittingFlaming::doClientRenderEffects() {
    ++flameTicks;
      if (flameTicks % 2 == 0 && flameTicks < 10) {
         Vec3d vec3d = dragon->getHeadLookVec(1.0F).normalize();
         vec3d.rotateYaw(-0.7853982F);
         double d0 = dragon->dragonPartHead.posX;
         double d1 = dragon->dragonPartHead.posY + (double)(dragon->dragonPartHead.height / 2.0F);
         double d2 = dragon->dragonPartHead.posZ;

         for(int i = 0; i < 8; ++i) {
            double d3 = d0 + MathHelper::nextGaussian<double>(dragon->getRNG()) / 2.0;
            double d4 = d1 + MathHelper::nextGaussian<double>(dragon->getRNG()) / 2.0;
            double d5 = d2 + MathHelper::nextGaussian<double>(dragon->getRNG()) / 2.0;

            for(int j = 0; j < 6; ++j) {
               dragon->world->spawnParticle(EnumParticleTypes::DRAGON_BREATH, d3, d4, d5, -vec3d.getx() * 0.07999999821186066 * (double)j, -vec3d.gety() * 0.6000000238418579, -vec3d.getz() * 0.07999999821186066 * (double)j,{});
            }

            vec3d.rotateYaw(0.19634955F);
         }
      }
}

void PhaseSittingFlaming::doLocalUpdate() {
    ++flameTicks;
      if (flameTicks >= 200) {
         if (flameCount >= 4) {
            dragon->getPhaseManager().setPhase(PhaseList::TAKEOFF.get());
         } else {
            dragon->getPhaseManager().setPhase(PhaseList::SITTING_SCANNING.get());
         }
      } else if (flameTicks == 10) {
         Vec3d vec3d = (Vec3d(dragon->dragonPartHead.posX - dragon->posX, 0.0, dragon->dragonPartHead.posZ - dragon->posZ)).normalize();
         float f = 5.0F;
         double d0 = dragon->dragonPartHead.posX + vec3d.getx() * 5.0 / 2.0;
         double d1 = dragon->dragonPartHead.posZ + vec3d.getz() * 5.0 / 2.0;
         double d2 = dragon->dragonPartHead.posY + (double)(dragon->dragonPartHead.height / 2.0F);
         BlockPos blockpos$mutableblockpos = BlockPos(MathHelper::floor(d0), MathHelper::floor(d2), MathHelper::floor(d1));

         while(dragon->world->isAirBlock(blockpos$mutableblockpos)) {
            --d2;
            blockpos$mutableblockpos.setPos(MathHelper::floor(d0), MathHelper::floor(d2), MathHelper::floor(d1));
         }

         d2 = (double)(MathHelper::floor(d2) + 1);
         areaEffectCloud = new EntityAreaEffectCloud(dragon->world, d0, d2, d1);
         areaEffectCloud->setOwner(dragon);
         areaEffectCloud->setRadius(5.0F);
         areaEffectCloud->setDuration(200);
         areaEffectCloud->setParticle(EnumParticleTypes::DRAGON_BREATH);
         areaEffectCloud->addEffect(PotionEffect(MobEffects::INSTANT_DAMAGE));
         dragon->world->spawnEntity(areaEffectCloud);
      }
}

void PhaseSittingFlaming::initPhase() {
    flameTicks = 0;
    ++flameCount;
}

void PhaseSittingFlaming::removeAreaEffect() {
    if (areaEffectCloud != nullptr) {
         areaEffectCloud->setDead();
         areaEffectCloud = nullptr;
      }
}

PhaseList * PhaseSittingFlaming::getType() {
    return PhaseList::SITTING_FLAMING.get();
}

void PhaseSittingFlaming::resetFlameCount() {
    flameCount = 0;
}
