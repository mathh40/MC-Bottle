#include "DamageSource.h"
#include <spdlog/fmt/bundled/format.h>
#include "instanceof.h"
#include "EntityDamageSource.h"
#include "EntityDamageSourceIndirect.h"

namespace DamageSource
{
	DamageSource::DamageSource(std::string damageTypeIn)
		:damageType(damageTypeIn)
	{

	}
	DamageSource DamageSource::causeMobDamage(EntityLivingBase mob)
	{
		return EntityDamageSource("mob", mob);
	}
	DamageSource DamageSource::causeIndirectDamage(Entity source, EntityLivingBase indirectEntityIn)
	{
		return EntityDamageSourceIndirect("mob", source, indirectEntityIn);
	}
	DamageSource DamageSource::causePlayerDamage(EntityPlayer player)
	{
		return EntityDamageSource("player", player);
	}
	DamageSource DamageSource::causeArrowDamage(EntityArrow arrow, std::optional<Entity> indirectEntityIn)
	{
		return EntityDamageSourceIndirect("arrow", arrow, indirectEntityIn).setProjectile();
	}
	DamageSource DamageSource::causeFireballDamage(EntityFireball fireball, std::optional<Entity> indirectEntityIn)
	{
		return !indirectEntityIn.has_value() ? EntityDamageSourceIndirect("onFire", fireball, fireball).setFireDamage().setProjectile() : EntityDamageSourceIndirect("fireball", fireball, indirectEntityIn).setFireDamage().setProjectile();
	}
	DamageSource DamageSource::causeThrownDamage(Entity source, std::optional<Entity> indirectEntityIn)
	{
		return EntityDamageSourceIndirect("thrown", source, indirectEntityIn).setProjectile();
	}
	DamageSource DamageSource::causeIndirectMagicDamage(Entity source, std::optional<Entity> indirectEntityIn)
	{
		return EntityDamageSourceIndirect("indirectMagic", source, indirectEntityIn).setDamageBypassesArmor().setMagicDamage();
	}
	DamageSource DamageSource::causeThornsDamage(Entity source)
	{
		return EntityDamageSource("thorns", source).setIsThornsDamage().setMagicDamage();
	}
	DamageSource DamageSource::causeExplosionDamage(std::optional<Explosion> explosionIn)
	{
		return explosionIn.has_value() && explosionIn.getExplosivePlacedBy() != nullptr ? EntityDamageSource("explosion.player", explosionIn.getExplosivePlacedBy()).setDifficultyScaled().setExplosion() : DamageSource("explosion").setDifficultyScaled().setExplosion();
	}
	DamageSource DamageSource::causeExplosionDamage(std::optional<EntityLivingBase> entityLivingBaseIn)
	{
		entityLivingBaseIn.has_value() ? EntityDamageSource("explosion.player", entityLivingBaseIn).setDifficultyScaled().setExplosion() : DamageSource("explosion").setDifficultyScaled().setExplosion();
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
	float DamageSource::getHungerDamage()
	{
		return hungerDamage;
	}
	bool DamageSource::canHarmInCreative() const
	{
		return DamageAllowedInCreativeMode;
	}
	bool DamageSource::isDamageAbsolute()
	{
		return damageIsAbsolute;
	}
	std::optional<Entity> DamageSource::getImmediateSource()
	{
		return getTrueSource();
	}
	std::optional<Entity> DamageSource::getTrueSource()
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
	ITextComponent DamageSource::getDeathMessage(EntityLivingBase entityLivingBaseIn)
	{
		EntityLivingBase entitylivingbase = entityLivingBaseIn.getAttackingEntity();
		std::string s = "death.attack." + damageType;
		std::string s1 = s + ".player";
		return I18n.canTranslate(s1) ? new TextComponentTranslation(s1, new Object[]{ entityLivingBaseIn.getDisplayName(), entitylivingbase.getDisplayName() }) : new TextComponentTranslation(s, new Object[]{ entityLivingBaseIn.getDisplayName() });
	}
	bool DamageSource::isFireDamage() const
	{
		return fireDamage;
	}
	std::string DamageSource::getDamageType()
	{
		return damageType;
	}
	DamageSource DamageSource::setDifficultyScaled()
	{
		difficultyScaled = true;
		return *this;
	}
	bool DamageSource::isDifficultyScaled()
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
		return instanceof<EntityPlayer>(entity) && ((EntityPlayer)entity).capabilities.isCreativeMode;
	}
	std::optional<Vec3d> DamageSource::getDamageLocation()
	{
		return {};
	}
}
