#include "NodeProcessor.h"

class SwimNodeProcessor : public NodeProcessor
{
  public:
    PathPoint getStart() override;
    PathPoint getPathPointToCoords(double x, double y, double z) override;
    int32_t findPathOptions(const std::vector<PathPoint> &pathOptions, PathPoint currentPoint, PathPoint targetPoint,
                            float maxDistance) override;
    PathNodeType getPathNodeType(IBlockAccess *blockaccessIn, int32_t x, int32_t y, int32_t z,
                                 EntityLiving *entitylivingIn, int32_t xSize, int32_t ySize, int32_t zSize,
                                 bool canBreakDoorsIn, bool canEnterDoorsIn) override;
    PathNodeType getPathNodeType(IBlockAccess *blockaccessIn, int32_t x, int32_t y, int32_t z) override;

  private:
    std::optional<PathPoint> getWaterNode(int32_t p_186328_1_, int32_t p_186328_2_, int32_t p_186328_3_);
    PathNodeType isFree(int32_t p_186327_1_, int32_t p_186327_2_, int32_t p_186327_3_) const;
};
