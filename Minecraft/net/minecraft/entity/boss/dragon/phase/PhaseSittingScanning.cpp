#include "PhaseSittingScanning.h"

#include "../../EntityDragon.h"
#include "PhaseChargingPlayer.h"

PhaseSittingScanning::PhaseSittingScanning(EntityDragon *dragonIn) : PhaseSittingBase(dragonIn)
{
}

void PhaseSittingScanning::doLocalUpdate()
{
    ++scanningTime;
    EntityLivingBase *entitylivingbase = dragon->world->getNearestAttackablePlayer(dragon, 20.0, 10.0);
    if (entitylivingbase != nullptr)
    {
        if (scanningTime > 25)
        {
            dragon->getPhaseManager().setPhase(PhaseList::SITTING_ATTACKING.get());
        }
        else
        {
            Vec3d vec3d =
                Vec3d(entitylivingbase->posX - dragon->posX, 0.0, entitylivingbase->posZ - dragon->posZ).normalize();
            Vec3d vec3d1 = Vec3d((double)MathHelper::sin(dragon->rotationYaw * 0.017453292F), 0.0,
                                 (double)(-MathHelper::cos(dragon->rotationYaw * 0.017453292F)))
                               .normalize();
            float f  = (float)vec3d1.dotProduct(vec3d);
            float f1 = (float)(MathHelper::acos((double)f) * 57.29577951308232) + 0.5F;
            if (f1 < 0.0F || f1 > 10.0F)
            {
                double d0 = entitylivingbase->posX - dragon->dragonPartHead.posX;
                double d1 = entitylivingbase->posZ - dragon->dragonPartHead.posZ;
                double d2 =
                    MathHelper::clamp(MathHelper::wrapDegrees(180.0 - MathHelper::atan2(d0, d1) * 57.29577951308232 -
                                                              (double)dragon->rotationYaw),
                                      -100.0, 100.0);
                dragon->randomYawVelocity *= 0.8F;
                float f2 = MathHelper::sqrt(d0 * d0 + d1 * d1) + 1.0F;
                float f3 = f2;
                if (f2 > 40.0F)
                {
                    f2 = 40.0F;
                }

                dragon->randomYawVelocity = (float)((double)dragon->randomYawVelocity + d2 * (double)(0.7F / f2 / f3));
                dragon->rotationYaw += dragon->randomYawVelocity;
            }
        }
    }
    else if (scanningTime >= 100)
    {
        entitylivingbase = dragon->world->getNearestAttackablePlayer(dragon, 150.0, 150.0);
        dragon->getPhaseManager().setPhase(PhaseList::TAKEOFF.get());
        if (entitylivingbase != nullptr)
        {
            dragon->getPhaseManager().setPhase(PhaseList::CHARGING_PLAYER.get());
            ((PhaseChargingPlayer *)dragon->getPhaseManager().getPhase(PhaseList::CHARGING_PLAYER.get()))
                ->setTarget(Vec3d(entitylivingbase->posX, entitylivingbase->posY, entitylivingbase->posZ));
        }
    }
}

void PhaseSittingScanning::initPhase()
{
    scanningTime = 0;
}

PhaseList *PhaseSittingScanning::getType()
{
    return PhaseList::SITTING_SCANNING.get();
}
