#include "EntityMinecart.h"

#include "DamageSource.h"
#include "WorldProvider.h"
#include "WorldServer.h"
#include "../EntityLivingBase.h"
#include "../../profiler/Profiler.h"

EntityMinecart::Type EntityMinecart::Type::RIDEABLE = EntityMinecart::Type(0, "MinecartRideable");
EntityMinecart::Type EntityMinecart::Type::CHEST = EntityMinecart::Type(1, "MinecartChest");
EntityMinecart::Type EntityMinecart::Type::FURNACE = EntityMinecart::Type(2, "MinecartFurnace");
EntityMinecart::Type EntityMinecart::Type::TNT = EntityMinecart::Type(3, "MinecartTNT");
EntityMinecart::Type EntityMinecart::Type::SPAWNER = EntityMinecart::Type(4, "MinecartSpawner");
EntityMinecart::Type EntityMinecart::Type::HOPPER = EntityMinecart::Type(5, "MinecartHopper");
EntityMinecart::Type EntityMinecart::Type::COMMAND_BLOCK = EntityMinecart::Type(6, "MinecartCommandBlock");

DataParameter EntityMinecart::ROLLING_AMPLITUDE = EntityDataManager::createKey(EntityMinecart.class, DataSerializers::VARINT);
DataParameter EntityMinecart::ROLLING_DIRECTION = EntityDataManager::createKey(EntityMinecart.class, DataSerializers::VARINT);
DataParameter EntityMinecart::DAMAGE = EntityDataManager::createKey(EntityMinecart.class, DataSerializers::FLOAT);
DataParameter EntityMinecart::DISPLAY_TILE = EntityDataManager::createKey(EntityMinecart.class, DataSerializers::VARINT);
DataParameter EntityMinecart::DISPLAY_TILE_OFFSET = EntityDataManager::createKey(EntityMinecart.class, DataSerializers::VARINT);
DataParameter EntityMinecart::SHOW_BLOCK = EntityDataManager::createKey(EntityMinecart.class, DataSerializers::BOOLEAN);

EntityMinecart::Type::Type(int32_t idIn, std::string nameIn) {
    id = idIn;
    name = nameIn;
    BY_ID.emplace(idIn, this);
}

int32_t EntityMinecart::Type::getId() const {
    return id;
}

std::string EntityMinecart::Type::getName() const {
    return name;
}

EntityMinecart::Type *EntityMinecart::Type::getById(int32_t idIn) {
    const auto entityminecart$type = BY_ID[idIn];
    return entityminecart$type == nullptr ? &RIDEABLE : entityminecart$type;
}

EntityMinecart::EntityMinecart(World *worldIn)
    :Entity(worldIn){
    preventEntitySpawning = true;
    setSize(0.98F, 0.7F);
}

EntityMinecart::EntityMinecart(World *worldIn, double x, double y, double z)
    :EntityMinecart(worldIn){
    setPosition(x, y, z);
    motionX = 0.0;
    motionY = 0.0;
    motionZ = 0.0;
    prevPosX = x;
    prevPosY = y;
    prevPosZ = z;
}

std::unique_ptr<EntityMinecart> EntityMinecart::create(World *worldIn, double x, double y, double z,
                                                       EntityMinecart::Type typeIn) {
    switch(typeIn) {
    case Type::CHEST:
         return std::make_unique<EntityMinecartChest>(worldIn, x, y, z);
      case Type::FURNACE:
         return std::make_unique< EntityMinecartFurnace>(worldIn, x, y, z);
      case Type::TNT:
         return std::make_unique< EntityMinecartTNT>(worldIn, x, y, z);
      case Type::SPAWNER:
         return std::make_unique< EntityMinecartMobSpawner>(worldIn, x, y, z);
      case Type::HOPPER:
         return std::make_unique< EntityMinecartHopper>(worldIn, x, y, z);
      case Type::COMMAND_BLOCK:
         return std::make_unique< EntityMinecartCommandBlock>(worldIn, x, y, z);
      default:
         return std::make_unique< EntityMinecartEmpty>(worldIn, x, y, z);
      }
}

std::optional<AxisAlignedBB> EntityMinecart::getCollisionBox(Entity *entityIn) {
    return entityIn->canBePushed() ? entityIn->getEntityBoundingBox() : std::nullopt;
}

std::optional<AxisAlignedBB> EntityMinecart::getCollisionBoundingBox() {
    return std::nullopt;
}

bool EntityMinecart::canBePushed() {
    return true;
}

double EntityMinecart::getMountedYOffset() const{
    return 0.0;
}

bool EntityMinecart::attackEntityFrom(DamageSource::DamageSource source, float amount) {
    if (!world->isRemote && !isDead) {
         if (isEntityInvulnerable(source)) {
            return false;
         } else {
            setRollingDirection(-getRollingDirection());
            setRollingAmplitude(10);
            markVelocityChanged();
            setDamage(getDamage() + amount * 10.0F);
            bool flag = Util::instanceof<EntityPlayer>(source.getTrueSource()) && ((EntityPlayer*)source.getTrueSource())->capabilities.isCreativeMode;
            if (flag || getDamage() > 40.0F) {
               removePassengers();
               if (flag && !hasCustomName()) {
                  setDead();
               } else {
                  killMinecart(source);
               }
            }

            return true;
         }
      } else {
         return true;
      }
}

void EntityMinecart::killMinecart(DamageSource::DamageSource source) {
    setDead();
      if (world->getGameRules().getBoolean("doEntityDrops")) {
         ItemStack itemstack = ItemStack(Items::MINECART, 1);
         if (hasCustomName()) {
            itemstack.setStackDisplayName(getCustomNameTag());
         }

         entityDropItem(itemstack, 0.0F);
      }
}

void EntityMinecart::performHurtAnimation() {
    setRollingDirection(-getRollingDirection());
    setRollingAmplitude(10);
    setDamage(getDamage() + getDamage() * 10.0F);
}

bool EntityMinecart::canBeCollidedWith() {
    return !isDead;
}

EnumFacing EntityMinecart::getAdjustedHorizontalFacing() const {
    return isInReverse ? getHorizontalFacing().getOpposite().rotateY() : getHorizontalFacing().rotateY();
}

void EntityMinecart::onUpdate() {
    if (getRollingAmplitude() > 0) {
         setRollingAmplitude(getRollingAmplitude() - 1);
      }

      if (getDamage() > 0.0F) {
         setDamage(getDamage() - 1.0F);
      }

      if (posY < -64.0) {
         outOfWorld();
      }

      int l;
      if (!world->isRemote && Util::instanceof<WorldServer>(world)) {
         world->profiler.startSection("portal");
         MinecraftServer* minecraftserver = world->getMinecraftServer();
         l = getMaxInPortalTime();
         if (inPortal) {
            if (minecraftserver->getAllowNether()) {
               if (!isRiding() && portalCounter++ >= l) {
                  portalCounter = l;
                  timeUntilPortal = getPortalCooldown();
                  int32_t j;
                  if (world->provider->getDimensionType().getId() == -1) {
                     j = 0;
                  } else {
                     j = -1;
                  }

                  changeDimension(j);
               }

               inPortal = false;
            }
         } else {
            if (portalCounter > 0) {
               portalCounter -= 4;
            }

            if (portalCounter < 0) {
               portalCounter = 0;
            }
         }

         if (timeUntilPortal > 0) {
            --timeUntilPortal;
         }

         world->profiler.endSection();
      }

      if (world->isRemote) {
         if (turnProgress > 0) {
            double d4 = posX + (minecartX - posX) / (double)turnProgress;
            double d5 = posY + (minecartY - posY) / (double)turnProgress;
            double d6 = posZ + (minecartZ - posZ) / (double)turnProgress;
            double d1 = MathHelper::wrapDegrees(minecartYaw - (double)rotationYaw);
            rotationYaw = (float)((double)rotationYaw + d1 / (double)turnProgress);
            rotationPitch = (float)((double)rotationPitch + (minecartPitch - (double)rotationPitch) / (double)turnProgress);
            --turnProgress;
            setPosition(d4, d5, d6);
            setRotation(rotationYaw, rotationPitch);
         } else {
            setPosition(posX, posY, posZ);
            setRotation(rotationYaw, rotationPitch);
         }
      } else {
         prevPosX = posX;
         prevPosY = posY;
         prevPosZ = posZ;
         if (!hasNoGravity()) {
            motionY -= 0.03999999910593033;
         }

         auto k = MathHelper::floor(posX);
         l = MathHelper::floor(posY);
         auto i1 = MathHelper::floor(posZ);
         if (BlockRailBase::isRailBlock(world, BlockPos(k, l - 1, i1))) {
            --l;
         }

         BlockPos blockpos = BlockPos(k, l, i1);
         IBlockState* iblockstate = world->getBlockState(blockpos);
         if (BlockRailBase::isRailBlock(iblockstate)) {
            moveAlongTrack(blockpos, iblockstate);
            if (iblockstate->getBlock() == Blocks::ACTIVATOR_RAIL) {
               onActivatorRailPass(k, l, i1,iblockstate->getValue(BlockRailPowered::POWERED));
            }
         } else {
            moveDerailedMinecart();
         }

         doBlockCollisions();
         rotationPitch = 0.0F;
         double d0 = prevPosX - posX;
         double d2 = prevPosZ - posZ;
         if (d0 * d0 + d2 * d2 > 0.001) {
            rotationYaw = (float)(MathHelper::atan2(d2, d0) * 180.0 / 3.141592653589793);
            if (isInReverse) {
               rotationYaw += 180.0F;
            }
         }

         double d3 = (double)MathHelper::wrapDegrees(rotationYaw - prevRotationYaw);
         if (d3 < -170.0 || d3 >= 170.0) {
            rotationYaw += 180.0F;
            isInReverse = !isInReverse;
         }

         setRotation(rotationYaw, rotationPitch);
         if (getType() == EntityMinecart::Type::RIDEABLE && motionX * motionX + motionZ * motionZ > 0.01) {
            auto list = world->getEntitiesInAABBexcluding(this, getEntityBoundingBox().grow(0.20000000298023224, 0.0, 0.20000000298023224), EntitySelectors::getTeamCollisionPredicate(this));
            for(auto entity1 : list)
               {
                  if (!(Util::instanceof<EntityPlayer>(entity1)) && !(Util::instanceof<EntityIronGolem>(entity1)) && !(Util::instanceof<EntityMinecart>(entity1)) && !isBeingRidden() && !entity1->isRiding()) {
                     entity1->startRiding(this);
                  } else {
                     entity1->applyEntityCollision(this);
                  }
               }
         } else {
            auto var12 = world->getEntitiesWithinAABBExcludingEntity(this, getEntityBoundingBox().grow(0.20000000298023224, 0.0, 0.20000000298023224));

             for(auto entity : var12){
               if (!isPassenger(entity) && entity->canBePushed() && Util::instanceof<EntityMinecart>(entity)) {
                  entity->applyEntityCollision(this);
               }
            }
         }

         handleWaterMovement();
      }
}

void EntityMinecart::onActivatorRailPass(int32_t x, int32_t y, int32_t z, bool receivingPower) {
    posX = x;
    posY = y;
    posZ = z;
    float f = width / 2.0F;
    float f1 = height;
    setEntityBoundingBox(AxisAlignedBB(x - (double)f, y, z - (double)f, x + (double)f, y + (double)f1, z + (double)f));
}

void EntityMinecart::setPosition(double x, double y, double z) {

}

std::optional<Vec3d> EntityMinecart::getPosOffset(double x, double y, double z, double offset) {
    auto i = MathHelper::floor(x);
      auto j = MathHelper::floor(y);
      auto k = MathHelper::floor(z);
      if (BlockRailBase::isRailBlock(world, BlockPos(i, j - 1, k))) {
         --j;
      }

      IBlockState* iblockstate = world->getBlockState(BlockPos(i, j, k));
      if (BlockRailBase::isRailBlock(iblockstate)) {
         BlockRailBase::EnumRailDirection blockrailbase$enumraildirection = (BlockRailBase::EnumRailDirection)iblockstate->getValue(((BlockRailBase*)iblockstate->getBlock())->getShapeProperty());
         y = (double)j;
         if (blockrailbase$enumraildirection.isAscending()) {
            y = (double)(j + 1);
         }

         auto aint = MATRIX[blockrailbase$enumraildirection.getMetadata()];
         double d0 = (double)(aint(1,0) - aint(0,0));
         double d1 = (double)(aint(1,2) - aint(0,2));
         double d2 = MathHelper::sqrt(d0 * d0 + d1 * d1);
         d0 /= d2;
         d1 /= d2;
         x += d0 * offset;
         z += d1 * offset;
         if (aint(0,1) != 0 && MathHelper::floor(x) - i == aint(0,0) && MathHelper::floor(z) - k == aint(0,2)) {
            y += (double)aint(0,1);
         } else if (aint(1,1) != 0 && MathHelper::floor(x) - i == aint(1,0) && MathHelper::floor(z) - k == aint(1,2)) {
            y += (double)aint(1,1);
         }

         return getPos(x, y, z);
      } else {
         return std::nullopt;
      }
}

std::optional<Vec3d> EntityMinecart::getPos(double p_70489_1_, double p_70489_3_, double p_70489_5_) {
    auto i = MathHelper::floor(p_70489_1_);
      auto j = MathHelper::floor(p_70489_3_);
      auto k = MathHelper::floor(p_70489_5_);
      if (BlockRailBase::isRailBlock(world, BlockPos(i, j - 1, k))) {
         --j;
      }

      IBlockState* iblockstate = world->getBlockState(BlockPos(i, j, k));
      if (BlockRailBase::isRailBlock(iblockstate)) {
         BlockRailBase::EnumRailDirection blockrailbase$enumraildirection = (BlockRailBase::EnumRailDirection)iblockstate->getValue(((BlockRailBase*)iblockstate->getBlock())->getShapeProperty());
         auto aint = MATRIX[blockrailbase$enumraildirection.getMetadata()];
         double d0 = (double)i + 0.5 + (double)aint[0][0] * 0.5;
         double d1 = (double)j + 0.0625 + (double)aint[0][1] * 0.5;
         double d2 = (double)k + 0.5 + (double)aint[0][2] * 0.5;
         double d3 = (double)i + 0.5 + (double)aint[1][0] * 0.5;
         double d4 = (double)j + 0.0625 + (double)aint[1][1] * 0.5;
         double d5 = (double)k + 0.5 + (double)aint[1][2] * 0.5;
         double d6 = d3 - d0;
         double d7 = (d4 - d1) * 2.0;
         double d8 = d5 - d2;
         double d9;
         if (d6 == 0.0) {
            d9 = p_70489_5_ - (double)k;
         } else if (d8 == 0.0) {
            d9 = p_70489_1_ - (double)i;
         } else {
            double d10 = p_70489_1_ - d0;
            double d11 = p_70489_5_ - d2;
            d9 = (d10 * d6 + d11 * d8) * 2.0;
         }

         p_70489_1_ = d0 + d6 * d9;
         p_70489_3_ = d1 + d7 * d9;
         p_70489_5_ = d2 + d8 * d9;
         if (d7 < 0.0) {
            ++p_70489_3_;
         }

         if (d7 > 0.0) {
            p_70489_3_ += 0.5;
         }

         return Vec3d(p_70489_1_, p_70489_3_, p_70489_5_);
      } else {
         return std::nullopt;
      }
}

AxisAlignedBB EntityMinecart::getRenderBoundingBox() {
    AxisAlignedBB axisalignedbb = getEntityBoundingBox();
    return hasDisplayTile() ? axisalignedbb.grow((double)MathHelper::abs(getDisplayTileOffset()) / 16.0) : axisalignedbb;
}

void EntityMinecart::applyEntityCollision(Entity *entityIn) {
    if (!world->isRemote && !entityIn->noClip && !noClip && !isPassenger(entityIn)) {
         double d0 = entityIn->posX - posX;
         double d1 = entityIn->posZ - posZ;
         double d2 = d0 * d0 + d1 * d1;
         if (d2 >= 9.999999747378752E-5) {
            d2 = (double)MathHelper::sqrt(d2);
            d0 /= d2;
            d1 /= d2;
            double d3 = 1.0 / d2;
            if (d3 > 1.0) {
               d3 = 1.0;
            }

            d0 *= d3;
            d1 *= d3;
            d0 *= 0.10000000149011612;
            d1 *= 0.10000000149011612;
            d0 *= (double)(1.0F - entityCollisionReduction);
            d1 *= (double)(1.0F - entityCollisionReduction);
            d0 *= 0.5;
            d1 *= 0.5;
            if (Util::instanceof<EntityMinecart>(entityIn)) {
               double d4 = entityIn->posX - posX;
               double d5 = entityIn->posZ - posZ;
               Vec3d vec3d = (Vec3d(d4, 0.0, d5)).normalize();
               Vec3d vec3d1 = (Vec3d((double)MathHelper::cos(rotationYaw * 0.017453292F), 0.0, (double)MathHelper::sin(rotationYaw * 0.017453292F))).normalize();
               double d6 = MathHelper::abs(vec3d.dotProduct(vec3d1));
               if (d6 < 0.800000011920929) {
                  return;
               }

               double d7 = entityIn->motionX + motionX;
               double d8 = entityIn->motionZ + motionZ;
               if (((EntityMinecart*)entityIn)->getType() == EntityMinecart::Type::FURNACE && getType() != EntityMinecart::Type::FURNACE) {
                  motionX *= 0.20000000298023224;
                  motionZ *= 0.20000000298023224;
                  addVelocity(entityIn->motionX - d0, 0.0, entityIn->motionZ - d1);
                  entityIn->motionX *= 0.949999988079071;
                  entityIn->motionZ *= 0.949999988079071;
               } else if (((EntityMinecart*)entityIn)->getType() != EntityMinecart::Type::FURNACE && getType() == EntityMinecart::Type::FURNACE) {
                  entityIn->motionX *= 0.20000000298023224;
                  entityIn->motionZ *= 0.20000000298023224;
                  entityIn->addVelocity(motionX + d0, 0.0, motionZ + d1);
                  motionX *= 0.949999988079071;
                  motionZ *= 0.949999988079071;
               } else {
                  d7 /= 2.0;
                  d8 /= 2.0;
                  motionX *= 0.20000000298023224;
                  motionZ *= 0.20000000298023224;
                  addVelocity(d7 - d0, 0.0, d8 - d1);
                  entityIn->motionX *= 0.20000000298023224;
                  entityIn->motionZ *= 0.20000000298023224;
                  entityIn->addVelocity(d7 + d0, 0.0, d8 + d1);
               }
            } else {
               addVelocity(-d0, 0.0, -d1);
               entityIn->addVelocity(d0 / 4.0, 0.0, d1 / 4.0);
            }
         }
      }
}

void EntityMinecart::setPositionAndRotationDirect(double x, double y, double z, float yaw, float pitch,
    int32_t posRotationIncrements, bool teleport) {
    minecartX = x;
    minecartY = y;
    minecartZ = z;
    minecartYaw = (double)yaw;
    minecartPitch = (double)pitch;
    turnProgress = posRotationIncrements + 2;
    motionX = velocityX;
    motionY = velocityY;
    motionZ = velocityZ;
}

void EntityMinecart::setVelocity(double x, double y, double z) {
    motionX = x;
    motionY = y;
    motionZ = z;
    velocityX = motionX;
    velocityY = motionY;
    velocityZ = motionZ;
}

void EntityMinecart::setDamage(float damage) {
    dataManager.set(DAMAGE, damage);
}

float EntityMinecart::getDamage() {
    return dataManager.get(DAMAGE);
}

void EntityMinecart::setRollingAmplitude(int32_t rollingAmplitude) {
    dataManager.set(ROLLING_AMPLITUDE, rollingAmplitude);
}

int32_t EntityMinecart::getRollingAmplitude() {
    return dataManager.get(ROLLING_AMPLITUDE);
}

void EntityMinecart::setRollingDirection(int32_t rollingDirection) {
    dataManager.set(ROLLING_DIRECTION, rollingDirection);
}

int32_t EntityMinecart::getRollingDirection() {
    return dataManager.get(ROLLING_DIRECTION);
}

IBlockState * EntityMinecart::getDisplayTile() {
    return !hasDisplayTile() ? getDefaultDisplayTile() : Block::getStateById(getDataManager().get(DISPLAY_TILE));
}

IBlockState * EntityMinecart::getDefaultDisplayTile() {
    return Blocks::AIR::getDefaultState();
}

int32_t EntityMinecart::getDisplayTileOffset() {
    return !hasDisplayTile() ? getDefaultDisplayTileOffset() : getDataManager().get(DISPLAY_TILE_OFFSET);
}

int32_t EntityMinecart::getDefaultDisplayTileOffset() const {
    return 6;
}

void EntityMinecart::setDisplayTile(IBlockState *displayTile) {
    getDataManager().set(DISPLAY_TILE, Block::getStateId(displayTile));
    setHasDisplayTile(true);
}

void EntityMinecart::setDisplayTileOffset(int32_t displayTileOffset) {
    getDataManager().set(DISPLAY_TILE_OFFSET, displayTileOffset);
    setHasDisplayTile(true);
}

bool EntityMinecart::hasDisplayTile() {
    return getDataManager().get(SHOW_BLOCK);
}

void EntityMinecart::setHasDisplayTile(bool showBlock) {
    getDataManager().set(SHOW_BLOCK, showBlock);
}

bool EntityMinecart::canTriggerWalking() {
    return false;
}

void EntityMinecart::entityInit() {
    dataManager.registe(ROLLING_AMPLITUDE, 0);
    dataManager.registe(ROLLING_DIRECTION, 1);
    dataManager.registe(DAMAGE, 0.0F);
    dataManager.registe(DISPLAY_TILE, 0);
    dataManager.registe(DISPLAY_TILE_OFFSET, 6);
    dataManager.registe(SHOW_BLOCK, false);
}

double EntityMinecart::getMaximumSpeed() const{
    return 0.4;
}

void EntityMinecart::moveDerailedMinecart() {
    double d0 = getMaximumSpeed();
    motionX = MathHelper::clamp(motionX, -d0, d0);
    motionZ = MathHelper::clamp(motionZ, -d0, d0);
    if (onGround) {
        motionX *= 0.5;
        motionY *= 0.5;
        motionZ *= 0.5;
        }

    move(MoverType::SELF, motionX, motionY, motionZ);
    if (!onGround) {
        motionX *= 0.949999988079071;
        motionY *= 0.949999988079071;
        motionZ *= 0.949999988079071;
    }
}

void EntityMinecart::moveAlongTrack(const BlockPos &pos, IBlockState *state) {
    fallDistance = 0.0F;
      auto vec3d = getPos(posX, posY, posZ);
      posY = (double)pos.gety();
      bool flag = false;
      bool flag1 = false;
      BlockRailBase blockrailbase = (BlockRailBase)state->getBlock();
      if (blockrailbase == Blocks::GOLDEN_RAIL) {
         flag = state->getValue(BlockRailPowered::POWERED);
         flag1 = !flag;
      }

      double d0 = 0.0078125;
      BlockRailBase::EnumRailDirection blockrailbase$enumraildirection = (BlockRailBase::EnumRailDirection)state->getValue(blockrailbase.getShapeProperty());
      switch(blockrailbase$enumraildirection) {
      case ASCENDING_EAST:
         motionX -= 0.0078125;
         ++posY;
         break;
      case ASCENDING_WEST:
         motionX += 0.0078125;
         ++posY;
         break;
      case ASCENDING_NORTH:
         motionZ += 0.0078125;
         ++posY;
         break;
      case ASCENDING_SOUTH:
         motionZ -= 0.0078125;
         ++posY;
      }

      
      multivec<int32_t> aint = MATRIX[blockrailbase$enumraildirection.getMetadata()];
      double d1 = (double)(aint[1][0] - aint[0][0]);
      double d2 = (double)(aint[1][2] - aint[0][2]);
      double d3 = MathHelper::sqrt(d1 * d1 + d2 * d2);
      double d4 = motionX * d1 + motionZ * d2;
      if (d4 < 0.0) {
         d1 = -d1;
         d2 = -d2;
      }

      double d5 = MathHelper::sqrt(motionX * motionX + motionZ * motionZ);
      if (d5 > 2.0) {
         d5 = 2.0;
      }

      motionX = d5 * d1 / d3;
      motionZ = d5 * d2 / d3;
      Entity* entity = getPassengers().empty() ? nullptr : (Entity*)getPassengers()[0];
      double d18;
      double d19;
      double d8;
      double d9;
      if (Util::instanceof<EntityLivingBase>(entity)) {
         d18 = (double)((EntityLivingBase*)entity)->moveForward;
         if (d18 > 0.0) {
            d19 = -MathHelper::sin((double)(entity->rotationYaw * 0.017453292F));
            d8 = MathHelper::cos((double)(entity->rotationYaw * 0.017453292F));
            d9 = motionX * motionX + motionZ * motionZ;
            if (d9 < 0.01) {
               motionX += d19 * 0.1;
               motionZ += d8 * 0.1;
               flag1 = false;
            }
         }
      }

      if (flag1) {
         d18 = MathHelper::sqrt(motionX * motionX + motionZ * motionZ);
         if (d18 < 0.03) {
            motionX *= 0.0;
            motionY *= 0.0;
            motionZ *= 0.0;
         } else {
            motionX *= 0.5;
            motionY *= 0.0;
            motionZ *= 0.5;
         }
      }

      d18 = (double)pos.getx() + 0.5 + (double)aint(0,0) * 0.5;
      d19 = (double)pos.getz() + 0.5 + (double)aint(0,2) * 0.5;
      d8 = (double)pos.getx() + 0.5 + (double)aint(1,0) * 0.5;
      d9 = (double)pos.getz() + 0.5 + (double)aint(1,2) * 0.5;
      d1 = d8 - d18;
      d2 = d9 - d19;
      double d10;
      double d22;
      double d23;
      if (d1 == 0.0) {
         posX = (double)pos.getx() + 0.5;
         d10 = posZ - (double)pos.getz();
      } else if (d2 == 0.0) {
         posZ = (double)pos.getz() + 0.5;
         d10 = posX - (double)pos.getx();
      } else {
         d22 = posX - d18;
         d23 = posZ - d19;
         d10 = (d22 * d1 + d23 * d2) * 2.0;
      }

      posX = d18 + d1 * d10;
      posZ = d19 + d2 * d10;
      setPosition(posX, posY, posZ);
      d22 = motionX;
      d23 = motionZ;
      if (isBeingRidden()) {
         d22 *= 0.75;
         d23 *= 0.75;
      }

      double d13 = getMaximumSpeed();
      d22 = MathHelper::clamp(d22, -d13, d13);
      d23 = MathHelper::clamp(d23, -d13, d13);
      move(MoverType::SELF, d22, 0.0, d23);
      if (aint[0][1] != 0 && MathHelper::floor(posX) - pos.getx() == aint[0][0] && MathHelper::floor(posZ) - pos.getz() == aint[0][2]) {
         setPosition(posX, posY + (double)aint[0][1], posZ);
      } else if (aint[1][1] != 0 && MathHelper::floor(posX) - pos.getx() == aint[1][0] && MathHelper::floor(posZ) - pos.getz() == aint[1][2]) {
         setPosition(posX, posY + (double)aint[1][1], posZ);
      }

      applyDrag();
      auto vec3d1 = getPos(posX, posY, posZ);
      if (vec3d1 != std::nullopt && vec3d != std::nullopt) {
         double d14 = (vec3d->gety() - vec3d1->gety()) * 0.05;
         d5 = MathHelper::sqrt(motionX * motionX + motionZ * motionZ);
         if (d5 > 0.0) {
            motionX = motionX / d5 * (d5 + d14);
            motionZ = motionZ / d5 * (d5 + d14);
         }

         setPosition(posX, vec3d1->gety(), posZ);
      }

      auto j = MathHelper::floor(posX);
      auto i = MathHelper::floor(posZ);
      if (j != pos.getx() || i != pos.getz()) {
         d5 = MathHelper::sqrt(motionX * motionX + motionZ * motionZ);
         motionX = d5 * (double)(j - pos.getx());
         motionZ = d5 * (double)(i - pos.getz());
      }

      if (flag) {
         double d15 = MathHelper::sqrt(motionX * motionX + motionZ * motionZ);
         if (d15 > 0.01) {
            double d16 = 0.06;
            motionX += motionX / d15 * 0.06;
            motionZ += motionZ / d15 * 0.06;
         } else if (blockrailbase$enumraildirection == BlockRailBase::EnumRailDirection::EAST_WEST) {
            if (world->getBlockState(pos.west())->isNormalCube()) {
               motionX = 0.02;
            } else if (world->getBlockState(pos.east())->isNormalCube()) {
               motionX = -0.02;
            }
         } else if (blockrailbase$enumraildirection == BlockRailBase::EnumRailDirection::NORTH_SOUTH) {
            if (world->getBlockState(pos.north())->isNormalCube()) {
               motionZ = 0.02;
            } else if (world->getBlockState(pos.south())->isNormalCube()) {
               motionZ = -0.02;
            }
         }
      }
}

void EntityMinecart::applyDrag() {
    if (isBeingRidden()) {
         motionX *= 0.996999979019165;
         motionY *= 0.0;
         motionZ *= 0.996999979019165;
      } else {
         motionX *= 0.9599999785423279;
         motionY *= 0.0;
         motionZ *= 0.9599999785423279;
      }
}

void EntityMinecart::readEntityFromNBT(NBTTagCompound *compound) {
    if (compound->getBoolean("CustomDisplayTile")) {
         Block* block;
         if (compound->hasKey("DisplayTile", 8)) {
            block = Block::getBlockFromName(compound->getString("DisplayTile"));
         } else {
            block = Block::getBlockById(compound->getInteger("DisplayTile"));
         }

         auto i = compound->getInteger("DisplayData");
         setDisplayTile(block == nullptr ? Blocks::AIR::getDefaultState() : block->getStateFromMeta(i));
         setDisplayTileOffset(compound->getInteger("DisplayOffset"));
      }
}

void EntityMinecart::writeEntityToNBT(NBTTagCompound *compound) {
    if (hasDisplayTile()) {
         compound->setBoolean("CustomDisplayTile", true);
         IBlockState* iblockstate = getDisplayTile();
         auto resourcelocation = Block::REGISTRY.getNameForObject(iblockstate->getBlock());
         compound->setString("DisplayTile", resourcelocation == std::nullopt ? "" : resourcelocation->to_string());
         compound->setInteger("DisplayData", iblockstate->getBlock()->getMetaFromState(iblockstate));
         compound->setInteger("DisplayOffset", getDisplayTileOffset());
      }
}
