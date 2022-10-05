#include "RayTraceResult.h"

RayTraceResult::RayTraceResult(Vec3d hitVecIn, EnumFacing sideHitIn, BlockPos blockPosIn)
    : RayTraceResult(Type::BLOCK, hitVecIn, sideHitIn, blockPosIn)
{
}

RayTraceResult::RayTraceResult(Vec3d hitVecIn, EnumFacing sideHitIn)
    : RayTraceResult(Type::BLOCK, hitVecIn, sideHitIn, BlockPos::ORIGIN)
{
}

RayTraceResult::RayTraceResult(Entity *entityIn)
    : RayTraceResult(entityIn, Vec3d(entityIn->posX, entityIn.posY, entityIn.posZ))
{
}

RayTraceResult::RayTraceResult(Type typeIn, Vec3d hitVecIn, EnumFacing sideHitIn, BlockPos blockPosIn)
    : typeOfHit(typeIn), blockPos(blockPosIn), sideHit(sideHitIn), hitVec(hitVecIn)
{
}

RayTraceResult::RayTraceResult(Entity *entityHitIn, Vec3d hitVecIn)
    : typeOfHit(Type::ENTITY), entityHit(entityHitIn), hitVec(hitVecIn)
{
}

BlockPos RayTraceResult::getBlockPos() const
{
    return blockPos;
}

std::string to_string(const RayTraceResult &a)
{
    return "HitResult{type=" + std::to_string(a.typeOfHit) + ", blockpos=" + std::to_string(a.getBlockPos) +
           ", f=" + std::to_string(a.sideHit) + ", pos=" + std::to_string(a.hitVec) + ", entity=" + a.entityHit + '}';
}
