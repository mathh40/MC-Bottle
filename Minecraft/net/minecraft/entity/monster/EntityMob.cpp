#include "EntityMob.h"

#include "../SharedMonsterAttributes.h"
#include "../../item/ItemAxe.h"
#include "../ai/attributes/AbstractAttributeMap.h"
#include "../ai/attributes/IAttributeInstance.h"

EntityMob::EntityMob(World *worldIn)
    :EntityCreature(worldIn){
    experienceValue = 5;
}

SoundCategory EntityMob::getSoundCategory() {
    return SoundCategory::HOSTILE;
}

void EntityMob::onLivingUpdate() {
    updateArmSwingProgress();
      float f = getBrightness();
      if (f > 0.5F) {
         idleTime += 2;
      }

      EntityCreature::onLivingUpdate();
}

void EntityMob::onUpdate() {
    EntityCreature::onUpdate();
      if (!world->isRemote && world->getDifficulty() == EnumDifficulty::PEACEFUL) {
         setDead();
      }
}

bool EntityMob::attackEntityFrom(DamageSource::DamageSource source, float amount) {
    return isEntityInvulnerable(source) ? false : EntityCreature::attackEntityFrom(source, amount);
}

bool EntityMob::attackEntityAsMob(Entity *entityIn) {
    float f = (float)getEntityAttribute(SharedMonsterAttributes::ATTACK_DAMAGE.get())->getAttributeValue();
      int32_t i = 0;
      if (Util::instanceof<EntityLivingBase>(entityIn)) {
         f += EnchantmentHelper::getModifierForCreature(getHeldItemMainhand(), ((EntityLivingBase*)entityIn)->getCreatureAttribute());
         i += EnchantmentHelper::getKnockbackModifier(this);
      }

      bool flag = entityIn->attackEntityFrom(DamageSource::DamageSource::causeMobDamage(this), f);
      if (flag) {
         if (i > 0 && Util::instanceof<EntityLivingBase>(entityIn)) {
            ((EntityLivingBase*)entityIn)->knockBack(this, (float)i * 0.5F, (double)MathHelper::sin(rotationYaw * 0.017453292F), (double)(-MathHelper::cos(rotationYaw * 0.017453292F)));
            motionX *= 0.6;
            motionZ *= 0.6;
         }

         int32_t j = EnchantmentHelper::getFireAspectModifier(this);
         if (j > 0) {
            entityIn->setFire(j * 4);
         }

         if (Util::instanceof<EntityPlayer>(entityIn)) {
            EntityPlayer* entityplayer = (EntityPlayer*)entityIn;
            ItemStack itemstack = getHeldItemMainhand();
            ItemStack itemstack1 = entityplayer->isHandActive() ? entityplayer->getActiveItemStack() : ItemStack::EMPTY;
            if (!itemstack.isEmpty() && !itemstack1.isEmpty() && Util::instanceof<ItemAxe>(itemstack.getItem()) && itemstack1.getItem() == Items::SHIELD) {
               float f1 = 0.25F + (float)EnchantmentHelper::getEfficiencyModifier(this) * 0.05F;
               if (MathHelper::nextFloat(rand) < f1) {
                  entityplayer->getCooldownTracker().setCooldown(Items::SHIELD, 100);
                  world->setEntityState(entityplayer, std::byte{30});
               }
            }
         }

         applyEnchantments(this, entityIn);
      }

      return flag;
}

float EntityMob::getBlockPathWeight(BlockPos pos) {
    return 0.5F - world->getLightBrightness(pos);
}

bool EntityMob::getCanSpawnHere() {
    return world->getDifficulty() != EnumDifficulty::PEACEFUL && isValidLightLevel() && EntityCreature::getCanSpawnHere();
}

bool EntityMob::isPreventingPlayerRest(EntityPlayer *playerIn) {
    return true;
}

bool EntityMob::isValidLightLevel() {
    BlockPos blockpos = BlockPos(posX, getEntityBoundingBox().getminY(), posZ);
      if (world->getLightFor(EnumSkyBlock::SKY, blockpos) > rand(32)) {
         return false;
      } else {
         auto i = world->getLightFromNeighbors(blockpos);
         if (world->isThundering()) {
            auto j = world->getSkylightSubtracted();
            world->setSkylightSubtracted(10);
            i = world->getLightFromNeighbors(blockpos);
            world->setSkylightSubtracted(j);
         }

         return i <= rand(8);
      }
}

void EntityMob::applyEntityAttributes() {
    EntityCreature::applyEntityAttributes();
    getAttributeMap()->registerAttribute(SharedMonsterAttributes::ATTACK_DAMAGE.get());
}

bool EntityMob::canDropLoot() {
    return true;
}

SoundEvent EntityMob::getSwimSound() {
    return SoundEvents::ENTITY_HOSTILE_SWIM;
}

SoundEvent EntityMob::getSplashSound() {
    return SoundEvents::ENTITY_HOSTILE_SPLASH;
}

SoundEvent EntityMob::getHurtSound(DamageSource::DamageSource damageSourceIn) {
    return SoundEvents::ENTITY_HOSTILE_HURT;
}

SoundEvent EntityMob::getDeathSound() {
    return SoundEvents::ENTITY_HOSTILE_DEATH;
}

SoundEvent EntityMob::getFallSound(int32_t heightIn) {
    return heightIn > 4 ? SoundEvents::ENTITY_HOSTILE_BIG_FALL : SoundEvents::ENTITY_HOSTILE_SMALL_FALL;
}
