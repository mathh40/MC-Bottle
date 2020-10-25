#include "EntityXPOrb.h"

#include "DamageSource.h"

EntityXPOrb::EntityXPOrb(World *worldIn, double x, double y, double z, int32_t expValue)
    :Entity(worldIn){
    setSize(0.5F, 0.5F);
    setPosition(x, y, z);
    rotationYaw = (float)(MathHelper::random() * 360.0);
    motionX = (double)((float)(MathHelper::random() * 0.20000000298023224 - 0.10000000149011612) * 2.0F);
    motionY = (double)((float)(MathHelper::random() * 0.2) * 2.0F);
    motionZ = (double)((float)(MathHelper::random() * 0.20000000298023224 - 0.10000000149011612) * 2.0F);
    xpValue = expValue;
}

EntityXPOrb::EntityXPOrb(World *worldIn)
    :Entity(worldIn){
    setSize(0.25F, 0.25F);
}

int32_t EntityXPOrb::getBrightnessForRender() {
    float f = 0.5F;
    f = MathHelper::clamp(f, 0.0F, 1.0F);
    int32_t i = Entity::getBrightnessForRender();
    int32_t j = i & 255;
    int32_t k = i >> 16 & 255;
    j += (int32_t)(f * 15.0F * 16.0F);
    if (j > 240) {
        j = 240;
    }

    return j | k << 16;
}

void EntityXPOrb::onUpdate() {
    Entity::onUpdate();
      if (delayBeforeCanPickup > 0) {
         --delayBeforeCanPickup;
      }

      prevPosX = posX;
      prevPosY = posY;
      prevPosZ = posZ;
      if (!hasNoGravity()) {
         motionY -= 0.029999999329447746;
      }

      if (world->getBlockState(BlockPos(this))->getMaterial() == Material::LAVA) {
         motionY = 0.20000000298023224;
         motionX = (double)((MathHelper::nextFloat(rand) - MathHelper::nextFloat(rand)) * 0.2F);
         motionZ = (double)((MathHelper::nextFloat(rand) - MathHelper::nextFloat(rand)) * 0.2F);
         playSound(SoundEvents::ENTITY_GENERIC_BURN, 0.4F, 2.0F + MathHelper::nextFloat(rand) * 0.4F);
      }

      pushOutOfBlocks(posX, (getEntityBoundingBox().getminY() + getEntityBoundingBox().getmaxY()) / 2.0, posZ);
      double d0 = 8.0;
      if (xpTargetColor < xpColor - 20 + getEntityId() % 100) {
         if (closestPlayer == nullptr || closestPlayer->getDistanceSq(this) > 64.0) {
            closestPlayer = world->getClosestPlayerToEntity(this, 8.0);
         }

         xpTargetColor = xpColor;
      }

      if (closestPlayer != nullptr && closestPlayer->isSpectator()) {
         closestPlayer = nullptr;
      }

      if (closestPlayer != nullptr) {
         double d1 = (closestPlayer->posX - posX) / 8.0;
         double d2 = (closestPlayer->posY + (double)closestPlayer->getEyeHeight() / 2.0 - posY) / 8.0;
         double d3 = (closestPlayer->posZ - posZ) / 8.0;
         double d4 = MathHelper::sqrt(d1 * d1 + d2 * d2 + d3 * d3);
         double d5 = 1.0 - d4;
         if (d5 > 0.0) {
            d5 *= d5;
            motionX += d1 / d4 * d5 * 0.1;
            motionY += d2 / d4 * d5 * 0.1;
            motionZ += d3 / d4 * d5 * 0.1;
         }
      }

      move(MoverType::SELF, motionX, motionY, motionZ);
      float f = 0.98F;
      if (onGround) {
         f = world->getBlockState(BlockPos(MathHelper::floor(posX), MathHelper::floor(getEntityBoundingBox().getminY()) - 1, MathHelper::floor(posZ)))->getBlock()->slipperiness * 0.98F;
      }

      motionX *= (double)f;
      motionY *= 0.9800000190734863;
      motionZ *= (double)f;
      if (onGround) {
         motionY *= -0.8999999761581421;
      }

      ++xpColor;
      ++xpOrbAge;
      if (xpOrbAge >= 6000) {
         setDead();
      }
}

bool EntityXPOrb::handleWaterMovement() {
    return world->handleMaterialAcceleration(getEntityBoundingBox(), Material::WATER, this);
}

bool EntityXPOrb::attackEntityFrom(DamageSource::DamageSource source, float amount) {
    if (isEntityInvulnerable(source)) {
         return false;
      } else {
         markVelocityChanged();
         xpOrbHealth = (int)((float)xpOrbHealth - amount);
         if (xpOrbHealth <= 0) {
            setDead();
         }

         return false;
      }
}

void EntityXPOrb::writeEntityToNBT(NBTTagCompound *compound) {
    compound->setShort("Health", xpOrbHealth);
    compound->setShort("Age", xpOrbAge);
    compound->setShort("Value",xpValue);
}

void EntityXPOrb::readEntityFromNBT(NBTTagCompound *compound) {
    xpOrbHealth = compound->getShort("Health");
    xpOrbAge = compound->getShort("Age");
    xpValue = compound->getShort("Value");
}

int32_t EntityXPOrb::durabilityToXp(int32_t durability) const{
    return durability / 2;
}

int32_t EntityXPOrb::xpToDurability(int32_t xp) const {
    return xp * 2;
}

void EntityXPOrb::onCollideWithPlayer(EntityPlayer *entityIn) {
    if (!world->isRemote && delayBeforeCanPickup == 0 && entityIn->xpCooldown == 0) {
         entityIn->xpCooldown = 2;
         entityIn->onItemPickup(this, 1);
         ItemStack itemstack = EnchantmentHelper::getEnchantedItem(Enchantments::MENDING, entityIn);
         if (!itemstack.isEmpty() && itemstack.isItemDamaged()) {
            auto i = MathHelper::min(xpToDurability(xpValue), itemstack.getItemDamage());
            xpValue -= durabilityToXp(i);
            itemstack.setItemDamage(itemstack.getItemDamage() - i);
         }

         if (xpValue > 0) {
            entityIn->addExperience(xpValue);
         }

         setDead();
      }
}

int32_t EntityXPOrb::getXpValue() const {
    return xpValue;
}

int32_t EntityXPOrb::getTextureByXP() const {
    if (xpValue >= 2477) {
         return 10;
      } else if (xpValue >= 1237) {
         return 9;
      } else if (xpValue >= 617) {
         return 8;
      } else if (xpValue >= 307) {
         return 7;
      } else if (xpValue >= 149) {
         return 6;
      } else if (xpValue >= 73) {
         return 5;
      } else if (xpValue >= 37) {
         return 4;
      } else if (xpValue >= 17) {
         return 3;
      } else if (xpValue >= 7) {
         return 2;
      } else {
         return xpValue >= 3 ? 1 : 0;
      }
}

int32_t EntityXPOrb::getXPSplit(int32_t expValue) {
    if (expValue >= 2477) {
         return 2477;
      } else if (expValue >= 1237) {
         return 1237;
      } else if (expValue >= 617) {
         return 617;
      } else if (expValue >= 307) {
         return 307;
      } else if (expValue >= 149) {
         return 149;
      } else if (expValue >= 73) {
         return 73;
      } else if (expValue >= 37) {
         return 37;
      } else if (expValue >= 17) {
         return 17;
      } else if (expValue >= 7) {
         return 7;
      } else {
         return expValue >= 3 ? 3 : 1;
      }
}

bool EntityXPOrb::canBeAttackedWithItem() {
    return false;
}

bool EntityXPOrb::canTriggerWalking() {
    return false;
}

void EntityXPOrb::entityInit() {

}

void EntityXPOrb::dealFireDamage(int32_t amount) {
    attackEntityFrom(DamageSource::IN_FIRE, (float)amount);
}
