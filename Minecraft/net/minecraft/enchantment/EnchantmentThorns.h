#pragma once
#include "Enchantment.h"

class EnchantmentThorns :public Enchantment {
public:
    EnchantmentThorns(Enchantment::Rarity rarityIn, std::span<const EntityEquipmentSlot> slots);
    int32_t getMinEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxEnchantability(int32_t enchantmentLevel) const override;
    int32_t getMaxLevel() const override;
    bool canApply(const ItemStack &stack) const override;
    void onUserHurt(EntityLivingBase *user, Entity *attacker, int32_t level) const override;
    static bool shouldHit(int32_t level, Random &rnd);
    static int32_t getDamage(int32_t level, Random &rnd);
};
