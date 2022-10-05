#include "PathFinder.h"

PathFinder::PathFinder(NodeProcessor *processor) : nodeProcessor(processor)
{
    pathOptions.resize(32);
}

std::optional<Path> PathFinder::findPath(IBlockAccess *worldIn, EntityLiving *entitylivingIn, Entity *targetEntity,
                                         float maxDistance)
{
    return findPath(worldIn, entitylivingIn, targetEntity->posX, targetEntity->getEntityBoundingBox().minY,
                    targetEntity->posZ, maxDistance);
}

std::optional<Path> PathFinder::findPath(IBlockAccess *worldIn, EntityLiving *entitylivingIn, BlockPos &targetPos,
                                         float maxDistance)
{
    return findPath(worldIn, entitylivingIn, (double)((float)targetPos.getx() + 0.5F),
                    (double)((float)targetPos.gety() + 0.5F), (double)((float)targetPos.getz() + 0.5F), maxDistance);
}

std::optional<Path> PathFinder::findPath(IBlockAccess *worldIn, EntityLiving *entitylivingIn, double x, double y,
                                         double z, float maxDistance)
{
    path.clearPath();
    nodeProcessor->init(worldIn, entitylivingIn);
    PathPoint pathpoint  = nodeProcessor->getStart();
    PathPoint pathpoint1 = nodeProcessor->getPathPointToCoords(x, y, z);
    auto path            = findPath(pathpoint, pathpoint1, maxDistance);
    nodeProcessor->postProcess();
    return path;
}

std::optional<Path> PathFinder::findPath(PathPoint &pathFrom, PathPoint &pathTo, float maxDistance)
{
    pathFrom.totalPathDistance = 0.0F;
    pathFrom.distanceToNext    = pathFrom.distanceManhattan(pathTo);
    pathFrom.distanceToTarget  = pathFrom.distanceToNext;
    path.clearPath();
    closedSet.clear();
    path.addPoint(pathFrom);
    PathPoint pathpoint = pathFrom;
    auto i              = 0;

    while (!path.isPathEmpty())
    {
        ++i;
        if (i >= 200)
        {
            break;
        }

        PathPoint pathpoint1 = path.dequeue();
        if (pathpoint1 == pathTo)
        {
            pathpoint = pathTo;
            break;
        }

        if (pathpoint1.distanceManhattan(pathTo) < pathpoint.distanceManhattan(pathTo))
        {
            pathpoint = pathpoint1;
        }

        pathpoint1.visited = true;
        auto j             = nodeProcessor->findPathOptions(pathOptions, pathpoint1, pathTo, maxDistance);

        for (auto k = 0; k < j; ++k)
        {
            PathPoint pathpoint2          = pathOptions[k];
            float f                       = pathpoint1.distanceManhattan(pathpoint2);
            pathpoint2.distanceFromOrigin = pathpoint1.distanceFromOrigin + f;
            pathpoint2.cost               = f + pathpoint2.costMalus;
            float f1                      = pathpoint1.totalPathDistance + pathpoint2.cost;
            if (pathpoint2.distanceFromOrigin < maxDistance &&
                (!pathpoint2.isAssigned() || f1 < pathpoint2.totalPathDistance))
            {
                pathpoint2.previous          = &pathpoint1;
                pathpoint2.totalPathDistance = f1;
                pathpoint2.distanceToNext    = pathpoint2.distanceManhattan(pathTo) + pathpoint2.costMalus;
                if (pathpoint2.isAssigned())
                {
                    path.changeDistance(pathpoint2, pathpoint2.totalPathDistance + pathpoint2.distanceToNext);
                }
                else
                {
                    pathpoint2.distanceToTarget = pathpoint2.totalPathDistance + pathpoint2.distanceToNext;
                    path.addPoint(pathpoint2);
                }
            }
        }
    }

    if (pathpoint == pathFrom)
    {
        return std::nullopt;
    }
    else
    {
        auto path = createPath(pathFrom, pathpoint);
        return path;
    }
}

Path PathFinder::createPath(const PathPoint &start, const PathPoint &end)
{
    auto i = 1;

    for (const PathPoint *pathpoint = &end; pathpoint->previous != nullptr; pathpoint = pathpoint->previous)
    {
        ++i;
    }

    std::vector<PathPoint> apathpoint;
    apathpoint.resize(i);
    const PathPoint *pathpoint1 = &end;
    --i;

    for (apathpoint[i] = end; pathpoint1->previous != nullptr; apathpoint[i] = *pathpoint1)
    {
        pathpoint1 = pathpoint1->previous;
        --i;
    }

    return Path(apathpoint);
}
