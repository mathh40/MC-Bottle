#include "EnchantmentArrowDamage.h"

EnchantmentArrowDamage::EnchantmentArrowDamage(Enchantment::Rarity rarityIn,
                                               std::span<const EntityEquipmentSlot> slots):
    Enchantment(rarityIn, EnumEnchantmentType::BOW, slots) {
    setName("arrowDamage");
}

int32_t EnchantmentArrowDamage::getMinEnchantability(int32_t enchantmentLevel) const{
    return 1 + (enchantmentLevel - 1) * 10;
}

int32_t EnchantmentArrowDamage::getMaxEnchantability(int32_t enchantmentLevel) const {
    return getMinEnchantability(enchantmentLevel) + 15;
}

int32_t EnchantmentArrowDamage::getMaxLevel() const {
    return 5;
}
