#include "PhaseChargingPlayer.h"

#include "../../EntityDragon.h"

std::shared_ptr<spdlog::logger> PhaseChargingPlayer::LOGGER = spdlog::get("Minecraft")->clone("PhaseChargingPlayer");

PhaseChargingPlayer::PhaseChargingPlayer(EntityDragon *dragonIn) : PhaseBase(dragonIn)
{
}

void PhaseChargingPlayer::doLocalUpdate()
{
    if (targetLocation == std::nullopt)
    {
        LOGGER->warn("Aborting charge player as no target was set.");
        dragon->getPhaseManager().setPhase(PhaseList::HOLDING_PATTERN.get());
    }
    else if (timeSinceCharge > 0 && timeSinceCharge++ >= 10)
    {
        dragon->getPhaseManager().setPhase(PhaseList::HOLDING_PATTERN.get());
    }
    else
    {
        double d0 = targetLocation->squareDistanceTo(dragon->posX, dragon->posY, dragon->posZ);
        if (d0 < 100.0 || d0 > 22500.0 || dragon->collidedHorizontally || dragon->collidedVertically)
        {
            ++timeSinceCharge;
        }
    }
}

void PhaseChargingPlayer::initPhase()
{
    targetLocation  = std::nullopt;
    timeSinceCharge = 0;
}

void PhaseChargingPlayer::setTarget(const Vec3d &p_188668_1_)
{
    targetLocation = p_188668_1_;
}

float PhaseChargingPlayer::getMaxRiseOrFall()
{
    return 3.0F;
}

std::optional<Vec3d> PhaseChargingPlayer::getTargetLocation()
{
    return targetLocation;
}

PhaseList *PhaseChargingPlayer::getType()
{
    return PhaseList::CHARGING_PLAYER.get();
}
