#include "PathNavigateClimber.h"

PathNavigateClimber::PathNavigateClimber(EntityLiving *entityLivingIn, World *worldIn)
  :PathNavigateGround(entityLivingIn, worldIn)
{
}

std::optional<Path> PathNavigateClimber::getPathToPos(BlockPos pos)
{
  targetPosition = pos;
  return PathNavigateGround::getPathToPos(pos);
}

std::optional<Path> PathNavigateClimber::getPathToEntityLiving(Entity *entityIn)
{
  targetPosition = BlockPos(entityIn);
  return PathNavigateGround::getPathToEntityLiving(entityIn);
}

bool PathNavigateClimber::tryMoveToEntityLiving(Entity *entityIn, double speedIn)
{
  auto path = getPathToEntityLiving(entityIn);
  if (path != std::nullopt) 
  {
    return setPath(path, speedIn);
  }
  else 
  {
    targetPosition = BlockPos(entityIn);
    speed = speedIn;
    return true;
  }
}

void PathNavigateClimber::onUpdateNavigation()
{
  if (!noPath()) 
  {
    PathNavigateGround::onUpdateNavigation();
  }
  else if (targetPosition.has_value()) 
  {
    double d0 = (double)(entity->width * entity->width);
    if (entity->getDistanceSqToCenter(targetPosition) < d0 || entity->posY > (double)targetPosition->gety() && entity->getDistanceSqToCenter(BlockPos(targetPosition->getx(), MathHelper::floor(entity->posy), targetPosition->getz())) < d0) 
    {
      targetPosition = std::nullopt;
    }
    else 
    {
      entity->getMoveHelper().setMoveTo((double)targetPosition->getx(), (double)targetPosition.gety(), (double)targetPosition->getz(), speed);
    }
  }
}
