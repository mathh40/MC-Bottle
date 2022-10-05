#include "RandomPositionGenerator.h"

#include "../../pathfinding/PathNavigate.h"
#include "EntityCreature.h"
#include "math/Vec3d.h"

namespace RandomPositionGenerator
{
std::optional<Vec3d> findRandomTargetBlock(EntityCreature *entitycreatureIn, int32_t xz, int32_t y,
                                           std::optional<Vec3d> targetVec3);
std::optional<Vec3d> generateRandomPos(EntityCreature *p_191379_0_, int32_t p_191379_1_, int32_t p_191379_2_,
                                       std::optional<Vec3d> p_191379_3_, bool p_191379_4_);
BlockPos moveAboveSolid(BlockPos p_191378_0_, EntityCreature *p_191378_1_);
bool isWaterDestination(BlockPos p_191380_0_, EntityCreature *p_191380_1_);

std::optional<Vec3d> findRandomTarget(EntityCreature *entitycreatureIn, int32_t xz, int32_t y)
{
    return findRandomTargetBlock(entitycreatureIn, xz, y, std::nullopt);
}

std::optional<Vec3d> getLandPos(EntityCreature *p_191377_0_, int32_t p_191377_1_, int32_t p_191377_2_)
{
    return generateRandomPos(p_191377_0_, p_191377_1_, p_191377_2_, std::nullopt, false);
}

std::optional<Vec3d> findRandomTargetBlockTowards(EntityCreature *entitycreatureIn, int32_t xz, int32_t y,
                                                  Vec3d targetVec3)
{
    auto staticVector = targetVec3.subtract(entitycreatureIn->posX, entitycreatureIn->posY, entitycreatureIn->posZ);
    return findRandomTargetBlock(entitycreatureIn, xz, y, staticVector);
}

std::optional<Vec3d> findRandomTargetBlockAwayFrom(EntityCreature *entitycreatureIn, int32_t xz, int32_t y,
                                                   Vec3d targetVec3)
{
    auto staticVector =
        Vec3d(entitycreatureIn->posX, entitycreatureIn->posY, entitycreatureIn->posZ).subtract(targetVec3);
    return findRandomTargetBlock(entitycreatureIn, xz, y, staticVector);
}

std::optional<Vec3d> findRandomTargetBlock(EntityCreature *entitycreatureIn, int32_t xz, int32_t y,
                                           std::optional<Vec3d> targetVec3)
{
    return generateRandomPos(entitycreatureIn, xz, y, targetVec3, true);
}

std::optional<Vec3d> generateRandomPos(EntityCreature *p_191379_0_, int32_t p_191379_1_, int32_t p_191379_2_,
                                       std::optional<Vec3d> p_191379_3_, bool p_191379_4_)
{
    PathNavigate *pathnavigate = p_191379_0_->getNavigator();
    pcg32 random               = p_191379_0_->getRNG();
    bool flag;
    if (p_191379_0_->hasHome())
    {
        double d0 = p_191379_0_->getHomePosition().distanceSq(MathHelper::floor(p_191379_0_->posX),
                                                              MathHelper::floor(p_191379_0_->posY),
                                                              MathHelper::floor(p_191379_0_->posZ)) +
                    4.0;
        double d1 = p_191379_0_->getMaximumHomeDistance() + p_191379_1_;
        flag      = d0 < d1 * d1;
    }
    else
    {
        flag = false;
    }

    bool flag1 = false;
    float f    = -99999.0F;
    auto k1    = 0;
    auto i     = 0;
    auto j     = 0;

    for (auto k = 0; k < 10; ++k)
    {
        auto l  = random(2 * p_191379_1_ + 1) - p_191379_1_;
        auto i1 = random(2 * p_191379_2_ + 1) - p_191379_2_;
        auto j1 = random(2 * p_191379_1_ + 1) - p_191379_1_;
        if (!p_191379_3_.has_value() || l * p_191379_3_->getx() + j1 * p_191379_3_->getz() >= 0.0)
        {
            BlockPos blockpos1;
            if (p_191379_0_->hasHome() && p_191379_1_ > 1)
            {
                blockpos1 = p_191379_0_->getHomePosition();
                if (p_191379_0_->posX > blockpos1.getx())
                {
                    l -= random(p_191379_1_ / 2);
                }
                else
                {
                    l += random(p_191379_1_ / 2);
                }

                if (p_191379_0_->posZ > blockpos1.getz())
                {
                    j1 -= random(p_191379_1_ / 2);
                }
                else
                {
                    j1 += random(p_191379_1_ / 2);
                }
            }

            blockpos1 = BlockPos(l + p_191379_0_->posX, i1 + p_191379_0_->posY, j1 + p_191379_0_->posZ);
            if ((!flag || p_191379_0_->isWithinHomeDistanceFromPosition(blockpos1)) &&
                pathnavigate->canEntityStandOnPos(blockpos1))
            {
                if (!p_191379_4_)
                {
                    blockpos1 = moveAboveSolid(blockpos1, p_191379_0_);
                    if (isWaterDestination(blockpos1, p_191379_0_))
                    {
                        continue;
                    }
                }

                float f1 = p_191379_0_->getBlockPathWeight(blockpos1);
                if (f1 > f)
                {
                    f     = f1;
                    k1    = l;
                    i     = i1;
                    j     = j1;
                    flag1 = true;
                }
            }
        }
    }

    if (flag1)
    {
        return Vec3d(k1 + p_191379_0_->posX, i + p_191379_0_->posY, j + p_191379_0_->posZ);
    }
    else
    {
        return std::nullopt;
    }
}

BlockPos moveAboveSolid(BlockPos p_191378_0_, EntityCreature *p_191378_1_)
{
    if (!p_191378_1_->world->getBlockState(p_191378_0_)->getMaterial().isSolid())
    {
        return p_191378_0_;
    }
    else
    {
        BlockPos blockpos;
        for (blockpos = p_191378_0_.up(); blockpos.gety() < p_191378_1_->world->getHeight() &&
                                          p_191378_1_->world->getBlockState(blockpos)->getMaterial().isSolid();
             blockpos = blockpos.up())
        {
        }

        return blockpos;
    }
}

bool isWaterDestination(BlockPos p_191380_0_, EntityCreature *p_191380_1_)
{
    return p_191380_1_->world->getBlockState(p_191380_0_)->getMaterial() == Material::WATER;
}
} // namespace RandomPositionGenerator
