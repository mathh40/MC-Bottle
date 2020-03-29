#pragma once
#include "NodeProcessor.h"

class EntityLiving;

class WalkNodeProcessor : public NodeProcessor
{
public:
    void init(IBlockAccess* sourceIn, EntityLiving* mob) override;
    void postProcess() override;
    PathPoint getStart() override;
    PathPoint getPathPointToCoords(double x, double y, double z) override;
    int32_t findPathOptions(const std::vector<PathPoint>& pathOptions, PathPoint currentPoint, PathPoint targetPoint, float maxDistance) override;
    PathNodeType getPathNodeType(IBlockAccess* blockaccessIn, int32_t x, int32_t y, int32_t z, EntityLiving* entitylivingIn, int32_t xSize, int32_t ySize, int32_t zSize, bool canBreakDoorsIn, bool canEnterDoorsIn) override;
protected:
    float avoidsWater;
private:
    std::optional<PathPoint> getSafePoint(int32_t x, int32_t y, int32_t z, int32_t p_186332_4_, double p_186332_5_, EnumFacing facing);
};
