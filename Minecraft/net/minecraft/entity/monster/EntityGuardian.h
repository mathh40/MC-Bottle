#pragma once
#include "../ai/EntityAIWander.h"
#include "EntityMob.h"

class EntityGuardian : public EntityMob
{
  public:
    EntityGuardian(World *worldIn);
    static void registerFixesGuardian(DataFixer fixer);
    bool isMoving();
    int32_t getAttackDuration() const;
    bool hasTargetedEntity();
    EntityLivingBase *getTargetedEntity();
    void notifyDataManagerChange(DataParameter key) override;
    int32_t getTalkInterval() override;
    float getEyeHeight() const override;
    float getBlockPathWeight(BlockPos pos) override;
    void onLivingUpdate() override;
    float getTailAnimation(float p_175471_1_) const;
    float getSpikesAnimation(float p_175469_1_) const;
    float getAttackAnimationScale(float p_175477_1_) const;
    bool isNotColliding() override;
    bool getCanSpawnHere() override;
    bool attackEntityFrom(DamageSource::DamageSource source, float amount) override;
    int32_t getVerticalFaceSpeed() override;
    void travel(float strafe, float vertical, float forward) override;

    class GuardianMoveHelper : public EntityMoveHelper
    {
        EntityGuardian *entityGuardian;

      public:
        GuardianMoveHelper(EntityGuardian *guardian);

        void onUpdateMoveHelper() override;
    };

    class AIGuardianAttack : public EntityAIBase
    {
      private:
        EntityGuardian *guardian;
        int32_t tickCounter;
        bool isElder;

      public:
        AIGuardianAttack(EntityGuardian *guardian);

        bool shouldExecute() override;

        bool shouldContinueExecuting() override;

        void startExecuting() override;

        void resetTask() override;

        void updateTask() override;
    };

  protected:
    void initEntityAI() override;
    void applyEntityAttributes() override;
    PathNavigate *createNavigator(World *worldIn) override;
    void entityInit() override;
    SoundEvent getAmbientSound() override;
    SoundEvent getHurtSound(DamageSource::DamageSource damageSourceIn) override;
    SoundEvent getDeathSound() override;
    bool canTriggerWalking() override;
    virtual SoundEvent getFlopSound();
    std::optional<ResourceLocation> getLootTable() override;
    bool isValidLightLevel() override;

    float clientSideTailAnimation      = 0.0f;
    float clientSideTailAnimationO     = 0.0f;
    float clientSideTailAnimationSpeed = 0.0f;
    float clientSideSpikesAnimation    = 0.0f;
    float clientSideSpikesAnimationO   = 0.0f;
    EntityAIWander *wander;

  private:
    void setMoving(bool moving);
    void setTargetedEntity(int32_t entityId);

    static DataParameter MOVING;
    static DataParameter TARGET_ENTITY;
    EntityLivingBase *targetedEntity;
    int32_t clientSideAttackTime;
    bool clientSideTouchedGround;
};
