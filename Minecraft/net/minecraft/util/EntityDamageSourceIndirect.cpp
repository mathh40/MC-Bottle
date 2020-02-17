#include "EntityDamageSourceIndirect.h"
#include "instanceof.h"

namespace DamageSource
{

	EntityDamageSourceIndirect::EntityDamageSourceIndirect(std::string damageTypeIn, Entity source, std::optional<Entity> indirectEntityIn)
		:EntityDamageSource(damageTypeIn, std::make_optional(source)), indirectEntity(indirectEntityIn)
	{
	}
	std::optional<Entity> EntityDamageSourceIndirect::getImmediateSource()
	{
		return damageSourceEntity;
	}
	std::optional<Entity> EntityDamageSourceIndirect::getTrueSource()
	{
		return indirectEntity;
	}
	ITextComponent EntityDamageSourceIndirect::getDeathMessage(EntityLivingBase entityLivingBaseIn)
	{
		ITextComponent itextcomponent = !indirectEntity.has_value() ? damageSourceEntity.getDisplayName() : indirectEntity.getDisplayName();
		ItemStack itemstack =  instanceof<EntityLivingBase>(indirectEntity) ? ((EntityLivingBase)indirectEntity).getHeldItemMainhand() : ItemStack.EMPTY;
		auto s = "death.attack." + damageType;
		auto s1 = s + ".item";
		return !itemstack.isEmpty() && itemstack.hasDisplayName() && I18n.canTranslate(s1) ? new TextComponentTranslation(s1, new Object[]{ entityLivingBaseIn.getDisplayName(), itextcomponent, itemstack.getTextComponent() }) : new TextComponentTranslation(s, new Object[]{ entityLivingBaseIn.getDisplayName(), itextcomponent });
	}
}
