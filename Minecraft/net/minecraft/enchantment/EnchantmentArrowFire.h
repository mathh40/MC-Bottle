#pragma once
#include "Enchantment.h"

class EnchantmentArrowFire : public Enchantment
{
  public:
    EnchantmentArrowFire(Enchantment::Rarity rarityIn, std::span<const EntityEquipmentSlot> slots);
    int32_t getMinEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxLevel() const override;
};
