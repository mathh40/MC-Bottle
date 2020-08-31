#include "EntityLeashKnot.h"

#include "EntityLiving.h"

EntityLeashKnot::EntityLeashKnot(World *worldIn)
    :EntityHanging(worldIn){

}

EntityLeashKnot::EntityLeashKnot(World *worldIn, BlockPos hangingPositionIn)
:EntityHanging(worldIn){
    setPosition(hangingPositionIn.getx() + 0.5, hangingPositionIn.gety() + 0.5, hangingPositionIn.getz() + 0.5);
    setEntityBoundingBox(AxisAlignedBB(posX - 0.1875, posY - 0.25 + 0.125, posZ - 0.1875, posX + 0.1875, posY + 0.25 + 0.125, posZ + 0.1875));
    forceSpawn = true;
}

void EntityLeashKnot::setPosition(double x, double y, double z) {
    EntityHanging::setPosition(MathHelper::floor(x) + 0.5, MathHelper::floor(y) + 0.5, MathHelper::floor(z) + 0.5);
}

void EntityLeashKnot::updateFacingWithBoundingBox(EnumFacing facingDirectionIn) {

}

int32_t EntityLeashKnot::getWidthPixels() {
    return 9;
}

int32_t EntityLeashKnot::getHeightPixels() {
    return 9;
}

float EntityLeashKnot::getEyeHeight() {
    return -0.0625F;
}

bool EntityLeashKnot::isInRangeToRenderDist(double distance) {
    return distance < 1024.0;
}

void EntityLeashKnot::onBroken(Entity *brokenEntity) {
    playSound(SoundEvents::ENTITY_LEASHKNOT_BREAK, 1.0F, 1.0F);
}

bool EntityLeashKnot::writeToNBTOptional(NBTTagCompound *compound) {
    return false;
}

void EntityLeashKnot::writeEntityToNBT(NBTTagCompound *compound) {
}

void EntityLeashKnot::readEntityFromNBT(NBTTagCompound *compound) {
}

bool EntityLeashKnot::processInitialInteract(EntityPlayer *player, EnumHand hand) {
    if (world->isRemote) {
         return true;
      } else {
         bool flag = false;
         double d0 = 7.0;
         auto list = world->getEntitiesWithinAABB<EntityLiving>(AxisAlignedBB(posX - 7.0, posY - 7.0, posZ - 7.0, posX + 7.0, posY + 7.0, posZ + 7.0));
          for(auto entityliving1 : list)
         {         
            if (entityliving1->getLeashed() && entityliving1->getLeashHolder() == player) {
               entityliving1->setLeashHolder(this, true);
               flag = true;
            }
         }

         if (!flag) {
            setDead();
            if (player->capabilities.isCreativeMode) {
               for(auto entityliving1 : list)
               {
                  if (entityliving1->getLeashed() && entityliving1->getLeashHolder() == this) {
                     entityliving1->clearLeashed(true, false);
                  }
               }
            }
         }

         return true;
      }
}

bool EntityLeashKnot::onValidSurface() {
    return Util::instanceof<BlockFence>(world->getBlockState(hangingPosition)->getBlock());
}

EntityLeashKnot * EntityLeashKnot::createKnot(World *worldIn, BlockPos fence) {
    EntityLeashKnot* entityleashknot = new EntityLeashKnot(worldIn, fence);
    worldIn->spawnEntity(entityleashknot);
    entityleashknot->playPlaceSound();
    return entityleashknot;
}

EntityLeashKnot * EntityLeashKnot::getKnotForPosition(World *worldIn, BlockPos pos) {
    auto i = pos.getx();
    auto j = pos.gety();
    auto k = pos.getz();
    auto list = worldIn->getEntitiesWithinAABB<EntityLeashKnot>(AxisAlignedBB(i - 1.0, j - 1.0, k - 1.0, i + 1.0, j + 1.0, k + 1.0));

    const auto ite = list.begin();
    const auto entityleashknot = *ite;
    do {
        if (ite == list.end()) {
            return nullptr;
        }

    } while(!(entityleashknot->getHangingPosition() == pos));

    return entityleashknot;
}

void EntityLeashKnot::playPlaceSound() {
}

void EntityLeashKnot::updateBoundingBox() {
    posX = hangingPosition.getx() + 0.5;
    posY = hangingPosition.gety() + 0.5;
    posZ = hangingPosition.getz() + 0.5;
}
