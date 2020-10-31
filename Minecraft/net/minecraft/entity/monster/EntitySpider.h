#pragma once
#include "EntityMob.h"
#include "../IEntityLivingData.h"

class EntitySpider :public EntityMob {
public:
    EntitySpider(World* worldIn);
    static void registerFixesSpider(DataFixer fixer);
    double getMountedYOffset() const override;
    void onUpdate() override;
    bool isOnLadder() override;
    void setInWeb() override;
    EnumCreatureAttribute getCreatureAttribute() override;
    bool isPotionApplicable(PotionEffect potioneffectIn) override;
    bool isBesideClimbableBlock();
    void setBesideClimbableBlock(bool climbing);
    IEntityLivingData* onInitialSpawn(DifficultyInstance difficulty, IEntityLivingData* livingdata) override;
    float getEyeHeight() const override;

    class GroupData :public IEntityLivingData {
    public:
        Potion effect;

        void setRandomEffect(pcg32 &rand);
    };

protected:
    void initEntityAI() override;
    PathNavigate* createNavigator(World* worldIn) override;
    void entityInit() override;
    void applyEntityAttributes() override;
    SoundEvent getAmbientSound() override;
    SoundEvent getHurtSound(DamageSource::DamageSource damageSourceIn) override;
    SoundEvent getDeathSound() override;
    void playStepSound(BlockPos pos, Block* blockIn) override;
    std::optional<ResourceLocation> getLootTable() override;


private:
    static DataParameter CLIMBING;
};
