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
    int32_t findPathOptions(std::vector<PathPoint>& pathOptions, PathPoint currentPoint, PathPoint targetPoint, float maxDistance) override;
    PathNodeType getPathNodeType(IBlockAccess* blockaccessIn, int32_t x, int32_t y, int32_t z, EntityLiving* entitylivingIn, int32_t xSize, int32_t ySize, int32_t zSize, bool canBreakDoorsIn, bool canEnterDoorsIn) override;
    PathNodeType getPathNodeType(IBlockAccess* p_193577_1_, int32_t x, int32_t y, int32_t z, int32_t xSize, int32_t ySize, int32_t zSize, bool canOpenDoorsIn, bool canEnterDoorsIn, EnumSet p_193577_10_, PathNodeType p_193577_11_, BlockPos p_193577_12_);
    PathNodeType getPathNodeType(IBlockAccess* blockaccessIn, int32_t x, int32_t y, int32_t z) override;
    PathNodeType checkNeighborBlocks(IBlockAccess* p_193578_1_, int32_t p_193578_2_, int32_t p_193578_3_, int32_t p_193578_4_, PathNodeType p_193578_5_);

protected:
    float avoidsWater;
    PathNodeType getPathNodeTypeRaw(IBlockAccess* p_189553_1_, int32_t p_189553_2_, int32_t p_189553_3_, int32_t p_189553_4_);
private:
    std::optional<PathPoint> getSafePoint(int32_t x, int32_t y, int32_t z, int32_t p_186332_4_, double p_186332_5_, EnumFacing facing);
    virtual PathNodeType getPathNodeType(EntityLiving* entitylivingIn, BlockPos pos);
    virtual PathNodeType getPathNodeType(EntityLiving* entitylivingIn, int32_t x, int32_t y, int32_t z);
};
