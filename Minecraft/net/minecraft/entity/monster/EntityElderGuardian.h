#pragma once
#include "EntityGuardian.h"

class EntityElderGuardian :public EntityGuardian {
public:
    EntityElderGuardian(World* worldIn);
    static void registerFixesElderGuardian(DataFixer fixer);
    int32_t getAttackDuration();
    void setGhost();

protected:
    void applyEntityAttributes() override;
    std::optional<ResourceLocation> getLootTable() override;
    SoundEvent getAmbientSound() override;
    SoundEvent getHurtSound(DamageSource::DamageSource damageSourceIn) override;
    SoundEvent getDeathSound() override;
    SoundEvent getFlopSound() override;
    void updateAITasks() override;

private:
};