#include "FlyingNodeProcessor.h"

void FlyingNodeProcessor::init(IBlockAccess *sourceIn, EntityLiving *mob)
{
    WalkNodeProcessor::init(sourceIn, mob);
    avoidsWater = mob->getPathPriority(PathNodeType::WATER);
}

void FlyingNodeProcessor::postProcess()
{
    entity->setPathPriority(PathNodeType::WATER, avoidsWater);
    WalkNodeProcessor::postProcess();
}

PathPoint FlyingNodeProcessor::getStart()
{
    int32_t i = 0;
    if (getCanSwim() && entity->isInWater())
    {
        i = entity->getEntityBoundingBox().minY;
        MutableBlockPos blockpos$mutableblockpos =
            MutableBlockPos(MathHelper::floor(entity->posX), i, MathHelper::floor(entity->posZ));

        for (Block block = blockaccess->getBlockState(blockpos$mutableblockpos).getBlock();
             block == Blocks::FLOWING_WATER || block == Blocks::WATER;
             block = blockaccess->getBlockState(blockpos$mutableblockpos).getBlock())
        {
            ++i;
            blockpos$mutableblockpos.setPos(MathHelper::floor(entity->posX), i, MathHelper::floor(entity->posZ));
        }
    }
    else
    {
        i = MathHelper::floor(entity->getEntityBoundingBox().minY + 0.5);
    }

    BlockPos blockpos1         = BlockPos(entity);
    PathNodeType pathnodetype1 = getPathNodeType(entity, blockpos1.getx(), i, blockpos1.getz());
    if (entity->getPathPriority(pathnodetype1) < 0.0F)
    {
        std::set<BlockPos> posset;
        posset.insert(BlockPos(entity->getEntityBoundingBox().minX, (double)i, entity->getEntityBoundingBox().minZ));
        posset.insert(BlockPos(entity->getEntityBoundingBox().minX, (double)i, entity->getEntityBoundingBox().maxZ));
        posset.insert(BlockPos(entity->getEntityBoundingBox().maxX, (double)i, entity->getEntityBoundingBox().minZ));
        posset.insert(BlockPos(entity->getEntityBoundingBox().maxX, (double)i, entity->getEntityBoundingBox().maxZ));

        for (BlockPos blockpos : posset)
        {
            PathNodeType pathnodetype = getPathNodeType(entity, blockpos);
            if (entity->getPathPriority(pathnodetype) >= 0.0F)
            {
                return WalkNodeProcessor::openPoint(blockpos.getx(), blockpos.gety(), blockpos.getz()).value();
            }
        }
    }

    return WalkNodeProcessor::openPoint(blockpos1.getx(), i, blockpos1.getz()).value();
}

PathPoint FlyingNodeProcessor::getPathPointToCoords(double x, double y, double z)
{
    return WalkNodeProcessor::openPoint(MathHelper::floor(x), MathHelper::floor(y), MathHelper::floor(z)).value();
}

int FlyingNodeProcessor::findPathOptions(std::vector<PathPoint> &pathOptions, PathPoint currentPoint,
                                         PathPoint targetPoint, float maxDistance)
{
    int i           = 0;
    auto pathpoint  = openPoint(currentPoint.x, currentPoint.y, currentPoint.z + 1);
    auto pathpoint1 = openPoint(currentPoint.x - 1, currentPoint.y, currentPoint.z);
    auto pathpoint2 = openPoint(currentPoint.x + 1, currentPoint.y, currentPoint.z);
    auto pathpoint3 = openPoint(currentPoint.x, currentPoint.y, currentPoint.z - 1);
    auto pathpoint4 = openPoint(currentPoint.x, currentPoint.y + 1, currentPoint.z);
    auto pathpoint5 = openPoint(currentPoint.x, currentPoint.y - 1, currentPoint.z);
    if (pathpoint.has_value() && !pathpoint->visited && pathpoint->distanceTo(targetPoint) < maxDistance)
    {
        pathOptions[i++] = pathpoint.value();
    }

    if (pathpoint1.has_value() && !pathpoint1->visited && pathpoint1->distanceTo(targetPoint) < maxDistance)
    {
        pathOptions[i++] = pathpoint1.value();
    }

    if (pathpoint2.has_value() && !pathpoint2->visited && pathpoint2->distanceTo(targetPoint) < maxDistance)
    {
        pathOptions[i++] = pathpoint2.value();
    }

    if (pathpoint3.has_value() && !pathpoint3->visited && pathpoint3->distanceTo(targetPoint) < maxDistance)
    {
        pathOptions[i++] = pathpoint3.value();
    }

    if (pathpoint4.has_value() && !pathpoint4->visited && pathpoint4->distanceTo(targetPoint) < maxDistance)
    {
        pathOptions[i++] = pathpoint4.value();
    }

    if (pathpoint5.has_value() && !pathpoint5->visited && pathpoint5->distanceTo(targetPoint) < maxDistance)
    {
        pathOptions[i++] = pathpoint5.value();
    }

    bool flag  = !pathpoint3.has_value() || pathpoint3->costMalus != 0.0F;
    bool flag1 = !pathpoint.has_value() || pathpoint->costMalus != 0.0F;
    bool flag2 = !pathpoint2.has_value() || pathpoint2->costMalus != 0.0F;
    bool flag3 = !pathpoint1.has_value() || pathpoint1->costMalus != 0.0F;
    bool flag4 = !pathpoint4.has_value() || pathpoint4->costMalus != 0.0F;
    bool flag5 = !pathpoint5.has_value() || pathpoint5->costMalus != 0.0F;
    if (flag && flag3)
    {
        auto pathpoint17 = openPoint(currentPoint.x - 1, currentPoint.y, currentPoint.z - 1);
        if (pathpoint17.has_value() && !pathpoint17->visited && pathpoint17->distanceTo(targetPoint) < maxDistance)
        {
            pathOptions[i++] = pathpoint17.value();
        }
    }

    if (flag && flag2)
    {
        auto pathpoint17 = openPoint(currentPoint.x + 1, currentPoint.y, currentPoint.z - 1);
        if (pathpoint17.has_value() && !pathpoint17->visited && pathpoint17->distanceTo(targetPoint) < maxDistance)
        {
            pathOptions[i++] = pathpoint17.value();
        }
    }

    if (flag1 && flag3)
    {
        auto pathpoint17 = openPoint(currentPoint.x - 1, currentPoint.y, currentPoint.z + 1);
        if (pathpoint17.has_value() && !pathpoint17->visited && pathpoint17->distanceTo(targetPoint) < maxDistance)
        {
            pathOptions[i++] = pathpoint17.value();
        }
    }

    if (flag1 && flag2)
    {
        auto pathpoint17 = openPoint(currentPoint.x + 1, currentPoint.y, currentPoint.z + 1);
        if (pathpoint17.has_value() && !pathpoint17->visited && pathpoint17->distanceTo(targetPoint) < maxDistance)
        {
            pathOptions[i++] = pathpoint17.value();
        }
    }

    if (flag && flag4)
    {
        auto pathpoint17 = openPoint(currentPoint.x, currentPoint.y + 1, currentPoint.z - 1);
        if (pathpoint17.has_value() && !pathpoint17->visited && pathpoint17->distanceTo(targetPoint) < maxDistance)
        {
            pathOptions[i++] = pathpoint17.value();
        }
    }

    if (flag1 && flag4)
    {
        auto pathpoint17 = openPoint(currentPoint.x, currentPoint.y + 1, currentPoint.z + 1);
        if (pathpoint17.has_value() && !pathpoint17->visited && pathpoint17->distanceTo(targetPoint) < maxDistance)
        {
            pathOptions[i++] = pathpoint17.value();
        }
    }

    if (flag2 && flag4)
    {
        auto pathpoint17 = openPoint(currentPoint.x + 1, currentPoint.y + 1, currentPoint.z);
        if (pathpoint17.has_value() && !pathpoint17->visited && pathpoint17->distanceTo(targetPoint) < maxDistance)
        {
            pathOptions[i++] = pathpoint17.value();
        }
    }

    if (flag3 && flag4)
    {
        auto pathpoint17 = openPoint(currentPoint.x - 1, currentPoint.y + 1, currentPoint.z);
        if (pathpoint17.has_value() && !pathpoint17->visited && pathpoint17->distanceTo(targetPoint) < maxDistance)
        {
            pathOptions[i++] = pathpoint17.value();
        }
    }

    if (flag && flag5)
    {
        auto pathpoint17 = openPoint(currentPoint.x, currentPoint.y - 1, currentPoint.z - 1);
        if (pathpoint17.has_value() && !pathpoint17->visited && pathpoint17->distanceTo(targetPoint) < maxDistance)
        {
            pathOptions[i++] = pathpoint17.value();
        }
    }

    if (flag1 && flag5)
    {
        auto pathpoint17 = openPoint(currentPoint.x, currentPoint.y - 1, currentPoint.z + 1);
        if (pathpoint17.has_value() && !pathpoint17->visited && pathpoint17->distanceTo(targetPoint) < maxDistance)
        {
            pathOptions[i++] = pathpoint17.value();
        }
    }

    if (flag2 && flag5)
    {
        auto pathpoint17 = openPoint(currentPoint.x + 1, currentPoint.y - 1, currentPoint.z);
        if (pathpoint17.has_value() && !pathpoint17->visited && pathpoint17->distanceTo(targetPoint) < maxDistance)
        {
            pathOptions[i++] = pathpoint17.value();
        }
    }

    if (flag3 && flag5)
    {
        auto pathpoint17 = openPoint(currentPoint.x - 1, currentPoint.y - 1, currentPoint.z);
        if (pathpoint17.has_value() && !pathpoint17->visited && pathpoint17->distanceTo(targetPoint) < maxDistance)
        {
            pathOptions[i++] = pathpoint17.value();
        }
    }

    return i;
}

PathNodeType FlyingNodeProcessor::getPathNodeType(IBlockAccess *blockaccessIn, int32_t x, int32_t y, int32_t z,
                                                  EntityLiving *entitylivingIn, int32_t xSize, int32_t ySize,
                                                  int32_t zSize, bool canBreakDoorsIn, bool canEnterDoorsIn)
{
    std::set<PathNodeType> enumset;
    PathNodeType pathnodetype = PathNodeType::BLOCKED;
    BlockPos blockpos         = BlockPos(entitylivingIn);
    pathnodetype = getPathNodeType(blockaccessIn, x, y, z, xSize, ySize, zSize, canBreakDoorsIn, canEnterDoorsIn,
                                   enumset, pathnodetype, blockpos);
    if (enumset.find(PathNodeType::FENCE) != enumset.end())
    {
        return PathNodeType::FENCE;
    }
    else
    {
        PathNodeType pathnodetype1 = PathNodeType::BLOCKED;

        for (auto pathnodetype2 : enumset)
        {
            if (entitylivingIn->getPathPriority(pathnodetype2) < 0.0F)
            {
                return pathnodetype2;
            }

            if (entitylivingIn->getPathPriority(pathnodetype2) >= entitylivingIn->getPathPriority(pathnodetype1))
            {
                pathnodetype1 = pathnodetype2;
            }
        }

        if (pathnodetype == PathNodeType::OPEN && entitylivingIn->getPathPriority(pathnodetype1) == 0.0F)
        {
            return PathNodeType::OPEN;
        }
        else
        {
            return pathnodetype1;
        }
    }
}

PathNodeType FlyingNodeProcessor::getPathNodeType(IBlockAccess *blockaccessIn, int32_t x, int32_t y, int32_t z)
{
    PathNodeType pathnodetype = getPathNodeTypeRaw(blockaccessIn, x, y, z);
    if (pathnodetype == PathNodeType::OPEN && y >= 1)
    {
        Block block                = blockaccessIn->getBlockState(BlockPos(x, y - 1, z)).getBlock();
        PathNodeType pathnodetype1 = getPathNodeTypeRaw(blockaccessIn, x, y - 1, z);
        if (pathnodetype1 != PathNodeType::DAMAGE_FIRE && block != Blocks::MAGMA && pathnodetype1 != PathNodeType::LAVA)
        {
            if (pathnodetype1 == PathNodeType::DAMAGE_CACTUS)
            {
                pathnodetype = PathNodeType::DAMAGE_CACTUS;
            }
            else
            {
                pathnodetype = pathnodetype1 != PathNodeType::WALKABLE && pathnodetype1 != PathNodeType::OPEN &&
                                       pathnodetype1 != PathNodeType::WATER
                                   ? PathNodeType::WALKABLE
                                   : PathNodeType::OPEN;
            }
        }
        else
        {
            pathnodetype = PathNodeType::DAMAGE_FIRE;
        }
    }

    pathnodetype = checkNeighborBlocks(blockaccessIn, x, y, z, pathnodetype);
    return pathnodetype;
}

std::optional<PathPoint> FlyingNodeProcessor::openPoint(int32_t x, int32_t y, int32_t z)
{
    std::optional<PathPoint> pathpoint;
    auto pathnodetype = getPathNodeType(entity, x, y, z);
    float f           = entity->getPathPriority(pathnodetype);
    if (f >= 0.0F)
    {
        pathpoint            = WalkNodeProcessor::openPoint(x, y, z);
        pathpoint->nodeType  = pathnodetype;
        pathpoint->costMalus = MathHelper::max(pathpoint->costMalus, f);
        if (pathnodetype == PathNodeType::WALKABLE)
        {
            ++pathpoint->costMalus;
        }
    }

    return pathnodetype != PathNodeType::OPEN && pathnodetype != PathNodeType::WALKABLE ? pathpoint : pathpoint;
}

PathNodeType FlyingNodeProcessor::getPathNodeType(EntityLiving *p_192559_1_, BlockPos p_192559_2_)
{
    return getPathNodeType(p_192559_1_, p_192559_2_.getx(), p_192559_2_.gety(), p_192559_2_.getz());
}

PathNodeType FlyingNodeProcessor::getPathNodeType(EntityLiving *p_192558_1_, int32_t p_192558_2_, int32_t p_192558_3_,
                                                  int32_t p_192558_4_)
{
    return getPathNodeType(blockaccess, p_192558_2_, p_192558_3_, p_192558_4_, p_192558_1_, entitySizeX, entitySizeY,
                           entitySizeZ, getCanOpenDoors(), getCanEnterDoors());
}
