#pragma once
#include "BossInfoServer.h"
#include "../IRangedAttackMob.h"
#include "../ai/EntityAIBase.h"
#include "../monster/EntityMob.h"

class EntityWither;

class AIDoNothing :public EntityAIBase {
  public:
    AIDoNothing();

    bool shouldExecute() override;
private:
    EntityWither* witherentity;
   };

class EntityWither :public EntityMob ,public IRangedAttackMob {
public:
    EntityWither(World* worldIn);
    static void registerFixesWither(DataFixer fixer);
    void writeEntityToNBT(NBTTagCompound* compound) override;
    void readEntityFromNBT(NBTTagCompound* compound) override;
    void setCustomNameTag(std::string_view name);
    void onLivingUpdate() override;
    static bool canDestroyBlock(Block* blockIn);
    void ignite();
    void setInWeb() override;
    void addTrackingPlayer(EntityPlayerMP* player) override;
    void removeTrackingPlayer(EntityPlayerMP* player) override;
    void attackEntityWithRangedAttack(EntityLivingBase* target, float distanceFactor) override;
    bool attackEntityFrom(DamageSource::DamageSource source, float amount) override;
    int32_t getBrightnessForRender() override;
    void fall(float distance, float damageMultiplier) override;
    void addPotionEffect(PotionEffect potioneffectIn) override;
    float getHeadYRotation(int32_t p_82207_1_);
    float getHeadXRotation(int32_t p_82210_1_);
    int32_t getInvulTime();
    void setInvulTime(int32_t time);
    int32_t getWatchedTargetId(int32_t head);
    void updateWatchedTargetId(int32_t targetOffset, int32_t newId);
    bool isArmored();
    EnumCreatureAttribute getCreatureAttribute() override;
    void setSwingingArms(bool swingingArms) override;

protected:
    void initEntityAI() override;
    void entityInit() override;
    SoundEvent getAmbientSound() override;
    SoundEvent getHurtSound(DamageSource::DamageSource damageSourceIn) override;
    SoundEvent getDeathSound() override;
    void updateAITasks() override;
    void dropFewItems(bool wasRecentlyHit, int32_t lootingModifier) override;
    void despawnEntity() override;
    void applyEntityAttributes() override;
    bool canBeRidden(Entity* entityIn) override;
    bool isNonBoss() override;


private:
    double getHeadX(int32_t p_82214_1_) const;
    double getHeadY(int32_t p_82208_1_) const;
    double getHeadZ(int32_t p_82213_1_) const;
    float rotlerp(float p_82204_1_, float p_82204_2_, float p_82204_3_) const;
    void launchWitherSkullToEntity(int32_t p_82216_1_, EntityLivingBase* p_82216_2_);
    void launchWitherSkullToCoords(int32_t p_82209_1_, double x, double y, double z, bool invulnerable);


    static DataParameter FIRST_HEAD_TARGET;
    static DataParameter SECOND_HEAD_TARGET;
    static DataParameter THIRD_HEAD_TARGET;
    static std::array<DataParameter,3> HEAD_TARGETS;
    static DataParameter INVULNERABILITY_TIME;
    std::array<float,2> xRotationHeads;
    std::array<float,2> yRotationHeads;
    std::array<float,2> xRotOHeads;
    std::array<float,2> yRotOHeads;
    std::array<int,2> nextHeadUpdate;
    std::array<int,2> idleHeadUpdates;
    int32_t blockBreakCounter;
    BossInfoServer bossInfo;
    static inline auto NOT_UNDEAD = [](Entity* p_apply_1_)
{
    return Util::instanceof<EntityLivingBase>(p_apply_1_) && ((EntityLivingBase*)p_apply_1_)->getCreatureAttribute() != EnumCreatureAttribute::UNDEAD && ((EntityLivingBase*)p_apply_1_)->attackable();
};;
};
