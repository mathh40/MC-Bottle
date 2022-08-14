#pragma once
#include "DamageSource.h"

namespace DamageSource
{
	class EntityDamageSource : public DamageSource
	{
	public:
		EntityDamageSource(std::string_view damageTypeIn, Entity* damageSourceEntityIn);
		EntityDamageSource setIsThornsDamage();
		bool getIsThornsDamage();
		std::optional <Entity> getTrueSource();
        std::shared_ptr<ITextComponent> getDeathMessage(EntityLivingBase *entityLivingBaseIn);
		bool isDifficultyScaled();
		Vec3d getDamageLocation();
	protected:
        Entity *damageSourceEntity;
	private:
		bool isThornsDamage;
	};
}
