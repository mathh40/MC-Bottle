#pragma once
#include "EntityMob.h"

class EntityGiantZombie :public EntityMob {
public:
    EntityGiantZombie(World *worldIn);
    static void registerFixesGiantZombie(DataFixer fixer);
    float getEyeHeight() const override;
    float getBlockPathWeight(BlockPos pos) override;

protected:
    void applyEntityAttributes() override;
    std::optional<ResourceLocation> getLootTable() override;
};
