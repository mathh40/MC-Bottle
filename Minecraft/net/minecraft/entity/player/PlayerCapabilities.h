#pragma once
class NBTTagCompound;

class PlayerCapabilities {
public:
    bool disableDamage;
    bool isFlying;
    bool allowFlying;
    bool isCreativeMode;
    bool allowEdit = true;

    void writeCapabilitiesToNBT(NBTTagCompound *tagCompound);
    void readCapabilitiesFromNBT(NBTTagCompound *tagCompound);
    float getFlySpeed() const;
    void setFlySpeed(float speed);
    float getWalkSpeed() const;
    void setPlayerWalkSpeed(float speed);


private:
    float flySpeed = 0.05F;
    float walkSpeed = 0.1F;
};
