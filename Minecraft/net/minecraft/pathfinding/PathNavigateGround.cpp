#include "PathNavigateGround.h"

PathNavigateGround::PathNavigateGround(EntityLiving *entitylivingIn, World *worldIn)
  :PathNavigate(entitylivingIn, worldIn)
{
}

std::optional<Path> PathNavigateGround::getPathToEntityLiving(Entity *entityIn)
{
  return getPathToPos(BlockPos(entityIn));
}

PathFinder PathNavigateGround::getPathFinder()
{
  nodeProcessor = new WalkNodeProcessor();
  nodeProcessor.setCanEnterDoors(true);
  return PathFinder(nodeProcessor);
}

bool PathNavigateGround::canNavigate()
{
  return entity->onGround || getCanSwim() && isInLiquid() || entity->isRiding();
}

Vec3d PathNavigateGround::getEntityPosition()
{
  return Vec3d(entity->posX, getPathablePosY(), entity->posZ);
}

std::optional<Path> PathNavigateGround::getPathToPos(BlockPos pos)
{
  BlockPos blockpos1;
  if (world->getBlockState(pos).getMaterial() == Material::AIR) 
  {
    for(blockpos1 = pos.down(); blockpos1.gety() > 0 && world->getBlockState(blockpos1).getMaterial() == Material::AIR; blockpos1 = blockpos1.down()) { }

    if (blockpos1.gety() > 0) 
    {
      return PathNavigate::getPathToPos(blockpos1.up());
    }

    while(blockpos1.gety() < world->getHeight() && world->getBlockState(blockpos1).getMaterial() == Material::AIR) 
    {
      blockpos1 = blockpos1.up();
    }

    pos = blockpos1;
  }

  if (!world->getBlockState(pos).getMaterial().isSolid()) 
  {
    return PathNavigate::getPathToPos(pos);
  }
  else 
  {
    for(blockpos1 = pos.up(); blockpos1.gety() < world->getHeight() && world->getBlockState(blockpos1).getMaterial().isSolid(); blockpos1 = blockpos1.up()) { }

    return PathNavigate::getPathToPos(blockpos1);
  }
}

void PathNavigateGround::removeSunnyPath()
{
  PathNavigate::removeSunnyPath();
  if (shouldAvoidSun) 
  {
    if (world->canSeeSky(BlockPos(MathHelper::floor(entity->posX), entity->getEntityBoundingBox().minY + 0.5, MathHelper::floor(entity->posZ)))) 
    {
      return;
    }

    for(int32_t i = 0; i < currentPath->getCurrentPathLength(); ++i) 
    {
      PathPoint pathpoint = currentPath->getPathPointFromIndex(i);
      if (world->canSeeSky(BlockPos(pathpoint.x, pathpoint.y, pathpoint.z))) 
      {
        currentPath->setCurrentPathLength(i - 1);
        return;
      }
    }
  }
}

bool PathNavigateGround::isDirectPathBetweenPoints(const Vec3d &posVec31, const Vec3d &posVec32, int32_t sizeX, int32_t sizeY, int32_t sizeZ)
{
  int32_t i = MathHelper::floor(posVec31.getx());
  int32_t j = MathHelper::floor(posVec31.getz());
  double d0 = posVec32.getx() - posVec31.getx();
  double d1 = posVec32.getz() - posVec31.getz();
  double d2 = d0 * d0 + d1 * d1;
  if (d2 < 1.0E-8) 
  {
    return false;
  }
  else 
  {
    double d3 = 1.0 / MathHelper::sqrt(d2);
    d0 *= d3;
    d1 *= d3;
    sizeX += 2;
    sizeZ += 2;
    if (!isSafeToStandAt(i, posVec31.gety(), j, sizeX, sizeY, sizeZ, posVec31, d0, d1)) 
    {
      return false;
    }
    else 
    {
      sizeX -= 2;
      sizeZ -= 2;
      double d4 = 1.0 / MathHelper::abs(d0);
      double d5 = 1.0 / MathHelper::abs(d1);
      double d6 = i - posVec31.getx();
      double d7 = j - posVec31.getz();
      if (d0 >= 0.0) 
      {
        ++d6;
      }

      if (d1 >= 0.0) 
      {
        ++d7;
      }

      d6 /= d0;
      d7 /= d1;
      int32_t k = d0 < 0.0 ? -1 : 1;
      int32_t l = d1 < 0.0 ? -1 : 1;
      int32_t i1 = MathHelper::floor(posVec32.getx());
      int32_t j1 = MathHelper::floor(posVec32.getz());
      int32_t k1 = i1 - i;
      int32_t l1 = j1 - j;

      do 
      {
        if (k1 * k <= 0 && l1 * l <= 0) 
        {
          return true;
        }

        if (d6 < d7) 
        {
          d6 += d4;
          i += k;
          k1 = i1 - i;
        }
        else 
        {
          d7 += d5;
          j += l;
          l1 = j1 - j;
        }
      }
      while(isSafeToStandAt(i, posVec31.gety(), j, sizeX, sizeY, sizeZ, posVec31, d0, d1));

      return false;
    }
  }
}

int PathNavigateGround::getPathablePosY()
{
  if (entity->isInWater() && getCanSwim()) 
  {
    int32_t i = entity->getEntityBoundingBox().minY;
    Block block = world->getBlockState(BlockPos(MathHelper::floor(entity->posX), i, MathHelper::floor(entity->posZ))).getBlock();
    int32_t j = 0;

    do 
    {
      if (block != Blocks::FLOWING_WATER && block != Blocks::WATER) 
      {
        return i;
      }

      ++i;
      block = world->getBlockState(BlockPos(MathHelper::floor(entity->posX), i, MathHelper::floor(entity->posZ))).getBlock();
      ++j;
    }
    while(j <= 16);

    return entity->getEntityBoundingBox().minY;
  }
  else 
  {
    return entity->getEntityBoundingBox().minY + 0.5;
  }
}
