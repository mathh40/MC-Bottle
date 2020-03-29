#include "PathNavigateFlying.h"

#include "../world/World.h"

PathNavigateFlying::PathNavigateFlying(EntityLiving *entityIn, World *worldIn)
  :PathNavigate(entityIn, worldIn)
{}

std::optional<Path> PathNavigateFlying::getPathToEntityLiving(Entity *entityIn)
{
    return getPathToPos(BlockPos(entityIn));
}

void PathNavigateFlying::onUpdateNavigation()
{
    ++totalTicks;
    if (tryUpdatePath) 
    {
        updatePath();
    }

    if (!noPath()) 
    {
        Vec3d vec3d1;
        if (canNavigate()) 
        {
            pathFollow();
        }
        else if (currentPath.has_value() && currentPath->getCurrentPathIndex() < currentPath->getCurrentPathLength()) 
        {
            vec3d1 = currentPath->getVectorFromIndex(entity, currentPath->getCurrentPathIndex());
            if (MathHelper::floor(entity->posX) == MathHelper::floor(vec3d1.getx()) && MathHelper::floor(entity->posY) == MathHelper::floor(vec3d1.gety()) && MathHelper::floor(entity->posZ) == MathHelper::floor(vec3d1.getz())) 
            {
                currentPath->setCurrentPathIndex(currentPath->getCurrentPathIndex() + 1);
            }
        }

        debugPathFinding();
        if (!noPath()) 
        {
            vec3d1 = currentPath.getPosition(entity);
            entity->getMoveHelper().setMoveTo(vec3d1.x, vec3d1.y, vec3d1.z, this.speed);
        }
    }
}

void PathNavigateFlying::setCanOpenDoors(bool canOpenDoorsIn)
{
    nodeProcessor->setCanOpenDoors(canOpenDoorsIn);
}

void PathNavigateFlying::setCanEnterDoors(bool canEnterDoorsIn)
{
    nodeProcessor->setCanEnterDoors(canEnterDoorsIn);
}

void PathNavigateFlying::setCanFloat(bool canFloatIn)
{
    nodeProcessor->setCanSwim(canFloatIn);
}

bool PathNavigateFlying::canFloat()
{
    return nodeProcessor->getCanSwim();
}

bool PathNavigateFlying::canEntityStandOnPos(BlockPos pos)
{
    return world->getBlockState(pos)->isTopSolid();
}

PathFinder PathNavigateFlying::getPathFinder()
{
    nodeProcessor = new FlyingNodeProcessor();
    nodeProcessor->setCanEnterDoors(true);
    return PathFinder(nodeProcessor);
}

bool PathNavigateFlying::canNavigate()
{
    return canFloat() && isInLiquid() || !entity->isRiding();
}

Vec3d PathNavigateFlying::getEntityPosition()
{
    return Vec3d(entity->posX, entity->posY, entity->posZ);
}

bool PathNavigateFlying::isDirectPathBetweenPoints(const Vec3d &posVec31, const Vec3d &posVec32, int32_t sizeX,
    int32_t sizeY, int32_t sizeZ)
{
    int32_t i = MathHelper::floor(posVec31.getx());
    int32_t j = MathHelper::floor(posVec31.gety());
    int32_t k = MathHelper::floor(posVec31.getz());
    double d0 = posVec32.getx() - posVec31.getx();
    double d1 = posVec32.gety() - posVec31.gety();
    double d2 = posVec32.getz() - posVec31.getz();
    double d3 = d0 * d0 + d1 * d1 + d2 * d2;
    if (d3 < 1.0E-8) 
    {
        return false;
    }
    else 
    {
        double d4 = 1.0 / MathHelper::sqrt(d3);
        d0 *= d4;
        d1 *= d4;
        d2 *= d4;
        double d5 = 1.0 / MathHelper::abs(d0);
        double d6 = 1.0 / MathHelper::abs(d1);
        double d7 = 1.0 / MathHelper::abs(d2);
        double d8 = (double)i - posVec31.getx();
        double d9 = (double)j - posVec31.gety();
        double d10 = (double)k - posVec31.getz();
        if (d0 >= 0.0) 
        {
            ++d8;
        }

        if (d1 >= 0.0) 
        {
            ++d9;
        }

        if (d2 >= 0.0) 
        {
            ++d10;
        }

        d8 /= d0;
        d9 /= d1;
        d10 /= d2;
        int32_t l = d0 < 0.0 ? -1 : 1;
        int32_t i1 = d1 < 0.0 ? -1 : 1;
        int32_t j1 = d2 < 0.0 ? -1 : 1;
        int32_t k1 = MathHelper::floor(posVec32.getx());
        int32_t l1 = MathHelper::floor(posVec32.gety());
        int32_t i2 = MathHelper::floor(posVec32.getz());
        int32_t j2 = k1 - i;
        int32_t k2 = l1 - j;
        int32_t l2 = i2 - k;

        while(true) 
        {
            while(j2 * l > 0 || k2 * i1 > 0 || l2 * j1 > 0) 
            {
                if (d8 < d10 && d8 <= d9) 
                {
                    d8 += d5;
                    i += l;
                    j2 = k1 - i;
                }
                else if (d9 < d8 && d9 <= d10) 
                {
                    d9 += d6;
                    j += i1;
                    k2 = l1 - j;
                }
                else 
                {
                    d10 += d7;
                    k += j1;
                    l2 = i2 - k;
                }
            }

            return true;
        }
    }
}
