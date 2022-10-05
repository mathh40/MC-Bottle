#pragma once
#include "Enchantment.h"

class EnchantmentSweepingEdge : public Enchantment
{
  public:
    EnchantmentSweepingEdge(Enchantment::Rarity p_i47366_1_, std::span<const EntityEquipmentSlot> p_i47366_2_);
    int32_t getMinEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxLevel() const override;
    static float getSweepingDamageRatio(int32_t p_191526_0_);
    std::string getName() const override;
};
