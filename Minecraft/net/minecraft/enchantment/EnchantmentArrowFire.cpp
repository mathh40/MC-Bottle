#include "EnchantmentArrowFire.h"

EnchantmentArrowFire::EnchantmentArrowFire(Enchantment::Rarity rarityIn, std::span<const EntityEquipmentSlot> slots)
    : Enchantment(rarityIn, EnumEnchantmentType::BOW, slots)
{
    setName("arrowFire");
}

int32_t EnchantmentArrowFire::getMinEnchantability(int32_t enchantmentLevel) const
{
    return 20;
}

int32_t EnchantmentArrowFire::getMaxEnchantability(int32_t enchantmentLevel) const
{
    return 50;
}

int32_t EnchantmentArrowFire::getMaxLevel() const
{
    return 1;
}
