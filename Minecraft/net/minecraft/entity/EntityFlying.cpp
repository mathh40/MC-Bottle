#include "EntityFlying.h"

EntityFlying::EntityFlying(World *worldIn) : EntityLiving(worldIn)
{
}

void EntityFlying::fall(float distance, float damageMultiplier)
{
}

void EntityFlying::travel(float strafe, float vertical, float forward)
{
    if (isInWater())
    {
        moveRelative(strafe, vertical, forward, 0.02F);
        move(MoverType::SELF, motionX, motionY, motionZ);
        motionX *= 0.800000011920929;
        motionY *= 0.800000011920929;
        motionZ *= 0.800000011920929;
    }
    else if (isInLava())
    {
        moveRelative(strafe, vertical, forward, 0.02F);
        move(MoverType::SELF, motionX, motionY, motionZ);
        motionX *= 0.5;
        motionY *= 0.5;
        motionZ *= 0.5;
    }
    else
    {
        float f = 0.91F;
        if (onGround)
        {
            f = world
                    ->getBlockState(BlockPos(MathHelper::floor(posX),
                                             MathHelper::floor(getEntityBoundingBox().getminY()) - 1,
                                             MathHelper::floor(posZ)))
                    ->getBlock()
                    ->slipperiness *
                0.91F;
        }

        float f1 = 0.16277136F / (f * f * f);
        moveRelative(strafe, vertical, forward, onGround ? 0.1F * f1 : 0.02F);
        f = 0.91F;
        if (onGround)
        {
            f = world
                    ->getBlockState(BlockPos(MathHelper::floor(posX),
                                             MathHelper::floor(getEntityBoundingBox().getminY()) - 1,
                                             MathHelper::floor(posZ)))
                    ->getBlock()
                    ->slipperiness *
                0.91F;
        }

        move(MoverType::SELF, motionX, motionY, motionZ);
        motionX *= (double)f;
        motionY *= (double)f;
        motionZ *= (double)f;
    }

    prevLimbSwingAmount = limbSwingAmount;
    double d1           = posX - prevPosX;
    double d0           = posZ - prevPosZ;
    float f2            = MathHelper::sqrt(d1 * d1 + d0 * d0) * 4.0F;
    if (f2 > 1.0F)
    {
        f2 = 1.0F;
    }

    limbSwingAmount += (f2 - limbSwingAmount) * 0.4F;
    limbSwing += limbSwingAmount;
}

bool EntityFlying::isOnLadder()
{
    return false;
}

void EntityFlying::updateFallState(double y, bool onGroundIn, IBlockState *state, BlockPos pos)
{
}
