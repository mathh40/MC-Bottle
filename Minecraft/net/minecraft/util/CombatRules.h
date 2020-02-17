#pragma once

namespace CombatRules
{
	float getDamageAfterAbsorb(const float damage, const float totalArmor, const float toughnessAttribute);
	float getDamageAfterMagicAbsorb(const float damage, const float enchantModifiers);
};