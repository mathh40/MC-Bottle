#include "EnchantmentWaterWorker.h"

EnchantmentWaterWorker::EnchantmentWaterWorker(Enchantment::Rarity rarityIn, std::span<EntityEquipmentSlot> slots)
    : Enchantment(rarityIn, EnumEnchantmentType::ARMOR_HEAD, slots)
{
    setName("waterWorker");
}

int32_t EnchantmentWaterWorker::getMinEnchantability(int32_t enchantmentLevel) const
{
    return 1;
}

int32_t EnchantmentWaterWorker::getMaxEnchantability(int32_t enchantmentLevel) const
{
    return getMinEnchantability(enchantmentLevel) + 40;
}

int32_t EnchantmentWaterWorker::getMaxLevel() const
{
    return 1;
}
