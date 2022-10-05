#include "EnchantmentWaterWalker.h"

EnchantmentWaterWalker::EnchantmentWaterWalker(Enchantment::Rarity rarityIn, std::span<EntityEquipmentSlot> slots)
    : Enchantment(rarityIn, EnumEnchantmentType::ARMOR_FEET, slots)
{
    setName("waterWalker");
}

int32_t EnchantmentWaterWalker::getMinEnchantability(int32_t enchantmentLevel) const
{
    return enchantmentLevel * 10;
}

int32_t EnchantmentWaterWalker::getMaxEnchantability(int32_t enchantmentLevel) const
{
    return getMinEnchantability(enchantmentLevel) + 15;
}

int32_t EnchantmentWaterWalker::getMaxLevel() const
{
    return 3;
}

bool EnchantmentWaterWalker::canApplyTogether(const Enchantment &ench) const
{
    return Enchantment::canApplyTogether(ench) && ench != Enchantments::FROST_WALKER;
}
