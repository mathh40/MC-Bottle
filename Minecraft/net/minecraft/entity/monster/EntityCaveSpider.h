#pragma once
#include "EntitySpider.h"

class EntityCaveSpider :public EntitySpider {
public:
    EntityCaveSpider(World* worldIn);
    static void registerFixesCaveSpider(DataFixer fixer);
    bool attackEntityAsMob(Entity* entityIn) override;
    IEntityLivingData* onInitialSpawn(DifficultyInstance difficulty, IEntityLivingData* livingdata) override;
    float getEyeHeight() const override;


protected:
    void applyEntityAttributes() override;
    std::optional<ResourceLocation> getLootTable() override;
private:
};