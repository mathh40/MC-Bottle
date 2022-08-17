#pragma once
#include "../world/Explosion.h"

#include <memory>
#include <string>
#include <optional>

class ITextComponent;
class EntityArrow;
class EntityPlayer;
class Entity;
class EntityLivingBase;
class EntityFireball;

namespace DamageSource
{
	class DamageSource
	{
	public:
		explicit DamageSource(std::string_view damageTypeIn);
		std::string damageType;
		std::unique_ptr<DamageSource> causeMobDamage(EntityLivingBase* mob);
		std::unique_ptr<DamageSource> causeIndirectDamage(Entity* source, EntityLivingBase* indirectEntityIn);
		std::unique_ptr<DamageSource> causePlayerDamage(EntityPlayer* player);
		std::unique_ptr<DamageSource> causeArrowDamage(EntityArrow* arrow, Entity* indirectEntityIn);
		std::unique_ptr<DamageSource> causeFireballDamage(EntityFireball* fireball,Entity* indirectEntityIn);
		std::unique_ptr<DamageSource> causeThrownDamage(Entity* source, Entity* indirectEntityIn);
		std::unique_ptr<DamageSource> causeIndirectMagicDamage(Entity* source, Entity* indirectEntityIn);
		std::unique_ptr<DamageSource> causeThornsDamage(Entity* source);
		std::unique_ptr<DamageSource> causeExplosionDamage(std::optional<Explosion> explosionIn);
		std::unique_ptr<DamageSource> causeExplosionDamage(EntityLivingBase* entityLivingBaseIn);
		bool isProjectile() const;
		DamageSource setProjectile();
		bool isExplosion() const;
		DamageSource setExplosion();
		bool isUnblockable() const;
		float getHungerDamage() const;
		bool canHarmInCreative() const;
		bool isDamageAbsolute() const;
		Entity* getImmediateSource();
		Entity* getTrueSource();
		DamageSource setDamageBypassesArmor();
		DamageSource setDamageAllowedInCreativeMode();
		DamageSource setDamageIsAbsolute();
		DamageSource setFireDamage();
        std::shared_ptr<ITextComponent> getDeathMessage(EntityLivingBase* entityLivingBaseIn);
		bool isFireDamage() const;
		std::string getDamageType() const;
		DamageSource setDifficultyScaled();
		bool isDifficultyScaled() const;
		bool isMagicDamage() const;
		DamageSource setMagicDamage();
		bool isCreativePlayer();
        std::optional<Vec3d> getDamageLocation();

		friend bool operator==(const DamageSource & lhs,const DamageSource & rhs);
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
