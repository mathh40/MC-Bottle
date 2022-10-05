#pragma once
#include "EntitySpellcasterIllager.h"

class EntitySheep;

class EntityEvoker : public EntitySpellcasterIllager
{
  public:
    explicit EntityEvoker(World *worldIn);
    static void registerFixesEvoker(DataFixer fixer);
    void readEntityFromNBT(NBTTagCompound *compound) override;
    void writeEntityToNBT(NBTTagCompound *compound) override;
    void onUpdate() override;
    bool isOnSameTeam(Entity *entityIn) override;

    class AIWololoSpell : public EntitySpellcasterIllager::AIUseSpell
    {
      public:
        AIWololoSpell(EntityEvoker *entityEvokerIn);

        bool shouldExecute() override;

        bool shouldContinueExecuting() override;

        void resetTask() override;

      protected:
        void castSpell() override;

        int32_t getCastWarmupTime() override;

        int32_t getCastingTime() override;

        int32_t getCastingInterval() override;

        std::optional<SoundEvent> getSpellPrepareSound() override;

        EntitySpellcasterIllager::SpellType getSpellType() override;

      private:
        EntityEvoker *entityEvoker;
    };

    class AISummonSpell : public EntitySpellcasterIllager::AIUseSpell
    {
      public:
        AISummonSpell(EntityEvoker *entityEvokerIn);

        bool shouldExecute() override;

      protected:
        int32_t getCastingTime() override;

        int32_t getCastingInterval() override;

        void castSpell() override;

        std::optional<SoundEvent> getSpellPrepareSound() override;

        EntitySpellcasterIllager::SpellType getSpellType() override;

      private:
        EntityEvoker *entityEvoker;
    };

    class AICastingSpell : public EntitySpellcasterIllager::AICastingApell
    {
      public:
        AICastingSpell(EntityEvoker *entityEvokerIn);

        void updateTask() override;

      private:
        EntityEvoker *entityEvoker;
    };

    class AIAttackSpell : public EntitySpellcasterIllager::AIUseSpell
    {
      public:
        AIAttackSpell(EntityEvoker *entityEvokerIn) : AIUseSpell(entityEvokerIn), entityEvoker(entityEvokerIn)
        {
        }

      protected:
        int32_t getCastingTime() override;

        int32_t getCastingInterval() override;

        void castSpell() override;

        std::optional<SoundEvent> getSpellPrepareSound() override;

        EntitySpellcasterIllager::SpellType getSpellType() override;

      private:
        void spawnFangs(double p_190876_1_, double p_190876_3_, double p_190876_5_, double p_190876_7_,
                        float p_190876_9_, int p_190876_10_);

        EntityEvoker *entityEvoker;
    };

  protected:
    void initEntityAI() override;
    void applyEntityAttributes() override;
    void entityInit() override;
    std::optional<ResourceLocation> getLootTable();
    void updateAITasks() override;
    SoundEvent getAmbientSound() override;
    SoundEvent getDeathSound() override;
    SoundEvent getHurtSound(DamageSource::DamageSource damageSourceIn) override;
    SoundEvent getSpellSound() override;

  private:
    void setWololoTarget(EntitySheep *wololoTargetIn);
    EntitySheep *getWololoTarget() const;

    EntitySheep *wololoTarget;
};
