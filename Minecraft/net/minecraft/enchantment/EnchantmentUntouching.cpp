#include "EnchantmentUntouching.h"

EnchantmentUntouching::EnchantmentUntouching(Enchantment::Rarity rarityIn, std::span<EntityEquipmentSlot> slots)
    : Enchantment(rarityIn, EnumEnchantmentType::DIGGER, slots)
{
    setName("untouching");
}

int32_t EnchantmentUntouching::getMinEnchantability(int32_t enchantmentLevel) const
{
    return 15;
}

int32_t EnchantmentUntouching::getMaxEnchantability(int32_t enchantmentLevel) const
{
    return Enchantment::getMinEnchantability(enchantmentLevel) + 50;
}

int32_t EnchantmentUntouching::getMaxLevel() const
{
    return 1;
}

bool EnchantmentUntouching::canApplyTogether(const Enchantment &ench) const
{
    return Enchantment::canApplyTogether(ench) && ench != Enchantments::FORTUNE;
}
