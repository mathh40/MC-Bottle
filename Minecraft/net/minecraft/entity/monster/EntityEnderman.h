#pragma once
#include "../ai/EntityAIBase.h"
#include "../ai/EntityAINearestAttackableTarget.h"
#include "EntityMob.h"

class EntityEnderman : public EntityMob
{
  public:
    EntityEnderman(World *worldIn);
    void setAttackTarget(EntityLivingBase *entitylivingbaseIn) override;
    void playEndermanSound();
    void notifyDataManagerChange(DataParameter key) override;
    static void registerFixesEnderman(DataFixer fixer);
    void writeEntityToNBT(NBTTagCompound *compound) override;
    void readEntityFromNBT(NBTTagCompound *compound) override;
    float getEyeHeight();
    void onLivingUpdate() override;
    void setHeldBlockState(IBlockState *state);
    IBlockState *getHeldBlockState();
    bool attackEntityFrom(DamageSource::DamageSource source, float amount) override;
    bool isScreaming();

    class AITakeBlock : public EntityAIBase
    {
      private:
        EntityEnderman *enderman;

      public:
        AITakeBlock(EntityEnderman *p_i45841_1_);

        bool shouldExecute() override;

        void updateTask() override;
    };

    class AIPlaceBlock : public EntityAIBase
    {
      private:
        EntityEnderman *enderman;

      public:
        AIPlaceBlock(EntityEnderman *p_i45843_1_);

        bool shouldExecute() override;

        void updateTask() override;

      private:
        bool canPlaceBlock(World *p_188518_1_, BlockPos p_188518_2_, Block *p_188518_3_, IBlockState *p_188518_4_,
                           IBlockState *p_188518_5_);
    };

    class AIFindPlayer : public EntityAINearestAttackableTarget
    {
      private:
        EntityEnderman *enderman;
        EntityPlayer *player;
        int32_t aggroTime;
        int32_t teleportTime;

      public:
        AIFindPlayer(EntityEnderman *p_i45842_1_);

        bool shouldExecute() override;

        void startExecuting() override;

        void resetTask() override;

        bool shouldContinueExecuting() override;

        void updateTask() override;
    };

  protected:
    void initEntityAI() override;
    void applyEntityAttributes() override;
    void entityInit() override;
    void updateAITasks() override;
    bool teleportRandomly();
    bool teleportToEntity(Entity *p_70816_1_);
    SoundEvent getAmbientSound() override;
    SoundEvent getHurtSound(DamageSource::DamageSource damageSourceIn) override;
    SoundEvent getDeathSound() override;
    void dropEquipment(bool wasRecentlyHit, int32_t lootingModifier) override;
    std::optional<ResourceLocation> getLootTable() override;

  private:
    bool shouldAttackPlayer(EntityPlayer *player);
    bool teleportTo(double x, double y, double z);

    static inline xg::Guid ATTACKING_SPEED_BOOST_ID = xg::Guid("020E0DFB-87AE-4653-9556-831010E291A0");
    static AttributeModifier ATTACKING_SPEED_BOOST;
    static inline std::unordered_set<Block *> CARRIABLE_BLOCKS = {
        Blocks::GRASS,      Blocks::DIRT,           Blocks::SAND,         Blocks::GRAVEL,   Blocks::YELLOW_FLOWER,
        Blocks::RED_FLOWER, Blocks::BROWN_MUSHROOM, Blocks::RED_MUSHROOM, Blocks::TNT,      Blocks::CACTUS,
        Blocks::CLAY,       Blocks::PUMPKIN,        Blocks::MELON_BLOCK,  Blocks::MYCELIUM, Blocks::NETHERRACK};
    static DataParameter CARRIED_BLOCK;
    static DataParameter SCREAMING;
    int32_t lastCreepySound;
    int32_t targetChangeTime;
};
