#include "EnchantmentDamage.h"

#include "../entity/EntityLivingBase.h"
#include "../item/ItemAxe.h"
#include "../item/ItemStack.h"

EnchantmentDamage::EnchantmentDamage(Enchantment::Rarity rarityIn, int32_t damageTypeIn,
                                     std::span<const EntityEquipmentSlot> slots)
    : Enchantment(rarityIn, EnumEnchantmentType::WEAPON, slots), damageType(damageTypeIn)
{
}

int32_t EnchantmentDamage::getMinEnchantability(int32_t enchantmentLevel) const
{
    return MIN_COST[damageType] + (enchantmentLevel - 1) * LEVEL_COST[damageType];
}

int32_t EnchantmentDamage::getMaxEnchantability(int32_t enchantmentLevel) const
{
    return getMinEnchantability(enchantmentLevel) + LEVEL_COST_SPAN[damageType];
}

int32_t EnchantmentDamage::getMaxLevel() const
{
    return 5;
}

float EnchantmentDamage::calcDamageByCreature(int32_t level, const EnumCreatureAttribute &creatureType) const
{
    if (damageType == 0)
    {
        return 1.0F + MathHelper::max(0, level - 1) * 0.5F;
    }
    else if (damageType == 1 && creatureType == EnumCreatureAttribute::UNDEAD)
    {
        return (float)level * 2.5F;
    }
    else
    {
        return damageType == 2 && creatureType == EnumCreatureAttribute::ARTHROPOD ? level * 2.5F : 0.0F;
    }
}

std::string EnchantmentDamage::getName() const
{
    return "enchantment.damage." + DAMAGE_NAMES[damageType];
}

bool EnchantmentDamage::canApplyTogether(const Enchantment &ench)
{
    return !(Util:: instanceof <EnchantmentDamage>(&ench));
}

bool EnchantmentDamage::canApply(const ItemStack &stack) const
{
    return Util:: instanceof <ItemAxe>(stack.getItem()) ? true : Enchantment::canApply(stack);
}

void EnchantmentDamage::onEntityDamaged(EntityLivingBase *user, Entity *target, int32_t level) const
{
    if (Util:: instanceof <EntityLivingBase>(target))
    {
        EntityLivingBase *entitylivingbase = (EntityLivingBase *)target;
        if (damageType == 2 && entitylivingbase->getCreatureAttribute() == EnumCreatureAttribute::ARTHROPOD)
        {
            auto i = 20 + user->getRNG()(10 * level);
            entitylivingbase->addPotionEffect(PotionEffect(MobEffects::SLOWNESS, i, 3));
        }
    }
}
