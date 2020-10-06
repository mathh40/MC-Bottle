#pragma once
#include "EntityWeatherEffect.h"

class EntityLightningBolt :public EntityWeatherEffect {
public:
    EntityLightningBolt(World* worldIn, double x, double y, double z, bool effectOnlyIn);
    SoundCategory getSoundCategory() override;
    void onUpdate() override;

    int64_t boltVertex;
protected:
    void entityInit() override;
    void readEntityFromNBT(NBTTagCompound* compound) override;
    void writeEntityToNBT(NBTTagCompound* compound) override;
private:
    int32_t lightningState;
    int32_t boltLivingTime;
    bool effectOnly;
};