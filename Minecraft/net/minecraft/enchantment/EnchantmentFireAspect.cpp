#include "EnchantmentFireAspect.h"

EnchantmentFireAspect::EnchantmentFireAspect(Enchantment::Rarity rarityIn, std::span<const EntityEquipmentSlot>slots):
    Enchantment(rarityIn, EnumEnchantmentType::WEAPON, slots) {
    setName("fire");
}

int32_t EnchantmentFireAspect::getMinEnchantability(int32_t enchantmentLevel) const {
    return 10 + 20 * (enchantmentLevel - 1);
}

int32_t EnchantmentFireAspect::getMaxEnchantability(int32_t enchantmentLevel) const {
    return Enchantment::getMinEnchantability(enchantmentLevel) + 50;
}

int32_t EnchantmentFireAspect::getMaxLevel() const {
    return 2;
}
