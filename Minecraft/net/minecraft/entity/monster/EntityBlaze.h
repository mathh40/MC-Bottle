#pragma once
#include "EntityMob.h"

class EntityBlaze : public EntityMob
{
  public:
    EntityBlaze(World *worldIn);
    static void registerFixesBlaze(DataFixer fixer);
    int32_t getBrightnessForRender() override;
    float getBrightness() override;
    void onLivingUpdate() override;
    bool isBurning() override;
    bool isCharged();
    void setOnFire(bool onFire);

  protected:
    void initEntityAI() override;
    void applyEntityAttributes() override;
    void entityInit() override;
    SoundEvent getAmbientSound() override;
    SoundEvent getHurtSound(DamageSource::DamageSource damageSourceIn) override;
    SoundEvent getDeathSound() override;
    void updateAITasks() override;
    void fall(float distance, float damageMultiplier) override;
    std::optional<ResourceLocation> getLootTable() override;
    bool isValidLightLevel() override;

  private:
    float heightOffset = 0.5F;
    int32_t heightOffsetUpdateTime;
    static DataParameter ON_FIRE;
};
