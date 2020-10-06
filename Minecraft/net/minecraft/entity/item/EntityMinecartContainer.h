#pragma once
#include "EntityMinecart.h"
#include "storage/loot/ILootContainer.h"

class EntityMinecartContainer :public EntityMinecart, public ILockableContainer, public ILootContainer {
public:
    explicit EntityMinecartContainer(World* worldIn);
    EntityMinecartContainer(World* worldIn, double x, double y, double z);
    void killMinecart(DamageSource::DamageSource source) override;
    bool isEmpty();

private:
    std::vector<ItemStack> minecartContainerItems;
    bool dropContentsWhenDead;
    ResourceLocation lootTable;
    int64_t lootTableSeed;
};
