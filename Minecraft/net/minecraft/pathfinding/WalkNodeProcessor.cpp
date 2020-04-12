#include "WalkNodeProcessor.h"

void WalkNodeProcessor::init(IBlockAccess *sourceIn, EntityLiving *mob)
{
    NodeProcessor::init(sourceIn, mob);
    avoidsWater = mob->getPathPriority(PathNodeType::WATER);
}

void WalkNodeProcessor::postProcess()
{
    entity->setPathPriority(PathNodeType::WATER, avoidsWater);
    NodeProcessor::postProcess();
}

PathPoint WalkNodeProcessor::getStart()
{
    int32_t i = 0;
    BlockPos blockpos;
    if (getCanSwim() && entity->isInWater()) 
    {
        i = (int)entity->getEntityBoundingBox().minY;
        BlockPos blockpos$mutableblockpos = BlockPos(MathHelper::floor(entity->posX), i, MathHelper::floor(entity->posZ));

        for(Block block = blockaccess->getBlockState(blockpos$mutableblockpos).getBlock(); block == Blocks::FLOWING_WATER || block == Blocks::WATER; block = blockaccess->getBlockState(blockpos$mutableblockpos).getBlock()) 
        {
            ++i;
            blockpos$mutableblockpos.setPos(MathHelper::floor(entity->posX), i, MathHelper::floor(entity->posZ));
        }
    }
    else if (entity->onGround) 
    {
        i = MathHelper::floor(entity->getEntityBoundingBox().minY + 0.5);
    }
    else 
    {
        for(blockpos = BlockPos(entity); (blockaccess->getBlockState(blockpos)->getMaterial() == Material::AIR || blockaccess->getBlockState(blockpos).getBlock().isPassable(blockaccess, blockpos)) && blockpos.gety() > 0; blockpos = blockpos.down()) 
        {
        }

        i = blockpos.up().gety();
    }

    blockpos = BlockPos(entity);
    PathNodeType pathnodetype1 = getPathNodeType(entity, blockpos.getx(), i, blockpos.getz());
    if (entity->getPathPriority(pathnodetype1) < 0.0F) 
    {
        std::set<BlockPos> set;
        set.emplace(BlockPos(entity->getEntityBoundingBox().minX, (double)i, entity->getEntityBoundingBox().minZ));
        set.emplace(BlockPos(entity->getEntityBoundingBox().minX, (double)i, entity->getEntityBoundingBox().maxZ));
        set.emplace(BlockPos(entity->getEntityBoundingBox().maxX, (double)i, entity->getEntityBoundingBox().minZ));
        set.emplace(BlockPos(entity->getEntityBoundingBox().maxX, (double)i, entity->getEntityBoundingBox().maxZ));

        for(auto blockpos1 : set) 
        {
            PathNodeType pathnodetype = getPathNodeType(entity, blockpos1);
            if (entity->getPathPriority(pathnodetype) >= 0.0F) 
            {
                return openPoint(blockpos1.getx(), blockpos1.gety(), blockpos1.getz());
            }
        }
    }

    return openPoint(blockpos.getx(), i, blockpos.getz());
}

PathPoint WalkNodeProcessor::getPathPointToCoords(double x, double y, double z)
{
    return openPoint(MathHelper::floor(x), MathHelper::floor(y), MathHelper::floor(z));
}

int32_t WalkNodeProcessor::findPathOptions(std::vector<PathPoint> &pathOptions, PathPoint currentPoint,
    PathPoint targetPoint, float maxDistance)
{
    int32_t i = 0;
    int32_t j = 0;
    PathNodeType pathnodetype = getPathNodeType(entity, currentPoint.x, currentPoint.y + 1, currentPoint.z);
    if (entity->getPathPriority(pathnodetype) >= 0.0F) {
        j = MathHelper::floor(MathHelper::max(1.0F, entity->stepHeight));
    }

    BlockPos blockpos = (BlockPos(currentPoint.x, currentPoint.y, currentPoint.z)).down();
    double d0 = (double)currentPoint.y - (1.0 - blockaccess->getBlockState(blockpos).getBoundingBox(blockaccess, blockpos).maxY);
    PathPoint pathpoint = getSafePoint(currentPoint.x, currentPoint.y, currentPoint.z + 1, j, d0, EnumFacing::SOUTH);
    PathPoint pathpoint1 = getSafePoint(currentPoint.x - 1, currentPoint.y, currentPoint.z, j, d0, EnumFacing::WEST);
    PathPoint pathpoint2 = getSafePoint(currentPoint.x + 1, currentPoint.y, currentPoint.z, j, d0, EnumFacing::EAST);
    PathPoint pathpoint3 = getSafePoint(currentPoint.x, currentPoint.y, currentPoint.z - 1, j, d0, EnumFacing::NORTH);
    if (pathpoint != null && !pathpoint.visited && pathpoint.distanceTo(targetPoint) < maxDistance) 
    {
        pathOptions[i++] = pathpoint;
    }

    if (pathpoint1 != null && !pathpoint1.visited && pathpoint1.distanceTo(targetPoint) < maxDistance) 
    {
        pathOptions[i++] = pathpoint1;
    }

    if (pathpoint2 != null && !pathpoint2.visited && pathpoint2.distanceTo(targetPoint) < maxDistance) 
    {
        pathOptions[i++] = pathpoint2;
    }

    if (pathpoint3 != null && !pathpoint3.visited && pathpoint3.distanceTo(targetPoint) < maxDistance) 
    {
        pathOptions[i++] = pathpoint3;
    }

    bool flag = pathpoint3 == null || pathpoint3.nodeType == PathNodeType::OPEN || pathpoint3.costMalus != 0.0F;
    bool flag1 = pathpoint == null || pathpoint.nodeType == PathNodeType::OPEN || pathpoint.costMalus != 0.0F;
    bool flag2 = pathpoint2 == null || pathpoint2.nodeType == PathNodeType::OPEN || pathpoint2.costMalus != 0.0F;
    bool flag3 = pathpoint1 == null || pathpoint1.nodeType == PathNodeType::OPEN || pathpoint1.costMalus != 0.0F;
    PathPoint pathpoint7;
    if (flag && flag3) 
    {
        pathpoint7 = getSafePoint(currentPoint.x - 1, currentPoint.y, currentPoint.z - 1, j, d0, EnumFacing::NORTH);
        if (pathpoint7 != null && !pathpoint7.visited && pathpoint7.distanceTo(targetPoint) < maxDistance) 
        {
            pathOptions[i++] = pathpoint7;
        }
    }

    if (flag && flag2) 
    {
        pathpoint7 = getSafePoint(currentPoint.x + 1, currentPoint.y, currentPoint.z - 1, j, d0, EnumFacing::NORTH);
        if (pathpoint7 != null && !pathpoint7.visited && pathpoint7.distanceTo(targetPoint) < maxDistance) 
        {
            pathOptions[i++] = pathpoint7;
        }
    }

    if (flag1 && flag3) 
    {
        pathpoint7 = getSafePoint(currentPoint.x - 1, currentPoint.y, currentPoint.z + 1, j, d0, EnumFacing::SOUTH);
        if (pathpoint7 != null && !pathpoint7.visited && pathpoint7.distanceTo(targetPoint) < maxDistance) 
        {
            pathOptions[i++] = pathpoint7;
        }
    }

    if (flag1 && flag2) 
    {
        pathpoint7 = getSafePoint(currentPoint.x + 1, currentPoint.y, currentPoint.z + 1, j, d0, EnumFacing::SOUTH);
        if (pathpoint7 != null && !pathpoint7.visited && pathpoint7.distanceTo(targetPoint) < maxDistance) 
        {
            pathOptions[i++] = pathpoint7;
        }
    }

    return i;
}

PathNodeType WalkNodeProcessor::getPathNodeType(IBlockAccess *blockaccessIn, int32_t x, int32_t y, int32_t z,
    EntityLiving *entitylivingIn, int32_t xSize, int32_t ySize, int32_t zSize, bool canBreakDoorsIn,
    bool canEnterDoorsIn)
{
    std::set<PathNodeType> enumset;
    PathNodeType pathnodetype = PathNodeType::BLOCKED;
    double d0 = (double)entitylivingIn->width / 2.0;
    BlockPos blockpos = BlockPos(entitylivingIn);
    pathnodetype = getPathNodeType(blockaccessIn, x, y, z, xSize, ySize, zSize, canBreakDoorsIn, canEnterDoorsIn, enumset, pathnodetype, blockpos);
    if (enumset.contains(PathNodeType::FENCE)) 
    {
        return PathNodeType::FENCE;
    }
    else 
    {
        PathNodeType pathnodetype1 = PathNodeType::BLOCKED;

        for(auto pathnodetype2 : enumset)
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

PathNodeType WalkNodeProcessor::getPathNodeType(IBlockAccess* p_193577_1_, int32_t x, int32_t y, int32_t z, int32_t xSize, int32_t ySize, int32_t zSize, bool canOpenDoorsIn, bool canEnterDoorsIn, EnumSet p_193577_10_, PathNodeType p_193577_11_, BlockPos p_193577_12_)
{
    for(int32_t i = 0; i < xSize; ++i) 
    {
        for(int32_t j = 0; j < ySize; ++j) 
        {
            for(int32_t k = 0; k < zSize; ++k) 
            {
                int32_t l = i + x;
                int32_t i1 = j + y;
                int32_t j1 = k + z;
                PathNodeType pathnodetype = getPathNodeType(p_193577_1_, l, i1, j1);
                if (pathnodetype == PathNodeType::DOOR_WOOD_CLOSED && canOpenDoorsIn && canEnterDoorsIn) {
                    pathnodetype = PathNodeType::WALKABLE;
                }

                if (pathnodetype == PathNodeType::DOOR_OPEN && !canEnterDoorsIn) {
                    pathnodetype = PathNodeType::BLOCKED;
                }

                if (pathnodetype == PathNodeType::RAIL && !(p_193577_1_.getBlockState(p_193577_12_).getBlock() instanceof BlockRailBase) && !(p_193577_1_.getBlockState(p_193577_12_.down()).getBlock() instanceof BlockRailBase)) {
                    pathnodetype = PathNodeType::FENCE;
                }

                if (i == 0 && j == 0 && k == 0) {
                    p_193577_11_ = pathnodetype;
                }

                p_193577_10_.add(pathnodetype);
            }
        }
    }

    return p_193577_11_;
}

PathNodeType WalkNodeProcessor::getPathNodeType(IBlockAccess *blockaccessIn, int32_t x, int32_t y, int32_t z)
{
    PathNodeType pathnodetype = getPathNodeTypeRaw(blockaccessIn, x, y, z);
    if (pathnodetype == PathNodeType::OPEN && y >= 1) 
    {
        Block block = blockaccessIn->getBlockState(BlockPos(x, y - 1, z)).getBlock();
        PathNodeType pathnodetype1 = getPathNodeTypeRaw(blockaccessIn, x, y - 1, z);
        pathnodetype = pathnodetype1 != PathNodeType::WALKABLE && pathnodetype1 != PathNodeType::OPEN && pathnodetype1 != PathNodeType::WATER && pathnodetype1 != PathNodeType::LAVA ? PathNodeType::WALKABLE : PathNodeType::OPEN;
        if (pathnodetype1 == PathNodeType::DAMAGE_FIRE || block == Blocks::MAGMA) 
        {
            pathnodetype = PathNodeType::DAMAGE_FIRE;
        }

        if (pathnodetype1 == PathNodeType::DAMAGE_CACTUS) 
        {
            pathnodetype = PathNodeType::DAMAGE_CACTUS;
        }
    }

    pathnodetype = checkNeighborBlocks(blockaccessIn, x, y, z, pathnodetype);
    return pathnodetype;
}

PathNodeType WalkNodeProcessor::checkNeighborBlocks(IBlockAccess *p_193578_1_, int32_t p_193578_2_, int32_t p_193578_3_,
    int32_t p_193578_4_, PathNodeType p_193578_5_)
{
    BlockPos blockpos$pooledmutableblockpos;
    if (p_193578_5_ == PathNodeType::WALKABLE) 
    {
        for(int32_t i = -1; i <= 1; ++i) 
        {
            for(int32_t j = -1; j <= 1; ++j) 
            {
                if (i != 0 || j != 0) 
                {
                    auto block = p_193578_1_->getBlockState(blockpos$pooledmutableblockpos.setPos(i + p_193578_2_, p_193578_3_, j + p_193578_4_)).getBlock();
                    if (block == Blocks::CACTUS) 
                    {
                        p_193578_5_ = PathNodeType::DANGER_CACTUS;
                    } else if (block == Blocks::FIRE) 
                    {
                        p_193578_5_ = PathNodeType::DANGER_FIRE;
                    }
                }
            }
        }
    }

    blockpos$pooledmutableblockpos.release();
    return p_193578_5_;
}

PathNodeType WalkNodeProcessor::getPathNodeTypeRaw(IBlockAccess *p_189553_1_, int32_t p_189553_2_, int32_t p_189553_3_,
    int32_t p_189553_4_)
{
    BlockPos blockpos = BlockPos(p_189553_2_, p_189553_3_, p_189553_4_);
    IBlockState* iblockstate = p_189553_1_->getBlockState(blockpos);
    Block* block = iblockstate->getBlock();
    Material material = iblockstate->getMaterial();
    if (material == Material::AIR) 
    {
        return PathNodeType::OPEN;
    }
    else if (block != Blocks::TRAPDOOR && block != Blocks::IRON_TRAPDOOR && block != Blocks::WATERLILY) 
    {
        if (block == Blocks::FIRE) 
        {
            return PathNodeType::DAMAGE_FIRE;
        }
        else if (block == Blocks::CACTUS) 
        {
            return PathNodeType::DAMAGE_CACTUS;
        }
        else if (Util::instanceof< BlockDoor>(block) && material == Material::WOOD && !iblockstate->getValue(BlockDoor::OPEN)) 
        {
            return PathNodeType::DOOR_WOOD_CLOSED;
        }
        else if (instanceof<BlockDoor>(block) && material == Material::IRON && !iblockstate->getValue(BlockDoor::OPEN)) 
        {
            return PathNodeType::DOOR_IRON_CLOSED;
        }
        else if (instanceof<BlockDoor>(block) && iblockstate->getValue(BlockDoorM::OPEN)) 
        {
            return PathNodeType::DOOR_OPEN;
        }
        else if (instanceof<BlockRailBase>(block)) 
        {
            return PathNodeType::RAIL;
        }
        else if (!(instanceof<BlockFence>(block)) && !(instanceof<BlockWall>(block)) && (!(instanceof<BlockFenceGate>(block)) || iblockstate->getValue(BlockFenceGate::OPEN))) 
        {
            if (material == Material::WATER) 
            {
                return PathNodeType::WATER;
            }
            else if (material == Material::LAVA) 
            {
                return PathNodeType::LAVA;
            }
            else 
            {
                return block->isPassable(p_189553_1_, blockpos) ? PathNodeType::OPEN : PathNodeType::BLOCKED;
            }
        }
        else 
        {
            return PathNodeType::FENCE;
        }
    }
    else 
    {
        return PathNodeType::TRAPDOOR;
    }
}

std::optional<PathPoint> WalkNodeProcessor::getSafePoint(int32_t x, int32_t y, int32_t z, int32_t p_186332_4_,
                                                         double p_186332_5_, EnumFacing facing)
{
    std::optional<PathPoint> pathpoint = std::nullopt;
    BlockPos blockpos = BlockPos(x, y, z);
    BlockPos blockpos1 = blockpos.down();
    double d0 = (double)y - (1.0 - blockaccess->getBlockState(blockpos1)->getBoundingBox(blockaccess, blockpos1).maxY);
    if (d0 - p_186332_5_ > 1.125) 
    {
        return std::nullopt;
    }
    else 
    {
        PathNodeType pathnodetype = getPathNodeType(entity, x, y, z);
        float f = entity->getPathPriority(pathnodetype);
        double d1 = (double)entity->width / 2.0;
        if (f >= 0.0F) 
        {
            pathpoint = openPoint(x, y, z);
            pathpoint->nodeType = pathnodetype;
            pathpoint->costMalus = MathHelper::max(pathpoint->costMalus, f);
        }

        if (pathnodetype == PathNodeType::WALKABLE) 
        {
            return pathpoint;
        }
        else 
        {
            if (pathpoint == std::nullopt && p_186332_4_ > 0 && pathnodetype != PathNodeType::FENCE && pathnodetype != PathNodeType::TRAPDOOR) 
            {
                pathpoint = getSafePoint(x, y + 1, z, p_186332_4_ - 1, p_186332_5_, facing);
                if (pathpoint.has_value() && (pathpoint->nodeType == PathNodeType::OPEN || pathpoint->nodeType == PathNodeType::WALKABLE) && entity->width < 1.0F) 
                {
                    double d2 = (double)(x - facing.getXOffset()) + 0.5;
                    double d3 = (double)(z - facing.getZOffset()) + 0.5;
                    AxisAlignedBB axisalignedbb = AxisAlignedBB(d2 - d1, (double)y + 0.001, d3 - d1, d2 + d1, (double)((float)y + entity->height), d3 + d1);
                    AxisAlignedBB axisalignedbb1 = blockaccess->getBlockState(blockpos)->getBoundingBox(blockaccess, blockpos);
                    AxisAlignedBB axisalignedbb2 = axisalignedbb.expand(0.0, axisalignedbb1.maxY - 0.002, 0.0);
                    if (entity->world.collidesWithAnyBlock(axisalignedbb2)) 
                    {
                        pathpoint = std::nullopt;
                    }
                }
            }

            if (pathnodetype == PathNodeType::OPEN) {
                AxisAlignedBB axisalignedbb3 = AxisAlignedBB((double)x - d1 + 0.5, (double)y + 0.001, (double)z - d1 + 0.5, (double)x + d1 + 0.5, (double)((float)y + entity->height), (double)z + d1 + 0.5);
                if (entity->world.collidesWithAnyBlock(axisalignedbb3)) {
                    return std::nullopt;
                }

                if (entity->width >= 1.0F) 
                {
                    PathNodeType pathnodetype1 = getPathNodeType(entity, x, y - 1, z);
                    if (pathnodetype1 == PathNodeType::BLOCKED) {
                        pathpoint = openPoint(x, y, z);
                        pathpoint->nodeType = PathNodeType::WALKABLE;
                        pathpoint->costMalus = MathHelper::max(pathpoint->costMalus, f);
                        return pathpoint;
                    }
                }

                int var25 = 0;

                while(y > 0 && pathnodetype == PathNodeType::OPEN) {
                    --y;
                    if (var25++ >= entity->getMaxFallHeight()) 
                    {
                        return std::nullopt;
                    }

                    pathnodetype = getPathNodeType(entity, x, y, z);
                    f = entity->getPathPriority(pathnodetype);
                    if (pathnodetype != PathNodeType::OPEN && f >= 0.0F) {
                        pathpoint = openPoint(x, y, z);
                        pathpoint->nodeType = pathnodetype;
                        pathpoint->costMalus = MathHelper::max(pathpoint->costMalus, f);
                        break;
                    }

                    if (f < 0.0F) 
                    {
                        return std::nullopt;
                    }
                }
            }

            return pathpoint;
        }
    }
}

PathNodeType WalkNodeProcessor::getPathNodeType(EntityLiving *entitylivingIn, BlockPos pos)
{
    return getPathNodeType(entitylivingIn, pos.getx(), pos.gety(), pos.getz());
}

PathNodeType WalkNodeProcessor::getPathNodeType(EntityLiving *entitylivingIn, int32_t x, int32_t y, int32_t z)
{
    return getPathNodeType(blockaccess, x, y, z, entitylivingIn, entitySizeX, entitySizeY, entitySizeZ, getCanOpenDoors(), getCanEnterDoors());
}
