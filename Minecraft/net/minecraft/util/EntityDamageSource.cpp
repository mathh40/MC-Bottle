#include "EntityDamageSource.h"
#include "instanceof.h"
#include "math/Vec3d.h"

DamageSource::EntityDamageSource::EntityDamageSource(std::string damageTypeIn, std::optional<Entity> damageSourceEntityIn)
	:DamageSource(damageTypeIn), damageSourceEntity(damageSourceEntityIn)
{
}

DamageSource::EntityDamageSource DamageSource::EntityDamageSource::setIsThornsDamage()
{
	isThornsDamage = true;
	return *this;
}

bool DamageSource::EntityDamageSource::getIsThornsDamage()
{
	return isThornsDamage;
}

std::optional<Entity> DamageSource::EntityDamageSource::getTrueSource()
{
	return std::optional<Entity>();
}

ITextComponent DamageSource::EntityDamageSource::getDeathMessage(EntityLivingBase entityLivingBaseIn)
{
	ItemStack itemstack = instanceof<EntityLivingBase>(damageSourceEntity) ? ((EntityLivingBase)damageSourceEntity).getHeldItemMainhand() : ItemStack.EMPTY;
	auto s = "death.attack." + damageType;
	auto s1 = s + ".item";
	return !itemstack.isEmpty() && itemstack.hasDisplayName() && I18n.canTranslate(s1) ? new TextComponentTranslation(s1, new Object[]{ entityLivingBaseIn.getDisplayName(), damageSourceEntity.getDisplayName(), itemstack.getTextComponent() }) : new TextComponentTranslation(s, new Object[]{ entityLivingBaseIn.getDisplayName(), damageSourceEntity.getDisplayName() });
}

bool DamageSource::EntityDamageSource::isDifficultyScaled()
{
	return damageSourceEntity.has_value() && instanceof<EntityLivingBase>(damageSourceEntity.value()) && !(instanceof<EntityPlayer>(damageSourceEntity.value()));
}

Vec3d DamageSource::EntityDamageSource::getDamageLocation()
{
	return Vec3d(damageSourceEntity.value().posX, damageSourceEntity.value().posY, damageSourceEntity.value().posZ);
}
