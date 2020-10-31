#include "PlayerCapabilities.h"

#include "../../nbt/NBTTagCompound.h"

void PlayerCapabilities::writeCapabilitiesToNBT(NBTTagCompound *tagCompound) {
    NBTTagCompound *nbttagcompound = new NBTTagCompound();
    nbttagcompound->setBoolean("invulnerable", disableDamage);
    nbttagcompound->setBoolean("flying", isFlying);
    nbttagcompound->setBoolean("mayfly", allowFlying);
    nbttagcompound->setBoolean("instabuild", isCreativeMode);
    nbttagcompound->setBoolean("mayBuild", allowEdit);
    nbttagcompound->setFloat("flySpeed", flySpeed);
    nbttagcompound->setFloat("walkSpeed", walkSpeed);
    tagCompound->setTag("abilities", nbttagcompound);
}

void PlayerCapabilities::readCapabilitiesFromNBT(NBTTagCompound *tagCompound) {
    if (tagCompound->hasKey("abilities", 10)) {
        NBTTagCompound *nbttagcompound = tagCompound->getCompoundTag("abilities");
        disableDamage = nbttagcompound->getBoolean("invulnerable");
        isFlying = nbttagcompound->getBoolean("flying");
        allowFlying = nbttagcompound->getBoolean("mayfly");
        isCreativeMode = nbttagcompound->getBoolean("instabuild");
        if (nbttagcompound->hasKey("flySpeed", 99)) {
            flySpeed = nbttagcompound->getFloat("flySpeed");
            walkSpeed = nbttagcompound->getFloat("walkSpeed");
        }

        if (nbttagcompound->hasKey("mayBuild", 1)) {
            allowEdit = nbttagcompound->getBoolean("mayBuild");
        }
    }
}

float PlayerCapabilities::getFlySpeed() const {
    return flySpeed;
}

void PlayerCapabilities::setFlySpeed(float speed) {
    flySpeed = speed;
}

float PlayerCapabilities::getWalkSpeed() const {
    return walkSpeed;
}

void PlayerCapabilities::setPlayerWalkSpeed(float speed) {
    walkSpeed = speed;
}
