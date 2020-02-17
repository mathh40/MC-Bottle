#pragma once
#include "DamageSource.h"

namespace DamageSource
{
	class EntityDamageSource : public DamageSource
	{
	public:
		EntityDamageSource(std::string damageTypeIn, std::optional<Entity> damageSourceEntityIn);
		EntityDamageSource setIsThornsDamage();
		bool getIsThornsDamage();
		std::optional <Entity> getTrueSource();
		ITextComponent getDeathMessage(EntityLivingBase entityLivingBaseIn);
		bool isDifficultyScaled();
		Vec3d getDamageLocation();
	protected:
		std::optional<Entity> damageSourceEntity;
	private:
		bool isThornsDamage;
	};
}
