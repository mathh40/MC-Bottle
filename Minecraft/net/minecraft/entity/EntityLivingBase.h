#pragma once
#include "CombatTracker.h"
#include "Entity.h"
#include "../item/ItemStack.h"
#include "../potion/PotionEffect.h"
#include "../util/CombatTracker.h"
#include "ai/attributes/AttributeModifier.h"
class AbstractAttributeMap;
class EntityLivingBase :public Entity
{
public:
    explicit EntityLivingBase(World* worldIn);
    void onKillCommand() override;
    bool canBreatheUnderwater();
    void onEntityUpdate() override;
    bool isChild();
    pcg32& getRNG();
    EntityLivingBase* getRevengeTarget() const;
    void setRevengeTarget(EntityLivingBase* livingBase);
    EntityLivingBase* getLastAttackedEntity() const;
    int32_t getLastAttackedEntityTime() const;
    void setLastAttackedEntity(Entity* entityIn);
    int32_t getIdleTime() const;
    void writeEntityToNBT(NBTTagCompound* compound) override;
    void readEntityFromNBT(NBTTagCompound* compound) override;
    static bool areAllPotionsAmbient(const std::vector<PotionEffect>& potionEffects);
    void clearActivePotions();
    std::vector<PotionEffect> getActivePotionEffects() const;
    std::unordered_map<Potion,PotionEffect> getActivePotionMap() const;
    bool isPotionActive(Potion potionIn) const;
    std::optional<PotionEffect> getActivePotionEffect(Potion potionIn);
    void addPotionEffect(PotionEffect potioneffectIn);
    bool isPotionApplicable(PotionEffect potioneffectIn);
    bool isEntityUndead();
    PotionEffect removeActivePotionEffect(Potion* potioneffectin);
    void removePotionEffect(Potion* potionIn);
    void heal(float healAmount);
    float getHealth();
    void setHealth(float health);
    bool attackEntityFrom(DamageSource source, float amount);
    DamageSource getLastDamageSource();
    void renderBrokenItemStack(const ItemStack& stack);
    void onDeath(DamageSource cause);


    bool isSwingInProgress;
    EnumHand swingingHand;
    int32_t swingProgressInt;
    int32_t arrowHitTimer;
    int32_t hurtTime;
    int32_t maxHurtTime;
    float attackedAtYaw;
    int32_t deathTime;
    float prevSwingProgress;
    float swingProgress;
    float prevLimbSwingAmount;
    float limbSwingAmount;
    float limbSwing;
    int32_t maxHurtResistantTime;
    float prevCameraPitch;
    float cameraPitch;
    float randomUnused2;
    float randomUnused1;
    float renderYawOffset;
    float prevRenderYawOffset;
    float rotationYawHead;
    float prevRotationYawHead;
    float jumpMovementFactor;
    float moveStrafing;
    float moveVertical;
    float moveForward;
    float randomYawVelocity;
protected:
    void entityInit() override;
    void applyEntityAttributes();
    void updateFallState(double y, bool onGroundIn, IBlockState* state, const BlockPos& pos) override;
    void frostWalk(const BlockPos& pos);
    void onDeathUpdate();
    bool canDropLoot();
    int32_t decreaseAirSupply(int32_t air);
    int32_t getExperiencePoints(EntityPlayer* player);
    bool isPlayer();
    void playEquipSound(ItemStack stack);
    void updatePotionEffects();
    void updatePotionMetadata();
    void resetPotionEffectMetadata();
    void onNewPotionEffect(PotionEffect id);
    void onChangedPotionEffect(PotionEffect id, bool p_70695_2_);
    void onFinishedPotionEffect(PotionEffect effect);
    void blockUsingShield(EntityLivingBase* p_190629_1_);
    void playHurtSound(DamageSource source);

    static DataParameter HAND_STATES;
    int32_t ticksSinceLastSwing;
    EntityPlayer* attackingPlayer;
    int32_t recentlyHit;
    bool dead;
    int idleTime;
    float prevOnGroundSpeedFactor;
    float onGroundSpeedFactor;
    float movedDistance;
    float prevMovedDistance;
    float unused180;
    int32_t scoreValue;
    float lastDamage;
    bool isJumping;
    int32_t newPosRotationIncrements;
    double interpTargetX;
    double interpTargetY;
    double interpTargetZ;
    double interpTargetYaw;
    double interpTargetPitch;
    ItemStack activeItemStack;
    int32_t activeItemStackUseCount;
    int32_t ticksElytraFlying;
private:
    bool checkTotemDeathProtection(DamageSource p_190628_1_);
    bool canBlockDamageSource(DamageSource damageSourceIn);

    static std::shared_ptr<spdlog::logger> LOGGER;
    static xg::Guid SPRINTING_SPEED_BOOST_ID;
    static AttributeModifier SPRINTING_SPEED_BOOST;    
    static DataParameter HEALTH;
    static DataParameter POTION_EFFECTS;
    static DataParameter HIDE_PARTICLES;
    static DataParameter ARROW_COUNT_IN_ENTITY;
    AbstractAttributeMap* attributeMap;
    CombatTracker combatTracker;
    std::unordered_map<Potion,PotionEffect> activePotionsMap;
    std::vector<ItemStack> handInventory;
    std::vector<ItemStack> armorArray;
    bool potionsNeedUpdate;
    EntityLivingBase* revengeTarget;
    int32_t revengeTimer;
    EntityLivingBase* lastAttackedEntity;
    int32_t lastAttackedEntityTime;
    float landMovementFactor;
    int32_t jumpTicks;
    float absorptionAmount;
    BlockPos prevBlockpos;
    DamageSource lastDamageSource;
    int64_t lastDamageStamp;
};