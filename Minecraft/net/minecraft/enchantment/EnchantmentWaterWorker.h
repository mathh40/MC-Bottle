#pragma once
#include "Enchantment.h"

class EnchantmentWaterWorker : public Enchantment
{
  public:
    EnchantmentWaterWorker(Enchantment::Rarity rarityIn, std::span<EntityEquipmentSlot> slots);
    int32_t getMinEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxLevel() const override;
};
