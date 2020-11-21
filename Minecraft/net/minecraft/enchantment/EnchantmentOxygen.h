#pragma once
#include "Enchantment.h"

class EnchantmentOxygen :public Enchantment {
  public:
    EnchantmentOxygen(Enchantment::Rarity rarityIn, std::span<const EntityEquipmentSlot> slots);
    int32_t getMinEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxLevel() const override;
};
