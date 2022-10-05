#include "SwimNodeProcessor.h"

PathPoint SwimNodeProcessor::getStart()
{
    return openPoint(MathHelper::floor(entity->getEntityBoundingBox().minX),
                     MathHelper::floor(entity->getEntityBoundingBox().minY + 0.5),
                     MathHelper::floor(entity->getEntityBoundingBox().minZ));
}

PathPoint SwimNodeProcessor::getPathPointToCoords(double x, double y, double z)
{
    return openPoint(MathHelper::floor(x - (double)(entity->width / 2.0F)), MathHelper::floor(y + 0.5),
                     MathHelper::floor(z - (double)(entity->width / 2.0F)));
}

int32_t SwimNodeProcessor::findPathOptions(const std::vector<PathPoint> &pathOptions, PathPoint currentPoint,
                                           PathPoint targetPoint, float maxDistance)
{
    int32_t i = 0;
    auto var6 = EnumFacing::values();

    for (int32_t var8 = 0; var8 < var6.size(); ++var8)
    {
        EnumFacing enumfacing = var6[var8];
        auto pathpoint =
            getWaterNode(currentPoint.x + enumfacing.getXOffset(), currentPoint.y + enumfacing.getYOffset(),
                         currentPoint.z + enumfacing.getZOffset());
        if (pathpoint.has_value() && !pathpoint->visited && pathpoint->distanceTo(targetPoint) < maxDistance)
        {
            pathOptions[i++] = pathpoint;
        }
    }

    return i;
}

PathNodeType SwimNodeProcessor::getPathNodeType(IBlockAccess *blockaccessIn, int32_t x, int32_t y, int32_t z,
                                                EntityLiving *entitylivingIn, int32_t xSize, int32_t ySize,
                                                int32_t zSize, bool canBreakDoorsIn, bool canEnterDoorsIn)
{
    return PathNodeType::WATER;
}

PathNodeType SwimNodeProcessor::getPathNodeType(IBlockAccess *blockaccessIn, int32_t x, int32_t y, int32_t z)
{
    return PathNodeType::WATER;
}

std::optional<PathPoint> SwimNodeProcessor::getWaterNode(int32_t p_186328_1_, int32_t p_186328_2_, int32_t p_186328_3_)
{
    PathNodeType pathnodetype = isFree(p_186328_1_, p_186328_2_, p_186328_3_);
    return pathnodetype == PathNodeType::WATER ? openPoint(p_186328_1_, p_186328_2_, p_186328_3_) : std::nullopt;
}

PathNodeType SwimNodeProcessor::isFree(int32_t p_186327_1_, int32_t p_186327_2_, int32_t p_186327_3_) const
{
    MutableBlockPos blockpos$mutableblockpos;

    for (int32_t i = p_186327_1_; i < p_186327_1_ + entitySizeX; ++i)
    {
        for (int32_t j = p_186327_2_; j < p_186327_2_ + entitySizeY; ++j)
        {
            for (int32_t k = p_186327_3_; k < p_186327_3_ + entitySizeZ; ++k)
            {
                IBlockState *iblockstate = blockaccess->getBlockState(blockpos$mutableblockpos.setPos(i, j, k));
                if (iblockstate->getMaterial() != Material::WATER)
                {
                    return PathNodeType::BLOCKED;
                }
            }
        }
    }

    return PathNodeType::WATER;
}
