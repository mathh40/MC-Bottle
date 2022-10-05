#include "EntityCreature.h"

AttributeModifier EntityCreature::FLEEING_SPEED_MODIFIER =
    AttributeModifier(FLEEING_SPEED_MODIFIER_UUID, "Fleeing speed bonus", 2.0, 2).setSaved(false);

EntityCreature::EntityCreature(World *worldIn)
    : EntityLiving(worldIn), homePosition(BlockPos::ORIGIN), maximumHomeDistance(-1.0f),
      restoreWaterCost(PathNodeType::WATER.getPriority())
{
}

float EntityCreature::getBlockPathWeight(BlockPos pos)
{
    return 0.0F;
}

bool EntityCreature::getCanSpawnHere()
{
    return EntityLiving::getCanSpawnHere() &&
           getBlockPathWeight(BlockPos(posX, getEntityBoundingBox().getminY(), posZ)) >= 0.0F;
}

bool EntityCreature::hasPath() const
{
    return !navigator->noPath();
}

bool EntityCreature::isWithinHomeDistanceCurrentPosition() const
{
    return isWithinHomeDistanceFromPosition(BlockPos(this));
}

bool EntityCreature::isWithinHomeDistanceFromPosition(BlockPos pos) const
{
    if (maximumHomeDistance == -1.0F)
    {
        return true;
    }
    else
    {
        return homePosition.distanceSq(pos) < (double)(maximumHomeDistance * maximumHomeDistance);
    }
}

void EntityCreature::setHomePosAndDistance(BlockPos pos, int32_t distance)
{
    homePosition        = pos;
    maximumHomeDistance = (float)distance;
}

BlockPos EntityCreature::getHomePosition() const
{
    return homePosition;
}

float EntityCreature::getMaximumHomeDistance() const
{
    return maximumHomeDistance;
}

void EntityCreature::detachHome()
{
    maximumHomeDistance = -1.0F;
}

bool EntityCreature::hasHome() const
{
    return maximumHomeDistance != -1.0F;
}

void EntityCreature::updateLeashedState()
{
    EntityLiving::updateLeashedState();
    if (getLeashed() && getLeashHolder() != nullptr && getLeashHolder()->world == world)
    {
        auto entity = getLeashHolder();
        setHomePosAndDistance(BlockPos(entity->posX, entity->posY, entity->posZ), 5);
        float f = getDistance(entity);
        if (Util:: instanceof <EntityTameable>(this) && ((EntityTameable *)this)->isSitting())
        {
            if (f > 10.0F)
            {
                clearLeashed(true, true);
            }

            return;
        }

        onLeashDistance(f);
        if (f > 10.0F)
        {
            clearLeashed(true, true);
            tasks.disableControlFlag(1);
        }
        else if (f > 6.0F)
        {
            double d0 = (entity->posX - posX) / (double)f;
            double d1 = (entity->posY - posY) / (double)f;
            double d2 = (entity->posZ - posZ) / (double)f;
            motionX += d0 * MathHelper::abs(d0) * 0.4;
            motionY += d1 * MathHelper::abs(d1) * 0.4;
            motionZ += d2 * MathHelper::abs(d2) * 0.4;
        }
        else
        {
            tasks.enableControlFlag(1);
            float f1 = 2.0F;
            Vec3d vec3d(entity->posX - posX, entity->posY - posY, entity->posZ - posZ);
            vec3d = vec3d.normalize().scale((double)MathHelper::max(f - 2.0F, 0.0F));
            getNavigator()->tryMoveToXYZ(posX + vec3d.getx(), posY + vec3d.gety(), posZ + vec3d.getz(),
                                         followLeashSpeed());
        }
    }
}

double EntityCreature::followLeashSpeed()
{
    return 1.0;
}

void EntityCreature::onLeashDistance(float p_142017_1_)
{
}
