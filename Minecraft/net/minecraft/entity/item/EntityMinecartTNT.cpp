#include "EntityMinecartTNT.h"


#include "DamageSource.h"
#include "datafix/DataFixer.h"

EntityMinecartTNT::EntityMinecartTNT(World *worldIn)
    :EntityMinecart(worldIn){

}

EntityMinecartTNT::EntityMinecartTNT(World *worldIn, double x, double y, double z)
    :EntityMinecart(worldIn, x, y, z){
}

void EntityMinecartTNT::registerFixesMinecartTNT(DataFixer fixer) {
    EntityMinecart::registerFixesMinecart(fixer, EntityMinecartTNT.class);
}

EntityMinecart::Type EntityMinecartTNT::getType() {
    return EntityMinecart::Type::TNT;
}

IBlockState * EntityMinecartTNT::getDefaultDisplayTile() {
    return Blocks::TNT->getDefaultState();
}

void EntityMinecartTNT::onUpdate() {
    EntityMinecart::onUpdate();
      if (minecartTNTFuse > 0) {
         --minecartTNTFuse;
         world->spawnParticle(EnumParticleTypes::SMOKE_NORMAL, posX, posY + 0.5, posZ, 0.0, 0.0, 0.0);
      } else if (minecartTNTFuse == 0) {
         explodeCart(motionX * motionX + motionZ * motionZ);
      }

      if (collidedHorizontally) {
         double d0 = motionX * motionX + motionZ * motionZ;
         if (d0 >= 0.009999999776482582) {
            explodeCart(d0);
         }
      }
}

bool EntityMinecartTNT::attackEntityFrom(DamageSource::DamageSource source, float amount) {
    auto entity = source.getImmediateSource();
      if (Util::instanceof<EntityArrow>(entity)) {
         auto entityarrow = (EntityArrow*)entity;
         if (entityarrow->isBurning()) {
            explodeCart(entityarrow->motionX * entityarrow->motionX + entityarrow->motionY * entityarrow->motionY + entityarrow->motionZ * entityarrow->motionZ);
         }
      }

      return EntityMinecart::attackEntityFrom(source, amount);
}

void EntityMinecartTNT::killMinecart(DamageSource::DamageSource source) {
    double d0 = motionX * motionX + motionZ * motionZ;
      if (!source.isFireDamage() && !source.isExplosion() && d0 < 0.009999999776482582) {
         EntityMinecart::killMinecart(source);
         if (!source.isExplosion() && world->getGameRules().getBoolean("doEntityDrops")) {
            entityDropItem(ItemStack(Blocks::TNT, 1), 0.0F);
         }
      } else if (minecartTNTFuse < 0) {
         ignite();
         minecartTNTFuse = rand(20) + rand(20);
      }
}

void EntityMinecartTNT::fall(float distance, float damageMultiplier) {
    if (distance >= 3.0F) {
         float f = distance / 10.0F;
         explodeCart((double)(f * f));
      }

      EntityMinecart::fall(distance, damageMultiplier);
}

void EntityMinecartTNT::onActivatorRailPass(int32_t x, int32_t y, int32_t z, bool receivingPower) {
    if (receivingPower && minecartTNTFuse < 0) {
         ignite();
      }
}

void EntityMinecartTNT::handleStatusUpdate(std::byte id) {
    if (id == std::byte{10}) {
         ignite();
      } else {
         EntityMinecart::handleStatusUpdate(id);
      }
}

void EntityMinecartTNT::ignite() {
    minecartTNTFuse = 80;
      if (!world->isRemote) {
         world->setEntityState(this, std::byte{10});
         if (!isSilent()) {
            world->playSound(nullptr, posX, posY, posZ, SoundEvents::ENTITY_TNT_PRIMED, SoundCategory::BLOCKS, 1.0F, 1.0F);
         }
      }
}

int EntityMinecartTNT::getFuseTicks() const {
    return minecartTNTFuse;
}

bool EntityMinecartTNT::isIgnited() const {
    return minecartTNTFuse > -1;
}

float EntityMinecartTNT::getExplosionResistance(Explosion explosionIn, World *worldIn, BlockPos pos,
    IBlockState *blockStateIn) {
    return isIgnited() && (BlockRailBase::isRailBlock(blockStateIn) || BlockRailBase::isRailBlock(worldIn, pos.up())) ? 0.0F : EntityMinecart::getExplosionResistance(explosionIn, worldIn, pos, blockStateIn);
}

bool EntityMinecartTNT::canExplosionDestroyBlock(Explosion explosionIn, World *worldIn, BlockPos pos,
    IBlockState *blockStateIn, float p_174816_5_) {
    return isIgnited() && (BlockRailBase::isRailBlock(blockStateIn) || BlockRailBase::isRailBlock(worldIn, pos.up())) ? false : EntityMinecart::canExplosionDestroyBlock(explosionIn, worldIn, pos, blockStateIn, p_174816_5_);
}

void EntityMinecartTNT::explodeCart(double p_94103_1_) {
    if (!world->isRemote) {
         double d0 = MathHelper::sqrt(p_94103_1_);
         if (d0 > 5.0) {
            d0 = 5.0;
         }

         world->createExplosion(this, posX, posY, posZ, (float)(4.0 + MathHelper::nextDouble(rand) * 1.5 * d0), true);
         setDead();
      }
}

void EntityMinecartTNT::readEntityFromNBT(NBTTagCompound *compound) {
    EntityMinecart::readEntityFromNBT(compound);
      if (compound->hasKey("TNTFuse", 99)) {
         minecartTNTFuse = compound->getInteger("TNTFuse");
      }
}

void EntityMinecartTNT::writeEntityToNBT(NBTTagCompound *compound) {
    EntityMinecart::writeEntityToNBT(compound);
    compound->setInteger("TNTFuse", minecartTNTFuse);
}
