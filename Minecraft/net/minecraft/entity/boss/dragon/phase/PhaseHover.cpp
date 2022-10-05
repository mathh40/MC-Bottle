#include "PhaseHover.h"

#include "../../EntityDragon.h"

PhaseHover::PhaseHover(EntityDragon *dragonIn) : PhaseBase(dragonIn)
{
}

void PhaseHover::doLocalUpdate()
{
    if (targetLocation == std::nullopt)
    {
        targetLocation = Vec3d(dragon->posX, dragon->posY, dragon->posZ);
    }
}

bool PhaseHover::getIsStationary()
{
    return true;
}

void PhaseHover::initPhase()
{
    targetLocation = std::nullopt;
}

float PhaseHover::getMaxRiseOrFall()
{
    return 1.0F;
}

std::optional<Vec3d> PhaseHover::getTargetLocation()
{
    return targetLocation;
}

PhaseList *PhaseHover::getType()
{
    return PhaseList::HOVER.get();
}
