#include "EntityLookHelper.h"

#include "Util.h"
#include "biome/Biome.h"

EntityLookHelper::EntityLookHelper(EntityLiving *entitylivingIn) : entity(entitylivingIn)
{
}

void EntityLookHelper::setLookPositionWithEntity(Entity *entityIn, float deltaYaw, float deltaPitch)
{
    posX = entityIn->posx;
    if (Util:: instanceof <EntityLivingBase>(entityIn))
    {
        posY = entityIn->posY + entityIn->getEyeHeight();
    }
    else
    {
        posY = (entityIn->getEntityBoundingBox().minY + entityIn->getEntityBoundingBox().maxY) / 2.0;
    }

    posZ           = entityIn->posZ;
    deltaLookYaw   = deltaYaw;
    deltaLookPitch = deltaPitch;
    isLooking      = true;
}

void EntityLookHelper::setLookPosition(double x, double y, double z, float deltaYaw, float deltaPitch)
{
    posX           = x;
    posY           = y;
    posZ           = z;
    deltaLookYaw   = deltaYaw;
    deltaLookPitch = deltaPitch;
    isLooking      = true;
}

void EntityLookHelper::onUpdateLook()
{
    entity->rotationPitch = 0.0F;
    if (isLooking)
    {
        isLooking               = false;
        double d0               = posX - entity->posX;
        double d1               = posY - (entity->posY + entity->getEyeHeight());
        double d2               = posZ - entity->posZ;
        double d3               = MathHelper::sqrt(d0 * d0 + d2 * d2);
        float f                 = (MathHelper::atan2(d2, d0) * 57.29577951308232) - 90.0F;
        float f1                = (-(MathHelper::atan2(d1, d3) * 57.29577951308232));
        entity->rotationPitch   = updateRotation(entity->rotationPitch, f1, deltaLookPitch);
        entity->rotationYawHead = updateRotation(entity->rotationYawHead, f, deltaLookYaw);
    }
    else
    {
        entity->rotationYawHead = updateRotation(entity->rotationYawHead, entity->renderYawOffset, 10.0F);
    }

    float f2 = MathHelper.wrapDegrees(entity->rotationYawHead - entity->renderYawOffset);
    if (!entity->getNavigator().noPath())
    {
        if (f2 < -75.0F)
        {
            entity->rotationYawHead = entity->renderYawOffset - 75.0F;
        }

        if (f2 > 75.0F)
        {
            entity->rotationYawHead = entity->renderYawOffset + 75.0F;
        }
    }
}

bool EntityLookHelper::getIsLooking() const
{
    return isLooking;
}

double EntityLookHelper::getLookPosX() const
{
    return posX;
}

double EntityLookHelper::getLookPosY() const
{
    return posY;
}

double EntityLookHelper::getLookPosZ() const
{
    return posZ;
}

float EntityLookHelper::updateRotation(float p_75652_1_, float p_75652_2_, float p_75652_3_) const
{
    float f = MathHelper::wrapDegrees(p_75652_2_ - p_75652_1_);
    if (f > p_75652_3_)
    {
        f = p_75652_3_;
    }

    if (f < -p_75652_3_)
    {
        f = -p_75652_3_;
    }

    return p_75652_1_ + f;
}
