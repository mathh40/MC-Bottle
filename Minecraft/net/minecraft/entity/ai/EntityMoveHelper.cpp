#include "EntityMoveHelper.h"


#include "../../pathfinding/PathNavigate.h"
#include "math/MathHelper.h"

EntityMoveHelper::EntityMoveHelper(EntityLiving *entitylivingIn)
    :action(Action::WAIT),entity(entitylivingIn)
{
}

bool EntityMoveHelper::isUpdating() const
{
    return action == Action::MOVE_TO;
}

double EntityMoveHelper::getSpeed() const
{
    return speed;
}

void EntityMoveHelper::setMoveTo(double x, double y, double z, double speedIn)
{
    posX = x;
    posY = y;
    posZ = z;
    speed = speedIn;
    action = Action::MOVE_TO;
}

void EntityMoveHelper::strafe(float forward, float strafe)
{
    action = Action::STRAFE;
    moveForward = forward;
    moveStrafe = strafe;
    speed = 0.25;
}

void EntityMoveHelper::read(const EntityMoveHelper &that)
{
    action = that.action;
    posX = that.posX;
    posY = that.posY;
    posZ = that.posZ;
    speed = MathHelper::max(that.speed, 1.0);
    moveForward = that.moveForward;
    moveStrafe = that.moveStrafe;
}

void EntityMoveHelper::onUpdateMoveHelper()
{
    if (action == Action::STRAFE) 
    {
        float f = entity->getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED).getAttributeValue();
        float f1 = speed * f;
        float f2 = moveForward;
        float f3 = moveStrafe;
        float f4 = MathHelper::sqrt(f2 * f2 + f3 * f3);
        if (f4 < 1.0F) 
        {
            f4 = 1.0F;
        }

        f4 = f1 / f4;
        f2 *= f4;
        f3 *= f4;
        float f5 = MathHelper::sin(entity->rotationYaw * 0.017453292F);
        float f6 = MathHelper::cos(entity->rotationYaw * 0.017453292F);
        float f7 = f2 * f6 - f3 * f5;
        float f8 = f3 * f6 + f2 * f5;
        PathNavigate* pathnavigate = entity->getNavigator();
        if (pathnavigate != nullptr) 
        {
            NodeProcessor* nodeprocessor = pathnavigate->getNodeProcessor();
            if (nodeprocessor != nullptr && nodeprocessor->getPathNodeType(entity->world, MathHelper::floor(entity->posX + f7), MathHelper::floor(entity->posY), MathHelper::floor(entity->posZ + f8)) != PathNodeType::WALKABLE) 
            {
                moveForward = 1.0F;
                moveStrafe = 0.0F;
                f1 = f;
            }
        }

        entity->setAIMoveSpeed(f1);
        entity->setMoveForward(moveForward);
        entity->setMoveStrafing(moveStrafe);
        action = Action::WAIT;
    }
    else if (action == Action::MOVE_TO) 
    {
        action = Action::WAIT;
        double d0 = posX - entity->posX;
        double d1 = posZ - entity->posZ;
        double d2 = posY - entity->posY;
        double d3 = d0 * d0 + d2 * d2 + d1 * d1;
        if (d3 < 2.500000277905201E-7) 
        {
            entity->setMoveForward(0.0F);
            return;
        }

        float f8 = MathHelper::atan2(d1, d0) * 57.29577951308232 - 90.0F;
        entity->rotationYaw = limitAngle(entity->rotationYaw, f8, 90.0F);
        entity->setAIMoveSpeed((speed * entity->getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED).getAttributeValue()));
        if (d2 > entity->stepHeight && d0 * d0 + d1 * d1 < MathHeelper::max(1.0F, entity->width)) 
        {
            entity->getJumpHelper().setJumping();
            action = Action::JUMPING;
        }
    }
    else if (action == Action::JUMPING) 
    {
        entity->setAIMoveSpeed((speed * entity->getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED).getAttributeValue()));
        if (entity->onGround) 
        {
            action = Action::WAIT;
        }
    }
    else 
    {
        entity->setMoveForward(0.0F);
    }
}

double EntityMoveHelper::getX() const
{
    return posX;
}

double EntityMoveHelper::getY() const
{
    return posY;
}

double EntityMoveHelper::getZ() const
{
    return posZ;
}

float EntityMoveHelper::limitAngle(float sourceAngle, float targetAngle, float maximumChange) const
{
    float f = MathHelper::wrapDegrees(targetAngle - sourceAngle);
    if (f > maximumChange) 
    {
        f = maximumChange;
    }

    if (f < -maximumChange) 
    {
        f = -maximumChange;
    }

    float f1 = sourceAngle + f;
    if (f1 < 0.0F) 
    {
        f1 += 360.0F;
    }
    else if (f1 > 360.0F) 
    {
        f1 -= 360.0F;
    }

    return f1;
}
