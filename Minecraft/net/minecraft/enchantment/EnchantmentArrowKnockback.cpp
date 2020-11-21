#include "EnchantmentArrowKnockback.h"

EnchantmentArrowKnockback::EnchantmentArrowKnockback(Enchantment::Rarity rarityIn,
                                                     std::span<const EntityEquipmentSlot>slots):
    Enchantment(rarityIn, EnumEnchantmentType::BOW, slots) {
    setName("arrowKnockback");
}

int32_t EnchantmentArrowKnockback::getMinEnchantability(int32_t enchantmentLevel) const {
    return 12 + (enchantmentLevel - 1) * 20;
}

int32_t EnchantmentArrowKnockback::getMaxEnchantability(int32_t enchantmentLevel) const {
    return getMinEnchantability(enchantmentLevel) + 25;
}

int32_t EnchantmentArrowKnockback::getMaxLevel() const {
    return 2;
}
