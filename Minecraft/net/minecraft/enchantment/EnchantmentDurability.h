#pragma once
#include "Enchantment.h"
#include "pcg_random.hpp"
#include "Util.h"

class EnchantmentDurability :public Enchantment {
public:
    EnchantmentDurability(Enchantment::Rarity rarityIn, std::span<const EntityEquipmentSlot>slots);
    int32_t getMinEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxLevel() const override;
    bool canApply(const ItemStack &stack) const override;
    static bool negateDamage(const ItemStack &stack, int32_t level, pcg32 &rand);
};