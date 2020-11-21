#include "EnchantmentDigging.h"
#include "../item/ItemStack.h"

EnchantmentDigging::EnchantmentDigging(Enchantment::Rarity rarityIn, std::span<const EntityEquipmentSlot>slots):
    Enchantment(rarityIn, EnumEnchantmentType::DIGGER, slots) {
    setName("digging");
}

int32_t EnchantmentDigging::getMinEnchantability(int32_t enchantmentLevel) const {
    return 1 + 10 * (enchantmentLevel - 1);
}

int32_t EnchantmentDigging::getMaxEnchantability(int32_t enchantmentLevel) const {
    return Enchantment::getMinEnchantability(enchantmentLevel) + 50;
}

int32_t EnchantmentDigging::getMaxLevel() const {
    return 5;
}

bool EnchantmentDigging::canApply(const ItemStack &stack) const {
    return stack.getItem() == Items::SHEARS ? true : Enchantment::canApply(stack);
}
