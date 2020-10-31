#pragma once
#include "EntitySenses.h"

class EntityMoveHelper
{
public:

    explicit EntityMoveHelper(EntityLiving* entitylivingIn);
    bool isUpdating() const;
    double getSpeed() const;
    void setMoveTo(double x, double y, double z, double speedIn);
    void strafe(float forward, float strafe);
    void read(const EntityMoveHelper& that);
    virtual void onUpdateMoveHelper();
    double getX() const;
    double getY() const;
    double getZ() const;

    enum class Action
    {
        WAIT,
        MOVE_TO,
        STRAFE,
        JUMPING
    };

    Action action;
protected:
    float limitAngle(float sourceAngle, float targetAngle, float maximumChange) const;

    EntityLiving* entity;
    double posX;
    double posY;
    double posZ;
    double speed;
    float moveForward;
    float moveStrafe;
private:
};
