#pragma once
#include <cstdint>

#include "Item.h"

class ToolMaterial
{
public:
    static ToolMaterial WOOD;
    static ToolMaterial STONE;
    static ToolMaterial IRON;
    static ToolMaterial DIAMOND;
    static ToolMaterial GOLD;

    ToolMaterial(int32_t harvestLevel, int32_t maxUses, float efficiency, float damageVsEntity, int32_t enchantability);
    int32_t getMaxUses() const;
    float getEfficiency() const;
    float getAttackDamage() const;
    int32_t getHarvestLevel() const;
    int32_t getEnchantability() const;
    Item* getRepairItem();
private:
    int32_t harvestLevel;
    int32_t maxUses;
    float efficiency;
    float attackDamage;
    int32_t enchantability;
};
