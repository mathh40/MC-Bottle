class EntityLivingBase;

class IRangedAttackMob
{
    virtual void attackEntityWithRangedAttack(EntityLivingBase* var1, float var2) = 0;

    virtual void setSwingingArms(bool var1) = 0;
};
