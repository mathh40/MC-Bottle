#include "NodeProcessor.h"

void NodeProcessor::init(IBlockAccess *sourceIn, EntityLiving *mob)
{
    blockaccess = sourceIn;
    entity      = mob;
    pointMap.clearMap();
    entitySizeX = MathHelper::floor(mob->width + 1.0F);
    entitySizeY = MathHelper::floor(mob->height + 1.0F);
    entitySizeZ = MathHelper::floor(mob->width + 1.0F);
}

void NodeProcessor::postProcess()
{
    blockaccess = nullptr;
    entity      = nullptr;
}

void NodeProcessor::setCanEnterDoors(bool canEnterDoorsIn)
{
    canEnterDoors = canEnterDoorsIn;
}

void NodeProcessor::setCanOpenDoors(bool canOpenDoorsIn)
{
    canOpenDoors = canOpenDoorsIn;
}

void NodeProcessor::setCanSwim(bool canSwimIn)
{
    canSwim = canSwimIn;
}

bool NodeProcessor::getCanEnterDoors() const
{
    return canEnterDoors;
}

bool NodeProcessor::getCanOpenDoors() const
{
    return canOpenDoors;
}

bool NodeProcessor::getCanSwim() const
{
    return canSwim;
}

std::optional<PathPoint> NodeProcessor::openPoint(int32_t x, int32_t y, int32_t z)
{
    auto i   = PathPoint::makeHash(x, y, z);
    auto ite = pointMap.find(i);
    if (ite != pointMap.end())
    {
        PathPoint pathpoint = ite->second;
        return pathpoint;
    }
    else
    {
        auto pathpoint = PathPoint(x, y, z);
        pointMap.emplace(i, pathpoint);
        return pathpoint;
    }
}
