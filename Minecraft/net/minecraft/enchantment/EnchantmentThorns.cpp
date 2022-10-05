#include "EnchantmentThorns.h"

#include "../entity/EntityLivingBase.h"
#include "../item/ItemArmor.h"
#include "../item/ItemStack.h"
#include "EnchantmentHelper.h"
#include "Util.h"

EnchantmentThorns::EnchantmentThorns(Enchantment::Rarity rarityIn, std::span<const EntityEquipmentSlot> slots)
    : Enchantment(rarityIn, EnumEnchantmentType::ARMOR_CHEST, slots)
{
    setName("thorns");
}

int32_t EnchantmentThorns::getMinEnchantability(int32_t enchantmentLevel) const
{
    return 10 + 20 * (enchantmentLevel - 1);
}

int32_t EnchantmentThorns::getMaxEnchantability(int32_t enchantmentLevel) const
{
    return Enchantment::getMinEnchantability(enchantmentLevel) + 50;
}

int32_t EnchantmentThorns::getMaxLevel() const
{
    return 3;
}

bool EnchantmentThorns::canApply(const ItemStack &stack) const
{
    return Util:: instanceof <ItemArmor>(stack.getItem()) ? true : Enchantment::canApply(stack);
}

void EnchantmentThorns::onUserHurt(EntityLivingBase *user, Entity *attacker, int32_t level) const
{
    auto random         = user->getRNG();
    ItemStack itemstack = EnchantmentHelper::getEnchantedItem(Enchantments::THORNS, user);
    if (shouldHit(level, random))
    {
        if (attacker != nullptr)
        {
            attacker->attackEntityFrom(DamageSource::causeThornsDamage(user), (float)getDamage(level, random));
        }

        if (!itemstack.isEmpty())
        {
            itemstack.damageItem(3, user);
        }
    }
    else if (!itemstack.isEmpty())
    {
        itemstack.damageItem(1, user);
    }
}

bool EnchantmentThorns::shouldHit(int32_t level, pcg32 &rnd)
{
    if (level <= 0)
    {
        return false;
    }
    else
    {
        return MathHelper::nextFloat(rnd) < 0.15F * (float)level;
    }
}

int32_t EnchantmentThorns::getDamage(int32_t level, pcg32 &rnd)
{
    return level > 10 ? level - 10 : 1 + rnd(4);
}
