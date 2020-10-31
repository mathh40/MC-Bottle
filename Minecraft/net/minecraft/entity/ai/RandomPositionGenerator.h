#pragma once
#include "math/Vec3d.h"
class EntityCreature;
namespace RandomPositionGenerator
{
    std::optional<Vec3d> findRandomTarget(EntityCreature* entitycreatureIn, int32_t xz, int32_t y);
    std::optional<Vec3d> getLandPos(EntityCreature* p_191377_0_, int32_t p_191377_1_, int32_t p_191377_2_);
    std::optional<Vec3d> findRandomTargetBlockTowards(EntityCreature* entitycreatureIn, int32_t xz, int32_t y, Vec3d targetVec3);
    std::optional<Vec3d> findRandomTargetBlockAwayFrom(EntityCreature* entitycreatureIn, int32_t xz, int32_t y, Vec3d targetVec3);
}
