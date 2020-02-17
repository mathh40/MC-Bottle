#pragma once
#include <string>
#include <unordered_map>
#include <optional>

namespace DamageSource
{

	class DamageSource
	{
	public:
		explicit DamageSource(std::string damageTypeIn);
		std::string damageType;
		DamageSource causeMobDamage(EntityLivingBase mob);
		DamageSource causeIndirectDamage(Entity source, EntityLivingBase indirectEntityIn);
		DamageSource causePlayerDamage(EntityPlayer player);
		DamageSource causeArrowDamage(EntityArrow arrow, std::optional<Entity> indirectEntityIn);
		DamageSource causeFireballDamage(EntityFireball fireball, std::optional<Entity> indirectEntityIn);
		DamageSource causeThrownDamage(Entity source, std::optional<Entity> indirectEntityIn);
		DamageSource causeIndirectMagicDamage(Entity source, std::optional<Entity> indirectEntityIn);
		DamageSource causeThornsDamage(Entity source);
		DamageSource causeExplosionDamage(std::optional<Explosion> explosionIn);
		DamageSource causeExplosionDamage(std::optional<EntityLivingBase> entityLivingBaseIn);
		bool isProjectile() const;
		DamageSource setProjectile();
		bool isExplosion() const;
		DamageSource setExplosion();
		bool isUnblockable() const;
		float getHungerDamage();
		bool canHarmInCreative() const;
		bool isDamageAbsolute();
		std::optional<Entity> getImmediateSource();
		std::optional<Entity> getTrueSource();
		DamageSource setDamageBypassesArmor();
		DamageSource setDamageAllowedInCreativeMode();
		DamageSource setDamageIsAbsolute();
		DamageSource setFireDamage();
		ITextComponent getDeathMessage(EntityLivingBase entityLivingBaseIn);
		bool isFireDamage() const;
		std::string getDamageType();
		DamageSource setDifficultyScaled();
		bool isDifficultyScaled();
		bool isMagicDamage() const;
		DamageSource setMagicDamage();
		bool isCreativePlayer();
		std::optional<Vec3d> getDamageLocation();
	private:
		bool Unblockable = false;
		bool DamageAllowedInCreativeMode = false;
		bool damageIsAbsolute = false;
		float hungerDamage = 0.1F;
		bool fireDamage = false;
		bool projectile = false;
		bool difficultyScaled = false;
		bool magicDamage = false;
		bool explosion = false;
	};

	static const DamageSource IN_FIRE = DamageSource("inFire").setFireDamage();
	static const DamageSource LIGHTNING_BOLT = DamageSource("lightningBolt");
	static const DamageSource ON_FIRE = DamageSource("onFire").setDamageBypassesArmor().setFireDamage();
	static const DamageSource LAVA = DamageSource("lava").setFireDamage();
	static const DamageSource HOT_FLOOR = DamageSource("hotFloor").setFireDamage();
	static const DamageSource IN_WALL = DamageSource("inWall").setDamageBypassesArmor();
	static const DamageSource CRAMMING = DamageSource("cramming").setDamageBypassesArmor();
	static const DamageSource DROWN = DamageSource("drown").setDamageBypassesArmor();
	static const DamageSource STARVE = DamageSource("starve").setDamageBypassesArmor().setDamageIsAbsolute();
	static const DamageSource CACTUS = DamageSource("cactus");
	static const DamageSource FALL = DamageSource("fall").setDamageBypassesArmor();
	static const DamageSource FLY_INTO_WALL = DamageSource("flyIntoWall").setDamageBypassesArmor();
	static const DamageSource OUT_OF_WORLD = DamageSource("outOfWorld").setDamageBypassesArmor().setDamageAllowedInCreativeMode();
	static const DamageSource GENERIC = DamageSource("generic").setDamageBypassesArmor();
	static const DamageSource MAGIC = DamageSource("magic").setDamageBypassesArmor().setMagicDamage();
	static const DamageSource WITHER = DamageSource("wither").setDamageBypassesArmor();
	static const DamageSource ANVIL = DamageSource("anvil");
	static const DamageSource FALLING_BLOCK = DamageSource("fallingBlock");
	static const DamageSource DRAGON_BREATH = DamageSource("dragonBreath").setDamageBypassesArmor();
	static const DamageSource FIREWORKS = DamageSource("fireworks").setExplosion();
}
