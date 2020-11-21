#include "EnchantmentArrowInfinite.h"

EnchantmentArrowInfinite::EnchantmentArrowInfinite(Enchantment::Rarity rarityIn,
                                                   std::span<const EntityEquipmentSlot>slots):
    Enchantment(rarityIn, EnumEnchantmentType::BOW, slots) {
    setName("arrowInfinite");
}

int32_t EnchantmentArrowInfinite::getMinEnchantability(int32_t enchantmentLevel) const{
    return 20;
}

int32_t EnchantmentArrowInfinite::getMaxEnchantability(int32_t enchantmentLevel) const{
    return 50;
}

int32_t EnchantmentArrowInfinite::getMaxLevel() const{
    return 1;
}

bool EnchantmentArrowInfinite::canApplyTogether(const Enchantment &ench) {
    return Util::instanceof<EnchantmentMending>(ench) ? false : Enchantment::canApplyTogether(ench);
}
