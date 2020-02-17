#pragma once
#include "BlockPos.h"
#include "enum.h"
#include "Vec3d.h"
#include "EnumFacing.h"

BETTER_ENUM(Type, uint8_t, MISS, BLOCK, ENTITY);

class RayTraceResult
{
public:
	Type typeOfHit;
	EnumFacing sideHit;
	Vec3d hitVec;
	Entity entityHit;

	RayTraceResult(Vec3d hitVecIn, EnumFacing sideHitIn, BlockPos blockPosIn);
	RayTraceResult(Vec3d hitVecIn, EnumFacing sideHitIn);
	RayTraceResult(Entity entityIn);
	RayTraceResult(Type typeIn, Vec3d hitVecIn, EnumFacing sideHitIn, BlockPos blockPosIn);
	RayTraceResult(Entity entityHitIn, Vec3d hitVecIn);
	BlockPos getBlockPos() const;
private:
	BlockPos blockPos;
};

std::string to_string(const RayTraceResult &a);
