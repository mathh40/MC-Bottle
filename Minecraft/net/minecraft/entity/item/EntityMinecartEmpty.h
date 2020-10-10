#pragma once
#include "EntityMinecart.h"

class EntityMinecartEmpty :public EntityMinecart {
public:
    EntityMinecartEmpty(World* worldIn);
    EntityMinecartEmpty(World* worldIn, double x, double y, double z);
    static void registerFixesMinecartEmpty(DataFixer fixer);
    bool processInitialInteract(EntityPlayer* player, EnumHand hand) override;
    void onActivatorRailPass(int32_t x, int32_t y, int32_t z, bool receivingPower) override;
    EntityMinecart::Type getType() override;

private:
};