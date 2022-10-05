#pragma once
#include "EntityMob.h"

class EntityEndermite : public EntityMob
{
  public:
    explicit EntityEndermite(World *worldIn);
    float getEyeHeight() const override;
    void readEntityFromNBT(NBTTagCompound *compound) override;
    void writeEntityToNBT(NBTTagCompound *compound) override;
    double getYOffset() override;
    bool isSpawnedByPlayer() const;
    void setSpawnedByPlayer(bool spawnedByPlayer);
    void onLivingUpdate() override;
    bool getCanSpawnHere() override;
    EnumCreatureAttribute getCreatureAttribute() override;

  protected:
    void initEntityAI() override;
    void applyEntityAttributes() override;
    bool canTriggerWalking() override;
    SoundEvent getAmbientSound() override;
    SoundEvent getHurtSound(DamageSource::DamageSource damageSourceIn) override;
    SoundEvent getDeathSound() override;
    void playStepSound(BlockPos pos, Block *blockIn) override;
    std::optional<ResourceLocation> getLootTable() override;
    static void registerFixesEndermite(DataFixer fixer);
    void onUpdate() override;
    void setRenderYawOffset(float offset) override;
    bool isValidLightLevel() override;

  private:
    int32_t lifetime;
    bool playerSpawned;
};
