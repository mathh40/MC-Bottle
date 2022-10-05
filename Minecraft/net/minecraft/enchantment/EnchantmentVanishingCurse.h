#pragma once
#include "Enchantment.h"

class EnchantmentVanishingCurse : public Enchantment
{
  public:
    EnchantmentVanishingCurse(Enchantment::Rarity p_i47252_1_, std::span<EntityEquipmentSlot> p_i47252_2_);
    int32_t getMinEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxLevel() const override;
    bool isTreasureEnchantment() const override;
    bool isCurse() const override;
};
