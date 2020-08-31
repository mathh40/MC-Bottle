#pragma once
#include "BlockPos.h"
#include "Vec3d.h"
#include "EnumFacing.h"

enum class TraceType : uint8_t
{
    MISS, BLOCK, ENTITY
};

class RayTraceResult
{
public:
    TraceType typeOfHit;
	EnumFacing sideHit;
	Vec3d hitVec;
	Entity* entityHit;

	RayTraceResult(Vec3d hitVecIn, EnumFacing sideHitIn, BlockPos blockPosIn);
	RayTraceResult(Vec3d hitVecIn, EnumFacing sideHitIn);
	RayTraceResult(Entity* entityIn);
	RayTraceResult(TraceType typeIn, Vec3d hitVecIn, EnumFacing sideHitIn, BlockPos blockPosIn);
	RayTraceResult(Entity* entityHitIn, Vec3d hitVecIn);
	BlockPos getBlockPos() const;
private:
	BlockPos blockPos;
};

std::string to_string(const RayTraceResult &a);
