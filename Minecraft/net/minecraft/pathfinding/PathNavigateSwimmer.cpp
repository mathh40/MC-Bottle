#include "PathNavigateSwimmer.h"

#include "../world/World.h"

PathNavigateSwimmer::PathNavigateSwimmer(EntityLiving *entitylivingIn, World *worldIn)
  :PathNavigate(entitylivingIn, worldIn)
{
}

bool PathNavigateSwimmer::canEntityStandOnPos(BlockPos pos)
{
  return !world->getBlockState(pos)->isFullBlock();
}

PathFinder PathNavigateSwimmer::getPathFinder()
{
  return PathFinder(new SwimNodeProcessor());
}

bool PathNavigateSwimmer::canNavigate()
{
  return isInLiquid();
}

Vec3d PathNavigateSwimmer::getEntityPosition()
{
  return Vec3d(entity->posX, entity->posY + entity->height * 0.5, entity->posZ);
}

void PathNavigateSwimmer::pathFollow()
{
  Vec3d vec3d = getEntityPosition();
  float f = entity->width * entity->width;
  if (vec3d.squareDistanceTo(currentPath->getVectorFromIndex(entity, currentPath->getCurrentPathIndex())) < f) 
  {
    currentPath->incrementPathIndex();
  }

  for(int32_t j = MathHelper::min(currentPath->getCurrentPathIndex() + 6, currentPath->getCurrentPathLength() - 1); j > currentPath->getCurrentPathIndex(); --j) 
  {
    Vec3d vec3d1 = currentPath->getVectorFromIndex(entity, j);
    if (vec3d1.squareDistanceTo(vec3d) <= 36.0 && isDirectPathBetweenPoints(vec3d, vec3d1, 0, 0, 0)) 
    {
      currentPath->setCurrentPathIndex(j);
      break;
    }
  }

  checkForStuck(vec3d);
}

bool PathNavigateSwimmer::isDirectPathBetweenPoints(const Vec3d &posVec31, const Vec3d &posVec32, int32_t sizeX, int32_t sizeY, int32_t sizeZ)
{
  RayTraceResult raytraceresult = world->rayTraceBlocks(posVec31, Vec3d(posVec32.getx(), posVec32.gety() + entity->height * 0.5, posVec32.getz()), false, true, false);
  return raytraceresult.typeOfHit == RayTraceResult::Type::MISS;
}
