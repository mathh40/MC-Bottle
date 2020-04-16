#include "ToolMaterial.h"

ToolMaterial ToolMaterial::WOOD(0, 59, 2.0F, 0.0F, 15);
ToolMaterial ToolMaterial::STONE(1, 131, 4.0F, 1.0F, 5);
ToolMaterial ToolMaterial::IRON(2, 250, 6.0F, 2.0F, 14);
ToolMaterial ToolMaterial::DIAMOND(3, 1561, 8.0F, 3.0F, 10);
ToolMaterial ToolMaterial::GOLD(0, 32, 12.0F, 0.0F, 22);


ToolMaterial::ToolMaterial(int32_t harvestLevel, int32_t maxUses, float efficiency, float damageVsEntity,
    int32_t enchantability)
        :harvestLevel(harvestLevel),maxUses(maxUses),efficiency(efficiency)
        ,attackDamage(damageVsEntity),enchantability(enchantability)
{
}

int32_t ToolMaterial::getMaxUses() const
{
    return maxUses;
}

float ToolMaterial::getEfficiency() const
{
    return efficiency;
}

float ToolMaterial::getAttackDamage() const
{
    return attackDamage;
}

int32_t ToolMaterial::getHarvestLevel() const
{
    return harvestLevel;
}

int32_t ToolMaterial::getEnchantability() const
{
    return enchantability;
}

Item * ToolMaterial::getRepairItem()
{
    if (this == WOOD) 
    {
        return Item::getItemFromBlock(Blocks::PLANKS);
    }
    else if (this == STONE) 
    {
        return Item::getItemFromBlock(Blocks::COBBLESTONE);
    }
    else if (this == GOLD) 
    {
        return Items::GOLD_INGOT;
    }
    else if (this == IRON) 
    {
        return Items::IRON_INGOT;
    }
    else 
    {
        return this == DIAMOND ? Items::DIAMOND : nullptr;
    }
}

std::string ToolMaterial::toString() const
{
    return typeid(*this).name();
}
