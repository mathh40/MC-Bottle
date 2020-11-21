#include "EnchantmentBindingCurse.h"

EnchantmentBindingCurse::EnchantmentBindingCurse(Enchantment::Rarity p_i47254_1_,
                                                 std::span<const EntityEquipmentSlot>p_i47254_2_):
    Enchantment(p_i47254_1_, EnumEnchantmentType::WEARABLE, p_i47254_2_) {
    setName("binding_curse");
}

int32_t EnchantmentBindingCurse::getMinEnchantability(int32_t enchantmentLevel) const {
    return 25;
}

int32_t EnchantmentBindingCurse::getMaxEnchantability(int32_t enchantmentLevel) const {
    return 50;
}

int32_t EnchantmentBindingCurse::getMaxLevel() const {
    return 1;
}

bool EnchantmentBindingCurse::isTreasureEnchantment() const {
    return true;
}

bool EnchantmentBindingCurse::isCurse() const {
    return true;
}
