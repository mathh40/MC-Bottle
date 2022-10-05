#include "EnchantmentLootBonus.h"

EnchantmentLootBonus::EnchantmentLootBonus(Enchantment::Rarity rarityIn, EnumEnchantmentType typeIn,
                                           std::span<const EntityEquipmentSlot> slots)
    : Enchantment(rarityIn, typeIn, slots)
{
    if (typeIn == EnumEnchantmentType::DIGGER)
    {
        setName("lootBonusDigger");
    }
    else if (typeIn == EnumEnchantmentType::FISHING_ROD)
    {
        setName("lootBonusFishing");
    }
    else
    {
        setName("lootBonus");
    }
}

int32_t EnchantmentLootBonus::getMinEnchantability(int32_t enchantmentLevel) const
{
    return 15 + (enchantmentLevel - 1) * 9;
}

int32_t EnchantmentLootBonus::getMaxEnchantability(int32_t enchantmentLevel) const
{
    return Enchantment::getMinEnchantability(enchantmentLevel) + 50;
}

int32_t EnchantmentLootBonus::getMaxLevel() const
{
    return 3;
}

bool EnchantmentLootBonus::canApplyTogether(const Enchantment &ench) const
{
    return Enchantment::canApplyTogether(ench) && ench != Enchantments::SILK_TOUCH;
}
