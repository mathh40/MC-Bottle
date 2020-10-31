#pragma once
#include "EntityMob.h"

class EntityCreeper :public EntityMob {
public:
    EntityCreeper(World* worldIn);
    int32_t getMaxFallHeight() override;
    void fall(float distance, float damageMultiplier) override;
    static void registerFixesCreeper(DataFixer fixer);
    void writeEntityToNBT(NBTTagCompound* compound) override;
    void readEntityFromNBT(NBTTagCompound* compound) override;
    void onUpdate() override;
    void onDeath(DamageSource::DamageSource cause) override;
    bool attackEntityAsMob(Entity* entityIn) override;
    bool getPowered() const;
    float getCreeperFlashIntensity(float p_70831_1_) const;
    int32_t getCreeperState();
    void setCreeperState(int32_t state);
    void onStruckByLightning(EntityLightningBolt* lightningBolt) override;
    bool hasIgnited();
    void ignite();
    bool ableToCauseSkullDrop() const;
    void incrementDroppedSkulls();

protected:
    void initEntityAI() override;
    void applyEntityAttributes() override;
    void entityInit() override;
    SoundEvent getHurtSound(DamageSource::DamageSource damageSourceIn) override;
    SoundEvent getDeathSound() override;
    std::optional<ResourceLocation> getLootTable() override;
    bool processInteract(EntityPlayer* player, EnumHand hand) override;


private:
    void explode();
    void spawnLingeringCloud() const;

    static DataParameter STATE;
    static DataParameter POWERED;
    static DataParameter IGNITED;
    int32_t lastActiveTime;
    int32_t timeSinceIgnited;
    int32_t fuseTime = 30;
    int32_t explosionRadius = 3;
    int32_t droppedSkulls;
};