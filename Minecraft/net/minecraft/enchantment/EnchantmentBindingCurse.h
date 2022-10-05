#pragma once
#include "Enchantment.h"

class EnchantmentBindingCurse : public Enchantment
{
  public:
    EnchantmentBindingCurse(Enchantment::Rarity p_i47254_1_, std::span<const EntityEquipmentSlot> p_i47254_2_);
    int32_t getMinEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxLevel() const override;
    bool isTreasureEnchantment() const override;
    bool isCurse() const override;
};
