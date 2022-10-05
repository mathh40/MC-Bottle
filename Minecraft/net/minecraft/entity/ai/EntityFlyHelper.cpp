#include "EntityFlyHelper.h"

#include "math/MathHelper.h"

EntityFlyHelper::EntityFlyHelper(EntityLiving *p_i47418_1_) : EntityMoveHelper(p_i47418_1_)
{
}

void EntityFlyHelper::onUpdateMoveHelper()
{
    if (action == Action::MOVE_TO)
    {
        action = Action::WAIT;
        entity->setNoGravity(true);
        double d0 = posX - entity->posX;
        double d1 = posY - entity->posY;
        double d2 = posZ - entity->posZ;
        double d3 = d0 * d0 + d1 * d1 + d2 * d2;
        if (d3 < 2.500000277905201E-7)
        {
            entity->setMoveVertical(0.0F);
            entity->setMoveForward(0.0F);
            return;
        }

        float f             = (MathHelper::atan2(d2, d0) * 57.29577951308232) - 90.0F;
        entity->rotationYaw = limitAngle(entity->rotationYaw, f, 10.0F);
        float f1;
        if (entity->onGround)
        {
            f1 = (speed * entity->getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED).getAttributeValue());
        }
        else
        {
            f1 = (speed * entity->getEntityAttribute(SharedMonsterAttributes::FLYING_SPEED).getAttributeValue());
        }

        entity->setAIMoveSpeed(f1);
        double d4             = (double)MathHelper.sqrt(d0 * d0 + d2 * d2);
        float f2              = (float)(-(MathHelper.atan2(d1, d4) * 57.29577951308232D));
        entity->rotationPitch = limitAngle(entity->rotationPitch, f2, 10.0F);
        entity->setMoveVertical(d1 > 0.0 ? f1 : -f1);
    }
    else
    {
        entity->setNoGravity(false);
        entity->setMoveVertical(0.0F);
        entity->setMoveForward(0.0F);
    }
}
