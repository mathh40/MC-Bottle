#pragma once
#include "EntityMob.h"
#include "../IRangedAttackMob.h"
#include "../ai/EntityAIAttackMelee.h"
#include "../ai/EntityAIAttackRangedBow.h"

class AbstractSkeleton;

class EntityAIAttackOnCollide : public EntityAIAttackMelee {
public:
    EntityAIAttackOnCollide(AbstractSkeleton *entitySkeletonIn, double speedIn, bool useLongMemoryIn);

    void resetTask() override;

    void startExecuting() override;
private:
};

class AbstractSkeleton :public EntityMob, public IRangedAttackMob {
public:
    AbstractSkeleton(World* worldIn);
    virtual SoundEvent getStepSound() = 0;
    EnumCreatureAttribute getCreatureAttribute() override;
    void onLivingUpdate() override;
    void updateRidden() override;
    IEntityLivingData* onInitialSpawn(DifficultyInstance difficulty, IEntityLivingData* livingdata) override;
    void setCombatTask();
    void attackEntityWithRangedAttack(EntityLivingBase* target, float distanceFactor) override;
    void readEntityFromNBT(NBTTagCompound* compound) override;
    void setItemStackToSlot(EntityEquipmentSlot slotIn, ItemStack stack) override;
    float getEyeHeight() const override;
    double getYOffset() override;
    bool isSwingingArms();
    void setSwingingArms(bool swingingArms) override;


protected:
    void initEntityAI() override;
    void applyEntityAttributes() override;
    void entityInit() override;
    void playStepSound(const BlockPos& pos, Block* blockIn);
    void setEquipmentBasedOnDifficulty(DifficultyInstance difficulty) override;
    EntityArrow* getArrow(float p_190726_1_);

private:
    static DataParameter SWINGING_ARMS;
    EntityAIAttackRangedBow* aiArrowAttack = new EntityAIAttackRangedBow(this, 1.0, 20, 15.0F);
    EntityAIAttackOnCollide* aiAttackOnCollide = new EntityEntityAIAttackOnCollideAIAttackMelee(this, 1.2, false);
};
