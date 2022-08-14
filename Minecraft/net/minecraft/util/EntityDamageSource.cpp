#include "EntityDamageSource.h"
#include "math/Vec3d.h"
#include <Util.h>
#include <text/TextComponentTranslation.h>
#include <text/translation/I18n.h>
#include "../item/ItemStack.h"

DamageSource::EntityDamageSource::EntityDamageSource(std::string_view damageTypeIn, Entity *damageSourceEntityIn)
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

std::shared_ptr<ITextComponent> DamageSource::EntityDamageSource::getDeathMessage(EntityLivingBase* entityLivingBaseIn)
{
	auto itemstack = Util::instanceof<EntityLivingBase*>(damageSourceEntity) ? reinterpret_cast<EntityLivingBase*>(damageSourceEntity)->getHeldItemMainhand() : ItemStack::EMPTY;
	auto s = "death.attack." + damageType;
	auto s1 = s + ".item";
	return !itemstack.isEmpty() && itemstack.hasDisplayName() && I18n::canTranslate(s1) ? std::make_shared<TextComponentTranslation>(s1, entityLivingBaseIn->getDisplayName(), damageSourceEntity->getDisplayName(), itemstack.getTextComponent()) : new TextComponentTranslation(s, new Object[]{ entityLivingBaseIn.getDisplayName(), damageSourceEntity.getDisplayName() });
}

bool DamageSource::EntityDamageSource::isDifficultyScaled()
{
	return damageSourceEntity.has_value() && instanceof<EntityLivingBase>(damageSourceEntity.value()) && !(instanceof<EntityPlayer>(damageSourceEntity.value()));
}

Vec3d DamageSource::EntityDamageSource::getDamageLocation()
{
	return Vec3d(damageSourceEntity.value().posX, damageSourceEntity.value().posY, damageSourceEntity.value().posZ);
}
