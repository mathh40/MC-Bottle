#pragma once
#include "../potion/PotionType.h"
#include "Entity.h"

class EntityAreaEffectCloud : public Entity
{
  public:
    EntityAreaEffectCloud(World *worldIn);
    EntityAreaEffectCloud(World *worldIn, double x, double y, double z);
    void setRadius(float radiusIn);
    float getRadius();
    void setPotion(const PotionType &potionIn);
    void addEffect(const PotionEffect &effect);
    int32_t getColor();
    void setColor(int32_t colorIn);
    EnumParticleTypes getParticle();
    void setParticle(const EnumParticleTypes &particleIn);
    int32_t getParticleParam1();
    void setParticleParam1(int32_t particleParam);
    int32_t getParticleParam2();
    void setParticleParam2(int32_t particleParam);
    bool shouldIgnoreRadius();
    int32_t getDuration() const;
    void setDuration(int32_t durationIn);
    void onUpdate() override;
    void setRadiusOnUse(float radiusOnUseIn);
    void setRadiusPerTick(float radiusPerTickIn);
    void setWaitTime(int32_t waitTimeIn);
    void setOwner(EntityLivingBase *ownerIn);
    EntityLivingBase *getOwner();
    void notifyDataManagerChange(const DataParameter &key) override;
    EnumPushReaction getPushReaction() override;

  protected:
    void entityInit() override;
    void setIgnoreRadius(bool ignoreRadius);
    void readEntityFromNBT(NBTTagCompound *compound) override;
    void writeEntityToNBT(NBTTagCompound *compound) override;

  private:
    void updateFixedColor();

    static DataParameter RADIUS;
    static DataParameter COLOR;
    static DataParameter IGNORE_RADIUS;
    static DataParameter PARTICLE;
    static DataParameter PARTICLE_PARAM_1;
    static DataParameter PARTICLE_PARAM_2;
    PotionType potion;
    std::vector<PotionEffect> effects;
    std::unordered_map<> reapplicationDelayMap;
    int32_t duration;
    int32_t waitTime;
    int32_t reapplicationDelay;
    bool colorSet;
    int32_t durationOnUse;
    float radiusOnUse;
    float radiusPerTick;
    EntityLivingBase *owner;
    xg::Guid ownerUniqueId;
    Util::RandomizerWithSentinelShift<> randombool;
};
