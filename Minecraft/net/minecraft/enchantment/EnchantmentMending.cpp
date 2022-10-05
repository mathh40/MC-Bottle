#include "EnchantmentMending.h"

EnchantmentMending::EnchantmentMending(Enchantment::Rarity rarityIn, std::span<const EntityEquipmentSlot> slots)
    : Enchantment(rarityIn, EnumEnchantmentType::BREAKABLE, slots)
{
    setName("mending");
}

int32_t EnchantmentMending::getMinEnchantability(int32_t enchantmentLevel) const
{
    return enchantmentLevel * 25;
}

int32_t EnchantmentMending::getMaxEnchantability(int32_t enchantmentLevel) const
{
    return getMinEnchantability(enchantmentLevel) + 50;
}

bool EnchantmentMending::isTreasureEnchantment() const
{
    return true;
}

int32_t EnchantmentMending::getMaxLevel() const
{
    return 1;
}
