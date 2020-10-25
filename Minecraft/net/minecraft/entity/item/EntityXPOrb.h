#pragma once
#include "../Entity.h"

class EntityXPOrb :public Entity {
public:
    EntityXPOrb(World* worldIn, double x, double y, double z, int32_t expValue);
    explicit EntityXPOrb(World* worldIn);
    int32_t getBrightnessForRender() override;
    void onUpdate() override;
    bool handleWaterMovement();
    bool attackEntityFrom(DamageSource::DamageSource source, float amount) override;
    void writeEntityToNBT(NBTTagCompound* compound) override;
    void readEntityFromNBT(NBTTagCompound* compound) override;
    void onCollideWithPlayer(EntityPlayer* entityIn) override;
    int32_t getXpValue() const;
    int32_t getTextureByXP() const;
    static int32_t getXPSplit(int32_t expValue);
    bool canBeAttackedWithItem() override;

    int32_t xpColor;
    int32_t xpOrbAge;
    int32_t delayBeforeCanPickup;

protected:
    bool canTriggerWalking() override;
    void entityInit() override;
    void dealFireDamage(int32_t amount) override;

private:
    int32_t durabilityToXp(int32_t durability) const;
    int32_t xpToDurability(int32_t xp) const;

    int32_t xpOrbHealth = 5;
    int32_t xpValue;
    EntityPlayer* closestPlayer;
    int32_t xpTargetColor;
};
