#include "EnchantmentSweepingEdge.h"

EnchantmentSweepingEdge::EnchantmentSweepingEdge(Enchantment::Rarity p_i47366_1_,
                                                 std::span<const EntityEquipmentSlot> p_i47366_2_):
    Enchantment(p_i47366_1_, EnumEnchantmentType::WEAPON, p_i47366_2_) {

}

int32_t EnchantmentSweepingEdge::getMinEnchantability(int32_t enchantmentLevel) const {
    return 5 + (enchantmentLevel - 1) * 9;
}

int32_t EnchantmentSweepingEdge::getMaxEnchantability(int32_t enchantmentLevel) const {
    return getMinEnchantability(enchantmentLevel) + 15;
}

int32_t EnchantmentSweepingEdge::getMaxLevel() const {
    return 3;
}

float EnchantmentSweepingEdge::getSweepingDamageRatio(int32_t p_191526_0_) {
    return 1.0F - 1.0F / (float)(p_191526_0_ + 1);
}

std::string EnchantmentSweepingEdge::getName() const {
    return "enchantment.sweeping";
}
