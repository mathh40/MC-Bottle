#include "PathNavigate.h"

#include "../world/ChunkCache.h"
#include "../world/World.h"

namespace sc = std::chrono;

PathNavigate::PathNavigate(EntityLiving *entityIn, World *worldIn)
    : lastPosCheck(Vec3d::ZERO), timeoutCachedNode(Vec3d::ZERO), maxDistanceToWaypoint(0.5), entity(entityIn),
      world(worldIn), pathSearchRange(entityIn->getEntityAttribute(SharedMonsterAttributes::FOLLOW_RANGE)),
      pathFinder(getPathFinder())
{
}

void PathNavigate::setSpeed(double speedIn)
{
    speed = speedIn;
}

float PathNavigate::getPathSearchRange() const
{
    return pathSearchRange->getAttributeValue();
}

bool PathNavigate::canUpdatePathOnTimeout() const
{
    return tryUpdatePath;
}

void PathNavigate::updatePath()
{
    if (world->getTotalWorldTime() - lastTimeUpdated > 20L)
    {
        currentPath     = getPathToPos(targetPos);
        lastTimeUpdated = world->getTotalWorldTime();
        tryUpdatePath   = false;
    }
    else
    {
        tryUpdatePath = true;
    }
}

std::optional<Path> PathNavigate::getPathToXYZ(double x, double y, double z)
{
    return getPathToPos(BlockPos(x, y, z));
}

std::optional<Path> PathNavigate::getPathToPos(BlockPos pos)
{
    if (!canNavigate())
    {
        return std::nullopt;
    }
    else if (currentPath.has_value() && !currentPath->isFinished() && pos == targetPos)
    {
        return currentPath;
    }
    else
    {
        targetPos = pos;
        float f   = getPathSearchRange();
        world->profiler.startSection("pathfind");
        BlockPos blockpos     = BlockPos(entity);
        int32_t i             = f + 8.0F;
        ChunkCache chunkcache = ChunkCache(world, blockpos.add(-i, -i, -i), blockpos.add(i, i, i), 0);
        Path path             = pathFinder.findPath(chunkcache, entity, targetPos, f);
        world->profiler.endSection();
        return path;
    }
}

std::optional<Path> PathNavigate::getPathToEntityLiving(Entity *entityIn)
{
    if (!canNavigate())
    {
        return std::nullopt;
    }
    else
    {
        BlockPos blockpos = BlockPos(entityIn);
        if (currentPath.has_value() && !currentPath->isFinished() && blockpos == targetPos)
        {
            return currentPath;
        }
        else
        {
            targetPos = blockpos;
            float f   = getPathSearchRange();
            world->profiler.startSection("pathfind");
            BlockPos blockpos1    = (BlockPos(entity)).up();
            int32_t i             = (f + 16.0F);
            ChunkCache chunkcache = new ChunkCache(world, blockpos1.add(-i, -i, -i), blockpos1.add(i, i, i), 0);
            Path path             = pathFinder.findPath(chunkcache, entity, entityIn, f);
            world->profiler.endSection();
            return path;
        }
    }
}

bool PathNavigate::tryMoveToXYZ(double x, double y, double z, double speedIn)
{
    return setPath(getPathToXYZ(x, y, z), speedIn);
}

bool PathNavigate::tryMoveToEntityLiving(Entity *entityIn, double speedIn)
{
    auto path = getPathToEntityLiving(entityIn);
    return path.has_value() && setPath(path, speedIn);
}

bool PathNavigate::setPath(std::optional<Path> pathentityIn, double speedIn)
{
    if (pathentityIn == std::nullopt)
    {
        currentPath = std::nullopt;
        return false;
    }
    else
    {
        if (!pathentityIn->isSamePath(currentPath.value()))
        {
            currentPath = pathentityIn;
        }

        removeSunnyPath();
        if (currentPath.getCurrentPathLength() <= 0)
        {
            return false;
        }
        else
        {
            speed          = speedIn;
            Vec3d vec3d    = getEntityPosition();
            ticksAtLastPos = totalTicks;
            lastPosCheck   = vec3d;
            return true;
        }
    }
}

std::optional<Path> PathNavigate::getPath() const
{
    return currentPath;
}

void PathNavigate::onUpdateNavigation()
{
    ++totalTicks;
    if (tryUpdatePath)
    {
        updatePath();
    }

    if (!noPath())
    {
        Vec3d vec3d2;
        if (canNavigate())
        {
            pathFollow();
        }
        else if (currentPath.has_value() && currentPath->getCurrentPathIndex() < currentPath->getCurrentPathLength())
        {
            vec3d2       = getEntityPosition();
            Vec3d vec3d1 = currentPath->getVectorFromIndex(entity, currentPath->getCurrentPathIndex());
            if (vec3d2.gety() > vec3d1.gety() && !entity->onGround &&
                MathHelper::floor(vec3d2.getx()) == MathHelper::floor(vec3d1.getx()) &&
                MathHelper::floor(vec3d2.getz()) == MathHelper::floor(vec3d1.getz()))
            {
                currentPath->setCurrentPathIndex(currentPath->getCurrentPathIndex() + 1);
            }
        }

        debugPathFinding();
        if (!noPath())
        {
            vec3d2                      = currentPath->getPosition(entity);
            BlockPos blockpos           = (BlockPos(vec3d2)).down();
            AxisAlignedBB axisalignedbb = world->getBlockState(blockpos)->getBoundingBox(world, blockpos);
            vec3d2                      = vec3d2.subtract(0.0, 1.0 - axisalignedbb.getmaxY(), 0.0);
            entity->getMoveHelper().setMoveTo(vec3d2.getx(), vec3d2.gety(), vec3d2.getz(), speed);
        }
    }
}

bool PathNavigate::noPath()
{
    return currentPath == std::nullopt || currentPath->isFinished();
}

void PathNavigate::clearPath()
{
    currentPath = std::nullopt;
}

bool PathNavigate::canEntityStandOnPos(BlockPos pos)
{
    return world->getBlockState(pos.down())->isFullBlock();
}

NodeProcessor *PathNavigate::getNodeProcessor() const
{
    return nodeProcessor;
}

void PathNavigate::debugPathFinding()
{
}

void PathNavigate::pathFollow()
{
    Vec3d vec3d = getEntityPosition();
    int32_t i   = currentPath->getCurrentPathLength();

    for (int32_t j = currentPath->getCurrentPathIndex(); j < currentPath->getCurrentPathLength(); ++j)
    {
        if ((double)currentPath->getPathPointFromIndex(j).y != MathHelper::floor(vec3d.gety()))
        {
            i = j;
            break;
        }
    }

    maxDistanceToWaypoint = entity->width > 0.75F ? entity->width / 2.0F : 0.75F - entity->width / 2.0F;
    Vec3d vec3d1          = currentPath.getCurrentPos();
    if (MathHelper::abs((entity->posX - (vec3d1.getx() + 0.5))) < maxDistanceToWaypoint &&
        MathHelper::abs((entity->posZ - (vec3d1.getz() + 0.5))) < maxDistanceToWaypoint &&
        MathHelper::abs(entity->posY - vec3d1.gety()) < 1.0)
    {
        currentPath->setCurrentPathIndex(currentPath->getCurrentPathIndex() + 1);
    }

    int32_t k  = MathHelper::ceil(entity->width);
    int32_t l  = MathHelper::ceil(entity->height);
    int32_t i1 = k;

    for (int32_t j1 = i - 1; j1 >= currentPath->getCurrentPathIndex(); --j1)
    {
        if (isDirectPathBetweenPoints(vec3d, currentPath->getVectorFromIndex(entity, j1), k, l, i1))
        {
            currentPath->setCurrentPathIndex(j1);
            break;
        }
    }

    checkForStuck(vec3d);
}

void PathNavigate::checkForStuck(Vec3d positionVec3)
{
    if (totalTicks - ticksAtLastPos > 100)
    {
        if (positionVec3.squareDistanceTo(lastPosCheck) < 2.25)
        {
            clearPath();
        }

        ticksAtLastPos = totalTicks;
        lastPosCheck   = positionVec3;
    }

    if (currentPath.has_value() && !currentPath->isFinished())
    {
        Vec3d vec3d = currentPath->getCurrentPos();
        if (vec3d == timeoutCachedNode)
        {
            auto time        = sc::system_clock::now();
            auto since_epoch = time.time_since_epoch();
            auto millis      = sc::duration_cast<sc::milliseconds>(since_epoch);
            timeoutTimer += millis.count() - lastTimeoutCheck;
        }
        else
        {
            timeoutCachedNode = vec3d;
            double d0         = positionVec3.distanceTo(timeoutCachedNode);
            timeoutLimit      = entity->getAIMoveSpeed() > 0.0F ? d0 / entity->getAIMoveSpeed() * 1000.0 : 0.0;
        }

        if (timeoutLimit > 0.0 && (double)timeoutTimer > timeoutLimit * 3.0)
        {
            timeoutCachedNode = Vec3d::ZERO;
            timeoutTimer      = 0L;
            timeoutLimit      = 0.0;
            clearPath();
        }

        auto time        = sc::system_clock::now();
        auto since_epoch = time.time_since_epoch();
        auto millis      = sc::duration_cast<sc::milliseconds>(since_epoch);

        lastTimeoutCheck = millis.count();
    }
}

bool PathNavigate::isInLiquid() const
{
    return entity->isInWater() || entity->isInLava();
}

void PathNavigate::removeSunnyPath()
{
    if (currentPath.has_value())
    {
        for (int32_t i = 0; i < currentPath->getCurrentPathLength(); ++i)
        {
            PathPoint pathpoint = currentPath.getPathPointFromIndex(i);
            PathPoint pathpoint1 =
                i + 1 < currentPath.getCurrentPathLength() ? currentPath->getPathPointFromIndex(i + 1) : null;
            IBlockState *iblockstate = world->getBlockState(BlockPos(pathpoint.x, pathpoint.y, pathpoint.z));
            Block block              = iblockstate->getBlock();
            if (block == Blocks::CAULDRON)
            {
                currentPath->setPoint(i, pathpoint.cloneMove(pathpoint.x, pathpoint.y + 1, pathpoint.z));
                if (pathpoint1 != null && pathpoint.y >= pathpoint1.y)
                {
                    currentPath->setPoint(i + 1, pathpoint1.cloneMove(pathpoint1.x, pathpoint.y + 1, pathpoint1.z));
                }
            }
        }
    }
}
