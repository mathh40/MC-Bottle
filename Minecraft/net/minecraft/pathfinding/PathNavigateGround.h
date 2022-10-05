#pragma once
#include "PathNavigate.h"

class PathNavigateGround : public PathNavigate
{
  public:
    PathNavigateGround(EntityLiving *entitylivingIn, World *worldIn);
    std::optional<Path> getPathToEntityLiving(Entity *entityIn) override;

  protected:
    PathFinder getPathFinder() override;
    bool canNavigate() override;
    Vec3d getEntityPosition() override;
    virtual std::optional<Path> getPathToPos(BlockPos pos);
    void removeSunnyPath() override;
    bool isDirectPathBetweenPoints(const Vec3d &posVec31, const Vec3d &posVec32, int32_t sizeX, int32_t sizeY,
                                   int32_t sizeZ) override;

  private:
    int getPathablePosY();

    bool shouldAvoidSun;
};
