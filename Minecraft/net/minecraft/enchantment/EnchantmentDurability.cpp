#include "EnchantmentDurability.h"

#include "../item/ItemArmor.h"
#include "../item/ItemStack.h"

EnchantmentDurability::EnchantmentDurability(Enchantment::Rarity rarityIn, std::span<const EntityEquipmentSlot> slots)
    : Enchantment(rarityIn, EnumEnchantmentType::BREAKABLE, slots)
{
    setName("durability");
}

int32_t EnchantmentDurability::getMinEnchantability(int32_t enchantmentLevel) const
{
    return 5 + (enchantmentLevel - 1) * 8;
}

int32_t EnchantmentDurability::getMaxEnchantability(int32_t enchantmentLevel) const
{
    return Enchantment::getMinEnchantability(enchantmentLevel) + 50;
}

int32_t EnchantmentDurability::getMaxLevel() const
{
    return 3;
}

bool EnchantmentDurability::canApply(const ItemStack &stack) const
{
    return stack.isItemStackDamageable() ? true : Enchantment::canApply(stack);
}

bool EnchantmentDurability::negateDamage(const ItemStack &stack, int32_t level, pcg32 &rand)
{
    if (Util:: instanceof <ItemArmor>(stack.getItem()) && MathHelper::nextFloat(rand) < 0.6F)
    {
        return false;
    }
    else
    {
        return rand(level + 1) > 0;
    }
}
