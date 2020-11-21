#pragma once
#include "Enchantment.h"

class EnchantmentLootBonus :public Enchantment {
public:
    EnchantmentLootBonus(Enchantment::Rarity rarityIn, EnumEnchantmentType typeIn,
                         std::span<const EntityEquipmentSlot> slots);
    int32_t getMinEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxLevel() const override;
    bool canApplyTogether(const Enchantment &ench) const override;
};
