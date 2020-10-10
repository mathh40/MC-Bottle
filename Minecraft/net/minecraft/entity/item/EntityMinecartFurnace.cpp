#include "EntityMinecartFurnace.h"


#include "DamageSource.h"
#include "datafix/DataFixer.h"

DataParameter EntityMinecartFurnace::POWERED = EntityDataManager::createKey(EntityMinecartFurnace.class, DataSerializers::BOOLEAN);

EntityMinecartFurnace::EntityMinecartFurnace(World *worldIn)
    :EntityMinecart(worldIn){
}

EntityMinecartFurnace::EntityMinecartFurnace(World *worldIn, double x, double y, double z)
    :EntityMinecart(worldIn, x, y, z){
}

void EntityMinecartFurnace::registerFixesMinecartFurnace(DataFixer fixer) {
    EntityMinecart::registerFixesMinecart(fixer, EntityMinecartFurnace.class);
}

EntityMinecart::Type EntityMinecartFurnace::getType() {
    return EntityMinecart::Type::FURNACE;
}

void EntityMinecartFurnace::onUpdate() {
    EntityMinecart::onUpdate();
      if (fuel > 0) {
         --fuel;
      }

      if (fuel <= 0) {
         pushX = 0.0;
         pushZ = 0.0;
      }

      setMinecartPowered(fuel > 0);
      if (isMinecartPowered() && rand(4) == 0) {
         world->spawnParticle(EnumParticleTypes::SMOKE_LARGE, posX, posY + 0.8, posZ, 0.0, 0.0, 0.0);
      }
}

void EntityMinecartFurnace::killMinecart(DamageSource::DamageSource source) {
    EntityMinecart::killMinecart(source);
      if (!source.isExplosion() && world->getGameRules().getBoolean("doEntityDrops")) {
         entityDropItem(ItemStack(Blocks::FURNACE, 1), 0.0F);
      }
}

bool EntityMinecartFurnace::processInitialInteract(EntityPlayer *player, EnumHand hand) {
    ItemStack itemstack = player->getHeldItem(hand);
      if (itemstack.getItem() == Items::COAL && fuel + 3600 <= 32000) {
         if (!player->capabilities.isCreativeMode) {
            itemstack.shrink(1);
         }

         fuel += 3600;
      }

      pushX = posX - player->posX;
      pushZ = posZ - player->posZ;
      return true;
}

IBlockState * EntityMinecartFurnace::getDefaultDisplayTile() {
    return (isMinecartPowered() ? Blocks::LIT_FURNACE : Blocks::FURNACE)->getDefaultState()->withProperty(BlockFurnace::FACING, EnumFacing::NORTH);
}

void EntityMinecartFurnace::entityInit() {
    EntityMinecart::entityInit();
    dataManager.registe(POWERED, false);
}

double EntityMinecartFurnace::getMaximumSpeed() const{
    return 0.2;
}

void EntityMinecartFurnace::moveAlongTrack(const BlockPos& pos, IBlockState *state) {
    EntityMinecart::moveAlongTrack(pos, state);
      double d0 = pushX * pushX + pushZ * pushZ;
      if (d0 > 1.0E-4 && motionX * motionX + motionZ * motionZ > 0.001) {
         d0 = (double)MathHelper::sqrt(d0);
         pushX /= d0;
         pushZ /= d0;
         if (pushX * motionX + pushZ * motionZ < 0.0) {
            pushX = 0.0;
            pushZ = 0.0;
         } else {
            double d1 = d0 / getMaximumSpeed();
            pushX *= d1;
            pushZ *= d1;
         }
      }
}

void EntityMinecartFurnace::applyDrag() {
    double d0 = pushX * pushX + pushZ * pushZ;
      if (d0 > 1.0E-4) {
         d0 = (double)MathHelper::sqrt(d0);
         pushX /= d0;
         pushZ /= d0;
         double d1 = 1.0;
         motionX *= 0.800000011920929;
         motionY *= 0.0;
         motionZ *= 0.800000011920929;
         motionX += pushX * 1.0;
         motionZ += pushZ * 1.0;
      } else {
         motionX *= 0.9800000190734863;
         motionY *= 0.0;
         motionZ *= 0.9800000190734863;
      }

      EntityMinecart::applyDrag();
}

void EntityMinecartFurnace::writeEntityToNBT(NBTTagCompound *compound) {
    EntityMinecart::writeEntityToNBT(compound);
    compound->setDouble("PushX", pushX);
    compound->setDouble("PushZ", pushZ);
    compound->setShort("Fuel", fuel);
}

void EntityMinecartFurnace::readEntityFromNBT(NBTTagCompound *compound) {
    EntityMinecart::readEntityFromNBT(compound);
    pushX = compound->getDouble("PushX");
    pushZ = compound->getDouble("PushZ");
    fuel = compound->getShort("Fuel");
}

bool EntityMinecartFurnace::isMinecartPowered() {
    return dataManager.get(POWERED);
}

void EntityMinecartFurnace::setMinecartPowered(bool p_94107_1_) {
    dataManager.set(POWERED, p_94107_1_);
}
