#include "CombatEntry.h"

#include "../entity/EntityLivingBase.h"
#include "DamageSource.h"
#include "Util.h"

CombatEntry::CombatEntry(const DamageSource::DamageSource &damageSrcIn, uint64_t timeIn, float healthAmount,
                         float damageAmount, const std::string &fallSuffixIn, float fallDistanceIn)
    : damageSrc(damageSrcIn), time(timeIn), damage(damageAmount), health(healthAmount), fallSuffix(fallSuffixIn),
      fallDistance(fallDistanceIn)
{
}

DamageSource::DamageSource CombatEntry::getDamageSrc()
{
    return damageSrc;
}

float CombatEntry::getDamage()
{
    return damage;
}

bool CombatEntry::isLivingDamageSrc()
{
    return Util:: instanceof <EntityLivingBase>(damageSrc.getTrueSource());
}

std::string CombatEntry::getFallSuffix()
{
    return fallSuffix;
}

ITextComponent *CombatEntry::getDamageSrcDisplayName()
{
    if (getDamageSrc().getTrueSource())
    {
        return getDamageSrc().getTrueSource()->getDisplayName();
    }
    else
    {
        return "";
    }
}

float CombatEntry::getDamageAmount()
{
    return damageSrc == DamageSource::OUT_OF_WORLD ? std::numeric_limits<float>::max : fallDistance;
}
