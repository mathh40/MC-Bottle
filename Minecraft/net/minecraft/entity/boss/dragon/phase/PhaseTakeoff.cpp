#include "PhaseTakeoff.h"

#include "../../EntityDragon.h"
#include "gen/feature/WorldGenEndPodium.h"

PhaseTakeoff::PhaseTakeoff(EntityDragon *dragonIn) : PhaseBase(dragonIn)
{
}

void PhaseTakeoff::doLocalUpdate()
{
    if (!firstTick && currentPath.has_value())
    {
        BlockPos blockpos = dragon->world.getTopSolidOrLiquidBlock(WorldGenEndPodium::END_PODIUM_LOCATION);
        double d0         = dragon->getDistanceSqToCenter(blockpos);
        if (d0 > 100.0)
        {
            dragon->getPhaseManager().setPhase(PhaseList::HOLDING_PATTERN.get());
        }
    }
    else
    {
        firstTick = false;
        findNewTarget();
    }
}

void PhaseTakeoff::initPhase()
{
    firstTick      = true;
    currentPath    = std::nullopt;
    targetLocation = std::nullopt;
}

std::optional<Vec3d> PhaseTakeoff::getTargetLocation()
{
    return targetLocation;
}

PhaseList *PhaseTakeoff::getType()
{
    return PhaseList::TAKEOFF.get();
}

void PhaseTakeoff::findNewTarget()
{
    auto i      = dragon->initPathPoints();
    Vec3d vec3d = dragon->getHeadLookVec(1.0F);
    auto j      = dragon->getNearestPpIdx(-vec3d.getx() * 40.0, 105.0, -vec3d.getz() * 40.0);
    if (dragon->getFightManager().has_value() && dragon->getFightManager()->getNumAliveCrystals() > 0)
    {
        j %= 12;
        if (j < 0)
        {
            j += 12;
        }
    }
    else
    {
        j -= 12;
        j &= 7;
        j += 12;
    }

    currentPath = dragon->findPath(i, j, std::nullopt);
    if (currentPath.has_value())
    {
        currentPath->incrementPathIndex();
        navigateToNextPathNode();
    }
}

void PhaseTakeoff::navigateToNextPathNode()
{
    Vec3d vec3d = currentPath->getCurrentPos();
    currentPath->incrementPathIndex();

    double d0;
    do
    {
        d0 = vec3d.gety() + (double)(MathHelper::nextFloat(dragon->getRNG()) * 20.0F);
    } while (d0 < vec3d.gety());

    targetLocation = Vec3d(vec3d.getx(), d0, vec3d.getz());
}
