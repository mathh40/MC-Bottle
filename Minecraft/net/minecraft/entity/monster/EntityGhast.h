#pragma once
#include "../EntityFlying.h"
#include "IMob.h"
#include "ai/EntityAIBase.h"

class EntityGhast : public EntityFlying, public IMob
{
  public:
    class GhastMoveHelper : public EntityMoveHelper
    {

      public:
        explicit GhastMoveHelper(EntityGhast *ghast);
        void onUpdateMoveHelper() override;

      private:
        bool isNotColliding(double x, double y, double z, double p_179926_7_) const;
        EntityGhast *parentEntity;
        int32_t courseChangeCooldown;
    };

    class AIRandomFly : public EntityAIBase
    {
      private:
        EntityGhast *parentEntity;

      public:
        AIRandomFly(EntityGhast *ghast);
        bool shouldExecute() override;
        bool shouldContinueExecuting() override;
        void startExecuting() override;
    };

    class AILookAround : public EntityAIBase
    {
      private:
        EntityGhast *parentEntity;

      public:
        AILookAround(EntityGhast *ghast);
        bool shouldExecute() override;
        void updateTask() override;
    };

    class AIFireballAttack : public EntityAIBase
    {
      private:
        EntityGhast *parentEntity;

      public:
        int32_t attackTimer;

        AIFireballAttack(EntityGhast *ghast);
        bool shouldExecute() override;
        void startExecuting() override;
        void resetTask() override;
        void updateTask() override;
    };

    EntityGhast(World *worldIn);
    bool isAttacking();
    void setAttacking(bool attacking);
    int32_t getFireballStrength() const;
    void onUpdate() override;
    bool attackEntityFrom(DamageSource::DamageSource source, float amount) override;
    SoundCategory getSoundCategory() override;
    bool getCanSpawnHere() override;
    int32_t getMaxSpawnedInChunk() override;
    static void registerFixesGhast(DataFixer fixer);
    void writeEntityToNBT(NBTTagCompound *compound) override;
    void readEntityFromNBT(NBTTagCompound *compound) override;
    float getEyeHeight() const override;

  protected:
    void initEntityAI() override;
    void entityInit() override;
    void applyEntityAttributes() override;
    SoundEvent getAmbientSound() override;
    SoundEvent getHurtSound(DamageSource::DamageSource damageSourceIn) override;
    SoundEvent getDeathSound() override;
    std::optional<ResourceLocation> getLootTable() override;
    float getSoundVolume() override;

  private:
    static DataParameter ATTACKING;
    int32_t explosionStrength = 1;
};
