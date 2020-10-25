#include "EntityTNTPrimed.h"

DataParameter EntityTNTPrimed::FUSE = EntityDataManager::createKey(EntityTNTPrimed.class, DataSerializers::VARINT);


EntityTNTPrimed::EntityTNTPrimed(World *worldIn)
    :Entity(worldIn),fuse(80){
        preventEntitySpawning = true;
        bisImmuneToFire = true;
        setSize(0.98F, 0.98F);
}

EntityTNTPrimed::EntityTNTPrimed(World *worldIn, double x, double y, double z, EntityLivingBase *igniter)
    :EntityTNTPrimed(worldIn){
    setPosition(x, y, z);
    float f = (float)(MathHelper::random() * 6.283185307179586);
    motionX = (double)(-((float)MathHelper::sin((double)f)) * 0.02F);
    motionY = 0.20000000298023224;
    motionZ = (double)(-((float)MathHelper::cos((double)f)) * 0.02F);
    setFuse(80);
    prevPosX = x;
    prevPosY = y;
    prevPosZ = z;
    tntPlacedBy = igniter;
}

bool EntityTNTPrimed::canBeCollidedWith() {
    return !isDead;
}

void EntityTNTPrimed::onUpdate() {
    prevPosX = posX;
      prevPosY = posY;
      prevPosZ = posZ;
      if (!hasNoGravity()) {
         motionY -= 0.03999999910593033;
      }

      move(MoverType::SELF, motionX, motionY, motionZ);
      motionX *= 0.9800000190734863;
      motionY *= 0.9800000190734863;
      motionZ *= 0.9800000190734863;
      if (onGround) {
         motionX *= 0.699999988079071;
         motionZ *= 0.699999988079071;
         motionY *= -0.5;
      }

      --fuse;
      if (fuse <= 0) {
         setDead();
         if (!world->isRemote) {
            explode();
         }
      } else {
         handleWaterMovement();
         world->spawnParticle(EnumParticleTypes::SMOKE_NORMAL, posX, posY + 0.5, posZ, 0.0, 0.0, 0.0);
      }
}

EntityLivingBase * EntityTNTPrimed::getTntPlacedBy() const {
    return tntPlacedBy;
}

float EntityTNTPrimed::getEyeHeight() const{
    return 0.0F;
}

void EntityTNTPrimed::setFuse(int32_t fuseIn) {
    dataManager.set(FUSE, fuseIn);
    fuse = fuseIn;
}

void EntityTNTPrimed::notifyDataManagerChange(DataParameter key) {
    if (FUSE == key) {
         fuse = getFuseDataManager();
      }
}

int32_t EntityTNTPrimed::getFuseDataManager() {
    return dataManager.get(FUSE);
}

int32_t EntityTNTPrimed::getFuse() const {
    return fuse;
}

void EntityTNTPrimed::entityInit() {
    dataManager.registe(FUSE, 80);
}

bool EntityTNTPrimed::canTriggerWalking() {
    return false;
}

void EntityTNTPrimed::writeEntityToNBT(NBTTagCompound *compound) {
    compound->setShort("Fuse", getFuse());
}

void EntityTNTPrimed::readEntityFromNBT(NBTTagCompound *compound) {
    setFuse(compound->getShort("Fuse"));
}

void EntityTNTPrimed::explode() {
    float f = 4.0F;
    world->createExplosion(this, posX, posY + (double)(height / 16.0F), posZ, 4.0F, true);
}
