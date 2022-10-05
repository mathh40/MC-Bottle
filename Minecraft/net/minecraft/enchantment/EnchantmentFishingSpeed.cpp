#include "EnchantmentFishingSpeed.h"

EnchantmentFishingSpeed::EnchantmentFishingSpeed(Enchantment::Rarity rarityIn, EnumEnchantmentType typeIn,
                                                 std::span<const EntityEquipmentSlot> slots)
    : Enchantment(rarityIn, typeIn, slots)
{
    setName("fishingSpeed");
}

int32_t EnchantmentFishingSpeed::getMinEnchantability(int32_t enchantmentLevel) const
{
    return 15 + (enchantmentLevel - 1) * 9;
}

int32_t EnchantmentFishingSpeed::getMaxEnchantability(int32_t enchantmentLevel) const
{
    return super.getMinEnchantability(enchantmentLevel) + 50;
}

int32_t EnchantmentFishingSpeed::getMaxLevel() const
{
    return 3;
}
