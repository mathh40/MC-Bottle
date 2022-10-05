#include "EnchantmentVanishingCurse.h"

EnchantmentVanishingCurse::EnchantmentVanishingCurse(Enchantment::Rarity p_i47252_1_,
                                                     std::span<EntityEquipmentSlot> p_i47252_2_)
    : Enchantment(p_i47252_1_, EnumEnchantmentType::ALL, p_i47252_2_)
{
    setName("vanishing_curse");
}

int32_t EnchantmentVanishingCurse::getMinEnchantability(int32_t enchantmentLevel) const
{
    return 25;
}

int32_t EnchantmentVanishingCurse::getMaxEnchantability(int32_t enchantmentLevel) const
{
    return 50;
}

int32_t EnchantmentVanishingCurse::getMaxLevel() const
{
    return 1;
}

bool EnchantmentVanishingCurse::isTreasureEnchantment() const
{
    return true;
}

bool EnchantmentVanishingCurse::isCurse() const
{
    return true;
}
