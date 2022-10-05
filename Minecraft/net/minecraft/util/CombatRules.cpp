#include "CombatRules.h"

#include "math/MathHelper.h"

float CombatRules::getDamageAfterAbsorb(const float damage, const float totalArmor, const float toughnessAttribute)
{
    auto f  = 2.0F + toughnessAttribute / 4.0F;
    auto f1 = MathHelper::clamp(totalArmor - damage / f, totalArmor * 0.2F, 20.0F);
    return damage * (1.0F - f1 / 25.0F);
}

float CombatRules::getDamageAfterMagicAbsorb(const float damage, const float enchantModifiers)
{
    auto f = MathHelper::clamp(enchantModifiers, 0.0F, 20.0F);
    return damage * (1.0F - f / 25.0F);
}
