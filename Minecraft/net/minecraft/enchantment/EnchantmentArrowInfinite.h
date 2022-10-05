#pragma once
#include "Enchantment.h"
#include "Util.h"

class EnchantmentArrowInfinite : public Enchantment
{
  public:
    EnchantmentArrowInfinite(Enchantment::Rarity rarityIn, std::span<const EntityEquipmentSlot> slots);
    int32_t getMinEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxLevel() const override;
    bool canApplyTogether(const Enchantment &ench) override;
};
