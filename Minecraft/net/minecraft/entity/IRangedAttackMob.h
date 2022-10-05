#pragma once
class EntityLivingBase;

class IRangedAttackMob
{
  public:
    virtual ~IRangedAttackMob() = default;

    virtual void attackEntityWithRangedAttack(EntityLivingBase *var1, float var2) = 0;

    virtual void setSwingingArms(bool var1) = 0;
};
