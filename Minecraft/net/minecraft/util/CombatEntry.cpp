#include "CombatEntry.h"
#include "DamageSource.h"

CombatEntry::CombatEntry(const DamageSource::DamageSource & damageSrcIn, const int timeIn, const float healthAmount, const float damageAmount, const std::string &fallSuffixIn, const float fallDistanceIn)
	:damageSrc(damageSrcIn), time(timeIn), damage(damageAmount), health(healthAmount), fallSuffix(fallSuffixIn), fallDistance(fallDistanceIn)
{
}

DamageSource CombatEntry::getDamageSrc()
{
	return damageSrc;
}

float CombatEntry::getDamage()
{
	return damage;
}

bool CombatEntry::isLivingDamageSrc()
{
	return instanceof<EntityLivingBase>(damageSrc.getTrueSource());
}

std::string CombatEntry::getFallSuffix()
{
	return fallSuffix;
}

ITextComponent CombatEntry::getDamageSrcDisplayName()
{
	if (getDamageSrc().getTrueSource())
	{
		return getDamageSrc().getTrueSource().getDisplayName();
	}
	else
	{
		return "";
	}
}

float CombatEntry::getDamageAmount()
{
	return damageSrc == DamageSource.OUT_OF_WORLD ? std::numeric_limits<float>::max : fallDistance;
}
