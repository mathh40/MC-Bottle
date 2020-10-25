#pragma once
#include "IMob.h"
#include "../EntityCreature.h"

class EntityMob :public EntityCreature , public IMob {
public:
    EntityMob(World* worldIn);
    SoundCategory getSoundCategory() override;
    void onLivingUpdate() override;
    void onUpdate() override;
    bool attackEntityFrom(DamageSource::DamageSource source, float amount) override;
    bool attackEntityAsMob(Entity* entityIn) override;
    float getBlockPathWeight(BlockPos pos) override;
    bool getCanSpawnHere() override;
    bool isPreventingPlayerRest(EntityPlayer* playerIn);

protected:
    bool isValidLightLevel();
    void applyEntityAttributes() override;
    bool canDropLoot() override;

    SoundEvent getSwimSound() override;
    SoundEvent getSplashSound() override;
    SoundEvent getHurtSound(DamageSource::DamageSource damageSourceIn) override;
    SoundEvent getDeathSound() override;
    SoundEvent getFallSound(int32_t heightIn) override;

private:
};