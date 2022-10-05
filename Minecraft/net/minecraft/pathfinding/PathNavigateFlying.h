#pragma once
#include "PathNavigate.h"

class PathNavigateFlying : public PathNavigate
{
  public:
    PathNavigateFlying(EntityLiving *entityIn, World *worldIn);
    std::optional<Path> getPathToEntityLiving(Entity *entityIn) override;
    void onUpdateNavigation() override;
    void setCanOpenDoors(bool canOpenDoorsIn);
    void setCanEnterDoors(bool canEnterDoorsIn);
    void setCanFloat(bool canFloatIn);
    bool canFloat();
    bool canEntityStandOnPos(BlockPos pos) override;

  protected:
    PathFinder getPathFinder() override;
    bool canNavigate() override;
    Vec3d getEntityPosition() override;
    bool isDirectPathBetweenPoints(const Vec3d &posVec31, const Vec3d &posVec32, int32_t sizeX, int32_t sizeY,
                                   int32_t sizeZ) override;

  private:
};
