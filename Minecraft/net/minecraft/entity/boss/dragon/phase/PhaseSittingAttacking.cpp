#include "PhaseSittingAttacking.h"

#include "../../EntityDragon.h"

PhaseSittingAttacking::PhaseSittingAttacking(EntityDragon *dragonIn) : PhaseSittingBase(dragonIn)
{
}

void PhaseSittingAttacking::doClientRenderEffects()
{
    dragon->world->playSound(dragon->posX, dragon->posY, dragon->posZ, SoundEvents::ENTITY_ENDERDRAGON_GROWL,
                             dragon->getSoundCategory(), 2.5F, 0.8F + MathHelper::nextFloat(dragon->getRNG()) * 0.3F,
                             false);
}

void PhaseSittingAttacking::doLocalUpdate()
{
    if (attackingTicks++ >= 40)
    {
        dragon->getPhaseManager().setPhase(PhaseList::SITTING_FLAMING.get());
    }
}

void PhaseSittingAttacking::initPhase()
{
    attackingTicks = 0;
}

PhaseList *PhaseSittingAttacking::getType()
{
    return PhaseList::SITTING_ATTACKING.get();
}
