#pragma once
#include "PathNavigate.h"

class PathNavigateSwimmer : public PathNavigate
{
  public:
    PathNavigateSwimmer(EntityLiving *entitylivingIn, World *worldIn);
    bool canEntityStandOnPos(BlockPos pos) override;

  protected:
    PathFinder getPathFinder() override;
    bool canNavigate() override;
    Vec3d getEntityPosition() override;
    void pathFollow() override;
    bool isDirectPathBetweenPoints(const Vec3d &posVec31, const Vec3d &posVec32, int32_t sizeX, int32_t sizeY,
                                   int32_t sizeZ) override;

  private:
};
