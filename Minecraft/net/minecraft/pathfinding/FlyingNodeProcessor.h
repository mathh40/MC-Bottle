#pragma once
#include "WalkNodeProcessor.h"

class FlyingNodeProcessor : public WalkNodeProcessor
{
  public:
    void init(IBlockAccess *sourceIn, EntityLiving *mob);
    void postProcess() override;
    PathPoint getStart() override;
    PathPoint getPathPointToCoords(double x, double y, double z) override;
    int32_t findPathOptions(std::vector<PathPoint> &pathOptions, PathPoint currentPoint, PathPoint targetPoint,
                            float maxDistance) override;
    PathNodeType getPathNodeType(IBlockAccess *blockaccessIn, int32_t x, int32_t y, int32_t z,
                                 EntityLiving *entitylivingIn, int32_t xSize, int32_t ySize, int32_t zSize,
                                 bool canBreakDoorsIn, bool canEnterDoorsIn) override;
    PathNodeType getPathNodeType(IBlockAccess *blockaccessIn, int32_t x, int32_t y, int32_t z) override;

  protected:
    std::optional<PathPoint> openPoint(int32_t x, int32_t y, int32_t z) override;

  private:
    PathNodeType getPathNodeType(EntityLiving *p_192559_1_, BlockPos p_192559_2_) override;
    PathNodeType getPathNodeType(EntityLiving *p_192558_1_, int32_t p_192558_2_, int32_t p_192558_3_,
                                 int32_t p_192558_4_) override;
};
