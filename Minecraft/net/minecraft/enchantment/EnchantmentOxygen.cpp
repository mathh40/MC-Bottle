#include "EnchantmentOxygen.h"

EnchantmentOxygen::EnchantmentOxygen(Enchantment::Rarity rarityIn, std::span<const EntityEquipmentSlot> slots)
    : Enchantment(rarityIn, EnumEnchantmentType::ARMOR_HEAD, slots)
{
    setName("oxygen");
}

int32_t EnchantmentOxygen::getMinEnchantability(int32_t enchantmentLevel) const
{
    return 10 * enchantmentLevel;
}

int32_t EnchantmentOxygen::getMaxEnchantability(int32_t enchantmentLevel) const
{
    return getMinEnchantability(enchantmentLevel) + 30;
}

int32_t EnchantmentOxygen::getMaxLevel() const
{
    return 3;
}
