#pragma once
#include "EntityMinecartContainer.h"

class EntityMinecartChest :public EntityMinecartContainer {
public:
    EntityMinecartChest(World* worldIn);
    EntityMinecartChest(World* worldIn, double x, double y, double z);
    static void registerFixesMinecartChest(DataFixer fixer);
    void killMinecart(DamageSource::DamageSource source) override;
    int32_t getSizeInventory() const override;
    EntityMinecart::Type getType() override;
    IBlockState* getDefaultDisplayTile() override;
    int32_t getDefaultDisplayTileOffset() const override;
    std::string getGuiID() const override;
    Container* createContainer(InventoryPlayer* playerInventory, EntityPlayer* playerIn);
private:
};