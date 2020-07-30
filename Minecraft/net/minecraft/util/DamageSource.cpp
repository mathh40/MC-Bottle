#include "DamageSource.h"
#include <spdlog/fmt/bundled/format.h>
#include "EntityDamageSource.h"
#include "EntityDamageSourceIndirect.h"
#include "../entity/Entity.h"
#include "text/TextComponentTranslation.h"
#include "text/translation/I18n.h"

namespace DamageSource
{
	DamageSource::DamageSource(std::string_view damageTypeIn)
		:damageType(damageTypeIn)
	{

	}
	DamageSource DamageSource::causeMobDamage(EntityLivingBase* mob)
	{
		return EntityDamageSource("mob", mob);
	}
	DamageSource DamageSource::causeIndirectDamage(Entity* source, EntityLivingBase* indirectEntityIn)
	{
		return EntityDamageSourceIndirect("mob", source, indirectEntityIn);
	}
	DamageSource DamageSource::causePlayerDamage(EntityPlayer* player)
	{
		return EntityDamageSource("player", player);
	}
	DamageSource DamageSource::causeArrowDamage(EntityArrow* arrow, Entity* indirectEntityIn)
	{
		return EntityDamageSourceIndirect("arrow", arrow, indirectEntityIn).setProjectile();
	}
	DamageSource DamageSource::causeFireballDamage(EntityFireball* fireball, Entity* indirectEntityIn)
	{
		return !indirectEntityIn->has_value() ? EntityDamageSourceIndirect("onFire", fireball, fireball).setFireDamage().setProjectile() : EntityDamageSourceIndirect("fireball", fireball, indirectEntityIn).setFireDamage().setProjectile();
	}
	DamageSource DamageSource::causeThrownDamage(Entity* source, Entity* indirectEntityIn)
	{
		return EntityDamageSourceIndirect("thrown", source, indirectEntityIn).setProjectile();
	}
	DamageSource DamageSource::causeIndirectMagicDamage(Entity* source, Entity* indirectEntityIn)
	{
		return EntityDamageSourceIndirect("indirectMagic", source, indirectEntityIn).setDamageBypassesArmor().setMagicDamage();
	}
	DamageSource DamageSource::causeThornsDamage(Entity* source)
	{
		return EntityDamageSource("thorns", source).setIsThornsDamage().setMagicDamage();
	}
	DamageSource DamageSource::causeExplosionDamage(Explosion* explosionIn)
	{
		return explosionIn->has_value() && explosionIn->getExplosivePlacedBy() != nullptr ? EntityDamageSource("explosion.player", explosionIn->getExplosivePlacedBy()).setDifficultyScaled().setExplosion() : DamageSource("explosion").setDifficultyScaled().setExplosion();
	}
	DamageSource DamageSource::causeExplosionDamage(EntityLivingBase* entityLivingBaseIn)
	{
		entityLivingBaseIn->has_value() ? EntityDamageSource("explosion.player", entityLivingBaseIn).setDifficultyScaled().setExplosion() : DamageSource("explosion").setDifficultyScaled().setExplosion();
	}
	bool DamageSource::isProjectile() const
	{
		return projectile;
	}
	DamageSource DamageSource::setProjectile()
	{
		projectile = true;
		return *this;
	}
	bool DamageSource::isExplosion() const
	{
		return explosion;
	}
	DamageSource DamageSource::setExplosion()
	{
		explosion = true;
		return *this;
	}
	bool DamageSource::isUnblockable() const
	{
		return Unblockable;
	}
	float DamageSource::getHungerDamage() const
    {
		return hungerDamage;
	}
	bool DamageSource::canHarmInCreative() const
	{
		return DamageAllowedInCreativeMode;
	}
	bool DamageSource::isDamageAbsolute() const
    {
		return damageIsAbsolute;
	}
	Entity* DamageSource::getImmediateSource()
	{
		return getTrueSource();
	}
	Entity* DamageSource::getTrueSource()
	{
		return {};
	}
	DamageSource DamageSource::setDamageBypassesArmor()
	{
		Unblockable = true;
		hungerDamage = 0.0F;
		return *this;
	}
	DamageSource DamageSource::setDamageAllowedInCreativeMode()
	{
		DamageAllowedInCreativeMode = true;
		return *this;
	}
	DamageSource DamageSource::setDamageIsAbsolute()
	{
		damageIsAbsolute = true;
		hungerDamage = 0.0F;
		return *this;
	}
	DamageSource DamageSource::setFireDamage()
	{
		fireDamage = true;
		return *this;
	}
	ITextComponent* DamageSource::getDeathMessage(EntityLivingBase* entityLivingBaseIn)
	{
		EntityLivingBase* entitylivingbase = entityLivingBaseIn->getAttackingEntity();
		std::string s = "death.attack." + damageType;
		std::string s1 = s + ".player";
		return I18n::canTranslate(s1) ? new TextComponentTranslation(s1, new Object[]{ entityLivingBaseIn->getDisplayName(), entitylivingbase.getDisplayName() }) : new TextComponentTranslation(s, new Object[]{ entityLivingBaseIn->getDisplayName() });
	}
	bool DamageSource::isFireDamage() const
	{
		return fireDamage;
	}
	std::string DamageSource::getDamageType() const
    {
		return damageType;
	}
	DamageSource DamageSource::setDifficultyScaled()
	{
		difficultyScaled = true;
		return *this;
	}
	bool DamageSource::isDifficultyScaled() const
    {
		return difficultyScaled;
	}
	bool DamageSource::isMagicDamage() const
	{
		return magicDamage;
	}
	DamageSource DamageSource::setMagicDamage()
	{
		magicDamage = true;
		return *this;
	}
	bool DamageSource::isCreativePlayer()
	{
		auto entity = getTrueSource();
		return Util::instanceof<EntityPlayer>(entity) && ((EntityPlayer*)entity)->capabilities.isCreativeMode;
	}
	std::optional<Vec3d> DamageSource::getDamageLocation()
	{
		return {};
	}
}
