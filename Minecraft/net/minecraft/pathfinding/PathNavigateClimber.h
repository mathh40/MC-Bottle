#pragma once
#include "PathNavigateGround.h"

class PathNavigateClimber : PathNavigateGround
{
public:
  PathNavigateClimber(EntityLiving* entityLivingIn, World* worldIn);
  std::optional<Path> getPathToPos(BlockPos pos) override;
  std::optional<Path> getPathToEntityLiving(Entity* entityIn) override;
  bool tryMoveToEntityLiving(Entity* entityIn, double speedIn) override;
  void onUpdateNavigation() override;
protected:
private:
  std::optional<BlockPos> targetPosition;
};