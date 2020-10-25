#include "EntityWither.h"
#include "../SharedMonsterAttributes.h"
#include "../../pathfinding/PathNavigateGround.h"
#include "../ai/EntityAIAttackRanged.h"
#include "../ai/EntityAIHurtByTarget.h"
#include "../ai/EntityAILookIdle.h"
#include "../ai/EntityAINearestAttackableTarget.h"
#include "../ai/EntityAISwimming.h"
#include "../ai/EntityAIWanderAvoidWater.h"
#include "../ai/EntityAIWatchClosest.h"
#include "../ai/attributes/IAttributeInstance.h"
#include "datafix/DataFixer.h"

DataParameter EntityWither::HEAD_TARGETS = {FIRST_HEAD_TARGET, SECOND_HEAD_TARGET, THIRD_HEAD_TARGET};
DataParameter EntityWither::FIRST_HEAD_TARGET = EntityDataManager::createKey(EntityWither.class, DataSerializers.VARINT);
DataParameter EntityWither::SECOND_HEAD_TARGET = EntityDataManager::createKey(EntityWither.class, DataSerializers.VARINT);
DataParameter EntityWither::THIRD_HEAD_TARGET = EntityDataManager::createKey(EntityWither.class, DataSerializers.VARINT);
std::array<DataParameter,3> EntityWither::HEAD_TARGETS = {FIRST_HEAD_TARGET, SECOND_HEAD_TARGET, THIRD_HEAD_TARGET};
DataParameter EntityWither::INVULNERABILITY_TIME = EntityDataManager.createKey(EntityWither.class, DataSerializers.VARINT);

AIDoNothing::AIDoNothing() {
    setMutexBits(7);
}

bool AIDoNothing::shouldExecute() {
    return witherentity->getInvulTime() > 0;
}

EntityWither::EntityWither(World *worldIn)
    :EntityMob(worldIn),bossInfo(getDisplayName(), BossInfo::Color::PURPLE, BossInfo::Overlay::PROGRESS){
    bossInfo.setDarkenSky(true);
    setHealth(getMaxHealth());
    setSize(0.9F, 3.5F);
    bisImmuneToFire = true;
    ((PathNavigateGround*)getNavigator())->setCanSwim(true);
    experienceValue = 50;
}

void EntityWither::registerFixesWither(DataFixer fixer) {
    EntityLiving::registerFixesMob(fixer, EntityWither.class);
}

void EntityWither::writeEntityToNBT(NBTTagCompound *compound) {
    EntityMob::writeEntityToNBT(compound);
    compound->setInteger("Invul", getInvulTime());
}

void EntityWither::readEntityFromNBT(NBTTagCompound *compound) {
    EntityMob::readEntityFromNBT(compound);
    setInvulTime(compound->getInteger("Invul"));
    if (hasCustomName()) {
        bossInfo.setName(getDisplayName());
    }
}

void EntityWither::setCustomNameTag(std::string_view name) {
    EntityMob::setCustomNameTag(name);
    bossInfo.setName(getDisplayName());
}

void EntityWither::onLivingUpdate() {
    motionY *= 0.6000000238418579;
      double d11;
      double d12;
      double d13;
      if (!world->isRemote && getWatchedTargetId(0) > 0) {
         Entity* entity = world->getEntityByID(getWatchedTargetId(0));
         if (entity != nullptr) {
            if (posY < entity->posY || !isArmored() && posY < entity->posY + 5.0) {
               if (motionY < 0.0) {
                  motionY = 0.0;
               }

               motionY += (0.5 - motionY) * 0.6000000238418579;
            }

            double d0 = entity->posX - posX;
            d11 = entity->posZ - posZ;
            d12 = d0 * d0 + d11 * d11;
            if (d12 > 9.0) {
               d13 = (double)MathHelper::sqrt(d12);
               motionX += (d0 / d13 * 0.5 - motionX) * 0.6000000238418579;
               motionZ += (d11 / d13 * 0.5 - motionZ) * 0.6000000238418579;
            }
         }
      }

      if (motionX * motionX + motionZ * motionZ > 0.05000000074505806) {
         rotationYaw = (float)MathHelper::atan2(motionZ, motionX) * 57.295776F - 90.0F;
      }

      EntityMob::onLivingUpdate();

      int j;
      for(j = 0; j < 2; ++j) {
         yRotOHeads[j] = yRotationHeads[j];
         xRotOHeads[j] = xRotationHeads[j];
      }

      int i1;
      for(j = 0; j < 2; ++j) {
         i1 = getWatchedTargetId(j + 1);
         Entity* entity1 = nullptr;
         if (i1 > 0) {
            entity1 = world->getEntityByID(i1);
         }

         if (entity1 != nullptr) {
            d11 = getHeadX(j + 1);
            d12 = getHeadY(j + 1);
            d13 = getHeadZ(j + 1);
            double d6 = entity1->posX - d11;
            double d7 = entity1->posY + (double)entity1->getEyeHeight() - d12;
            double d8 = entity1->posZ - d13;
            double d9 = (double)MathHelper::sqrt(d6 * d6 + d8 * d8);
            float f = (float)(MathHelper::atan2(d8, d6) * 57.29577951308232) - 90.0F;
            float f1 = (float)(-(MathHelper::atan2(d7, d9) * 57.29577951308232));
            xRotationHeads[j] = rotlerp(xRotationHeads[j], f1, 40.0F);
            yRotationHeads[j] = rotlerp(yRotationHeads[j], f, 10.0F);
         } else {
            yRotationHeads[j] = rotlerp(yRotationHeads[j], renderYawOffset, 10.0F);
         }
      }

      bool flag = isArmored();

      for(i1 = 0; i1 < 3; ++i1) {
         double d10 = getHeadX(i1);
         double d2 = getHeadY(i1);
         double d4 = getHeadZ(i1);
         world->spawnParticle(EnumParticleTypes::SMOKE_NORMAL, d10 + MathHelper::nextGaussian<double>(rand) * 0.30000001192092896, d2 + MathHelper::nextGaussian<double>(rand) * 0.30000001192092896, d4 + MathHelper::nextGaussian<double>(rand) * 0.30000001192092896, 0.0, 0.0, 0.0,{});
         if (flag && world->rand(4) == 0) {
            world->spawnParticle(EnumParticleTypes::SPELL_MOB, d10 + MathHelper::nextGaussian<double>(rand) * 0.30000001192092896, d2 + MathHelper::nextGaussian<double>(rand) * 0.30000001192092896, d4 + MathHelper::nextGaussian<double>(rand) * 0.30000001192092896, 0.699999988079071, 0.699999988079071, 0.5,{});
         }
      }

      if (getInvulTime() > 0) {
         for(i1 = 0; i1 < 3; ++i1) {
            world->spawnParticle(EnumParticleTypes::SPELL_MOB, posX + MathHelper::nextGaussian<double>(rand), posY + (double)(MathHelper::nextFloat(rand) * 3.3F), posZ + MathHelper::nextGaussian<double>(rand), 0.699999988079071, 0.699999988079071, 0.8999999761581421,{});
         }
      }
}

bool EntityWither::canDestroyBlock(Block *blockIn) {
    return blockIn != Blocks::BEDROCK && blockIn != Blocks::END_PORTAL && blockIn != Blocks::END_PORTAL_FRAME && blockIn != Blocks::COMMAND_BLOCK && blockIn != Blocks::REPEATING_COMMAND_BLOCK && blockIn != Blocks::CHAIN_COMMAND_BLOCK && blockIn != Blocks::BARRIER && blockIn != Blocks::STRUCTURE_BLOCK && blockIn != Blocks::STRUCTURE_VOID && blockIn != Blocks::PISTON_EXTENSION && blockIn != Blocks::END_GATEWAY;
}

void EntityWither::ignite() {
    setInvulTime(220);
    setHealth(getMaxHealth() / 3.0F);
}

void EntityWither::setInWeb() {

}

void EntityWither::addTrackingPlayer(EntityPlayerMP *player) {
    EntityMob::addTrackingPlayer(player);
    bossInfo.addPlayer(player);
}

void EntityWither::removeTrackingPlayer(EntityPlayerMP *player) {
    EntityMob::removeTrackingPlayer(player);
    bossInfo.removePlayer(player);
}

void EntityWither::attackEntityWithRangedAttack(EntityLivingBase *target, float distanceFactor) {
    launchWitherSkullToEntity(0, target);
}

bool EntityWither::attackEntityFrom(DamageSource::DamageSource source, float amount) {
    if (isEntityInvulnerable(source)) {
         return false;
      } else if (source != DamageSource::DROWN && !(Util::instanceof<EntityWither>(source.getTrueSource()))) {
         if (getInvulTime() > 0 && source != DamageSource::OUT_OF_WORLD) {
            return false;
         } else {
            Entity* entity1;
            if (isArmored()) {
               entity1 = source.getImmediateSource();
               if (Util::instanceof<EntityArrow>(entity1)) {
                  return false;
               }
            }

            entity1 = source.getTrueSource();
            if (entity1 != nullptr && !(Util::instanceof<EntityPlayer>(entity1)) && Util::instanceof<EntityLivingBase>(entity1) && ((EntityLivingBase*)entity1)->getCreatureAttribute() == this.getCreatureAttribute()) {
               return false;
            } else {
               if (blockBreakCounter <= 0) {
                  blockBreakCounter = 20;
               }

               for(auto i = 0; i < idleHeadUpdates.size(); ++i) {
                  idleHeadUpdates[i] += 3;
               }

               return EntityMob::attackEntityFrom(source, amount);
            }
         }
      } else {
         return false;
      }
}

int32_t EntityWither::getBrightnessForRender() {
    return 15728880;
}

void EntityWither::fall(float distance, float damageMultiplier) {

}

void EntityWither::addPotionEffect(PotionEffect potioneffectIn) {

}

float EntityWither::getHeadYRotation(int32_t p_82207_1_) {
    return yRotationHeads[p_82207_1_];
}

float EntityWither::getHeadXRotation(int32_t p_82210_1_) {
    return xRotationHeads[p_82210_1_];
}

int32_t EntityWither::getInvulTime() {
    return dataManager.get(INVULNERABILITY_TIME);
}

void EntityWither::setInvulTime(int32_t time) {
    dataManager.set(INVULNERABILITY_TIME, time);
}

int32_t EntityWither::getWatchedTargetId(int32_t head) {
    return dataManager.get(HEAD_TARGETS[head]);
}

void EntityWither::updateWatchedTargetId(int32_t targetOffset, int32_t newId) {
    dataManager.set(HEAD_TARGETS[targetOffset], newId);
}

bool EntityWither::isArmored() {
    return getHealth() <= getMaxHealth() / 2.0F;
}

EnumCreatureAttribute EntityWither::getCreatureAttribute() {
    return EnumCreatureAttribute::UNDEAD;
}

void EntityWither::setSwingingArms(bool swingingArms) {
}

void EntityWither::initEntityAI() {
    tasks.addTask(0, new AIDoNothing());
    tasks.addTask(1, new EntityAISwimming(this));
    tasks.addTask(2, new EntityAIAttackRanged(this, 1.0, 40, 20.0F));
    tasks.addTask(5, new EntityAIWanderAvoidWater(this, 1.0));
    tasks.addTask(6, new EntityAIWatchClosest(this, EntityPlayer.class, 8.0F));
    tasks.addTask(7, new EntityAILookIdle(this));
    targetTasks.addTask(1, new EntityAIHurtByTarget(this, false, new Class[0]));
    targetTasks.addTask(2, new EntityAINearestAttackableTarget(this, EntityLiving.class, 0, false, false, NOT_UNDEAD));
}

void EntityWither::entityInit() {
    EntityMob::entityInit();
    dataManager.registe(FIRST_HEAD_TARGET, 0);
    dataManager.registe(SECOND_HEAD_TARGET, 0);
    dataManager.registe(THIRD_HEAD_TARGET, 0);
    dataManager.registe(INVULNERABILITY_TIME, 0);
}

SoundEvent EntityWither::getAmbientSound() {
    return SoundEvents::ENTITY_WITHER_AMBIENT;
}

SoundEvent EntityWither::getHurtSound(DamageSource::DamageSource damageSourceIn) {
    return SoundEvents::ENTITY_WITHER_HURT;
}

SoundEvent EntityWither::getDeathSound() {
    return SoundEvents::ENTITY_WITHER_DEATH;
}

void EntityWither::updateAITasks() {
    int i;
      if (getInvulTime() > 0) {
         i = getInvulTime() - 1;
         if (i <= 0) {
            world->newExplosion(this, posX, posY + (double)getEyeHeight(), posZ, 7.0F, false, world->getGameRules().getBoolean("mobGriefing"));
            world->playBroadcastSound(1023, BlockPos(this), 0);
         }

         setInvulTime(i);
         if (ticksExisted % 10 == 0) {
            heal(10.0F);
         }
      } else {
         EntityMob::updateAITasks();

         int l1;
         int i2;
         for(i = 1; i < 3; ++i) {
            if (ticksExisted >= nextHeadUpdate[i - 1]) {
               nextHeadUpdate[i - 1] = ticksExisted + 10 + rand(10);
               if (world->getDifficulty() == EnumDifficulty::NORMAL || world->getDifficulty() == EnumDifficulty::HARD) {
                  l1 = i - 1;
                  i2 = idleHeadUpdates[i - 1];
                  idleHeadUpdates[l1] = idleHeadUpdates[i - 1] + 1;
                  if (i2 > 15) {
                     float f = 10.0F;
                     float f1 = 5.0F;
                     double d0 = MathHelper::nextDouble(rand, posX - 10.0, posX + 10.0);
                     double d1 = MathHelper::nextDouble(rand, posY - 5.0, posY + 5.0);
                     double d2 = MathHelper::nextDouble(rand, posZ - 10.0, posZ + 10.0);
                     launchWitherSkullToCoords(i + 1, d0, d1, d2, true);
                     idleHeadUpdates[i - 1] = 0;
                  }
               }

               l1 = getWatchedTargetId(i);
               if (l1 > 0) {
                  Entity* entity = world->getEntityByID(l1);
                  if (entity != nullptr && entity->isEntityAlive() && getDistanceSq(entity) <= 900.0 && canEntityBeSeen(entity)) {
                     if (Util::instanceof<EntityPlayer>(entity) && ((EntityPlayer*)entity)->capabilities.disableDamage) {
                        updateWatchedTargetId(i, 0);
                     } else {
                        launchWitherSkullToEntity(i + 1, (EntityLivingBase*)entity);
                        nextHeadUpdate[i - 1] = ticksExisted + 40 + rand(20);
                        idleHeadUpdates[i - 1] = 0;
                     }
                  } else {
                     updateWatchedTargetId(i, 0);
                  }
               } else {
                  auto list = world->getEntitiesWithinAABB<EntityLivingBase>(getEntityBoundingBox().grow(20.0, 8.0, 20.0), [](const Entity* entity){NOT_UNDEAD(entity) && EntitySelectors::NOT_SPECTATING(entity);});

                  for(int j2 = 0; j2 < 10 && !list.empty(); ++j2) {
                     auto entitylivingbase = list[rand(list.size())];
                     if (entitylivingbase != this && entitylivingbase->isEntityAlive() && canEntityBeSeen(entitylivingbase)) {
                        if (Util::instanceof<EntityPlayer>(entitylivingbase)) {
                           if (!((EntityPlayer*)entitylivingbase)->capabilities.disableDamage) {
                              updateWatchedTargetId(i, entitylivingbase->getEntityId());
                           }
                        } else {
                           updateWatchedTargetId(i, entitylivingbase->getEntityId());
                        }
                        break;
                     }

                     list.erase(entitylivingbase);
                  }
               }
            }
         }

         if (getAttackTarget() != nullptr) {
            updateWatchedTargetId(0, getAttackTarget()->getEntityId());
         } else {
            updateWatchedTargetId(0, 0);
         }

         if (blockBreakCounter > 0) {
            --blockBreakCounter;
            if (blockBreakCounter == 0 && world->getGameRules().getBoolean("mobGriefing")) {
               i = MathHelper::floor(posY);
               l1 = MathHelper::floor(posX);
               i2 = MathHelper::floor(posZ);
               bool flag = false;
               int k2 = -1;

               while(true) {
                  if (k2 > 1) {
                     if (flag) {
                        world->playEvent(nullptr, 1022, new BlockPos(this), 0);
                     }
                     break;
                  }

                  for(int l2 = -1; l2 <= 1; ++l2) {
                     for(int j = 0; j <= 3; ++j) {
                        int i3 = l1 + k2;
                        int k = i + j;
                        int l = i2 + l2;
                        BlockPos blockpos = BlockPos(i3, k, l);
                        IBlockState* iblockstate = world->getBlockState(blockpos);
                        Block* block = iblockstate->getBlock();
                        if (iblockstate->getMaterial() != Material::AIR && canDestroyBlock(block)) {
                           flag = world->destroyBlock(blockpos, true) || flag;
                        }
                     }
                  }

                  ++k2;
               }
            }
         }

         if (ticksExisted % 20 == 0) {
            heal(1.0F);
         }

         bossInfo.setPercent(getHealth() / getMaxHealth());
      }
}

void EntityWither::dropFewItems(bool wasRecentlyHit, int32_t lootingModifier) {
    EntityItem* entityitem = dropItem(Items::NETHER_STAR, 1);
      if (entityitem != nullptr) {
         entityitem->setNoDespawn();
      }
}

void EntityWither::despawnEntity() {
    idleTime = 0;
}

void EntityWither::applyEntityAttributes() {
    EntityMob::applyEntityAttributes();
    getEntityAttribute(SharedMonsterAttributes::MAX_HEALTH.get())->setBaseValue(300.0);
    getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED.get())->setBaseValue(0.6000000238418579);
    getEntityAttribute(SharedMonsterAttributes::FOLLOW_RANGE.get())->setBaseValue(40.0);
    getEntityAttribute(SharedMonsterAttributes::ARMOR.get())->setBaseValue(4.0);
}

bool EntityWither::canBeRidden(Entity *entityIn) {
    return false;
}

bool EntityWither::isNonBoss() {
    return false;
}

double EntityWither::getHeadX(int32_t p_82214_1_) const {
    if (p_82214_1_ <= 0) {
         return posX;
      } else {
         float f = (renderYawOffset + (float)(180 * (p_82214_1_ - 1))) * 0.017453292F;
         float f1 = MathHelper::cos(f);
         return posX + (double)f1 * 1.3;
      }
}

double EntityWither::getHeadY(int32_t p_82208_1_) const {
    return p_82208_1_ <= 0 ? posY + 3.0 : posY + 2.2;
}

double EntityWither::getHeadZ(int32_t p_82213_1_) const {
    if (p_82213_1_ <= 0) {
         return posZ;
      } else {
         float f = (renderYawOffset + (float)(180 * (p_82213_1_ - 1))) * 0.017453292F;
         float f1 = MathHelper::sin(f);
         return posZ + (double)f1 * 1.3;
      }
}

float EntityWither::rotlerp(float p_82204_1_, float p_82204_2_, float p_82204_3_) const {
    float f = MathHelper::wrapDegrees(p_82204_2_ - p_82204_1_);
      if (f > p_82204_3_) {
         f = p_82204_3_;
      }

      if (f < -p_82204_3_) {
         f = -p_82204_3_;
      }

      return p_82204_1_ + f;
}

void EntityWither::launchWitherSkullToEntity(int32_t p_82216_1_, EntityLivingBase *p_82216_2_) {
    launchWitherSkullToCoords(p_82216_1_, p_82216_2_->posX, p_82216_2_->posY + (double)p_82216_2_->getEyeHeight() * 0.5, p_82216_2_->posZ, p_82216_1_ == 0 && MathHelper::nextFloat(rand) < 0.001F);
}

void EntityWither::launchWitherSkullToCoords(int32_t p_82209_1_, double x, double y, double z, bool invulnerable) {
    world->playEvent(nullptr, 1024, BlockPos(this), 0);
    double d0 = getHeadX(p_82209_1_);
    double d1 = getHeadY(p_82209_1_);
    double d2 = getHeadZ(p_82209_1_);
    double d3 = x - d0;
    double d4 = y - d1;
    double d5 = z - d2;
    EntityWitherSkull* entitywitherskull = new EntityWitherSkull(world, this, d3, d4, d5);
    if (invulnerable) {
     entitywitherskull->setInvulnerable(true);
    }

    entitywitherskull->posY = d1;
    entitywitherskull->posX = d0;
    entitywitherskull->posZ = d2;
    world->spawnEntity(entitywitherskull);
}
