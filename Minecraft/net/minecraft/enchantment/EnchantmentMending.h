#pragma once
#include "Enchantment.h"

class EnchantmentMending :public Enchantment {
  public:
    EnchantmentMending(Enchantment::Rarity rarityIn, std::span<const EntityEquipmentSlot> slots);
public:
    int32_t getMinEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxEnchantability(int32_t enchantmentLevel) const override;
    bool isTreasureEnchantment() const override;
    int32_t getMaxLevel() const override;
};
