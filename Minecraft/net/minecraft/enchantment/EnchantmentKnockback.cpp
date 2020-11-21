#include "EnchantmentKnockback.h"

EnchantmentKnockback::EnchantmentKnockback(Enchantment::Rarity rarityIn,
                                           std::span<const EntityEquipmentSlot> slots):
    Enchantment(rarityIn, EnumEnchantmentType::WEAPON, slots) {
    setName("knockback");
}

int32_t EnchantmentKnockback::getMinEnchantability(int32_t enchantmentLevel) const {
    return 5 + 20 * (enchantmentLevel - 1);
}

int32_t EnchantmentKnockback::getMaxEnchantability(int32_t enchantmentLevel) const {
    return Enchantment::getMinEnchantability(enchantmentLevel) + 50;
}

int32_t EnchantmentKnockback::getMaxLevel() const {
    return 2;
}
