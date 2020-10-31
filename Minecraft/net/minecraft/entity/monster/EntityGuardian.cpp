#include "EntityGuardian.h"

#include "../SharedMonsterAttributes.h"
#include "../../pathfinding/PathNavigateSwimmer.h"
#include "../ai/EntityAILookIdle.h"
#include "../ai/EntityAIMoveTowardsRestriction.h"
#include "../ai/EntityAINearestAttackableTarget.h"
#include "../ai/EntityAIWatchClosest.h"
#include "../ai/attributes/IAttributeInstance.h"
#include "datafix/DataFixer.h"

DataParameter EntityGuardian::MOVING = EntityDataManager.createKey(EntityGuardian.class, DataSerializers::BOOLEAN);
DataParameter EntityGuardian::TARGET_ENTITY = EntityDataManager.createKey(EntityGuardian.class, DataSerializers::VARINT);

class GuardianTargetSelector
{
private:
    EntityGuardian* parentEntity;

public:
    GuardianTargetSelector(EntityGuardian *guardian);


    bool operator()(EntityLivingBase *p_apply_1_) const;
};

GuardianTargetSelector::GuardianTargetSelector(EntityGuardian *guardian):
    parentEntity(guardian) {
}

bool GuardianTargetSelector::operator()(EntityLivingBase *p_apply_1_) const {
    return (Util::instanceof<EntityPlayer>(p_apply_1_) || Util::instanceof<EntitySquid>(p_apply_1_)) && p_apply_1_->
           getDistanceSq(parentEntity) > 9.0;
}



EntityGuardian::EntityGuardian(World *worldIn)
    :EntityMob(worldIn){
    experienceValue = 10;
    setSize(0.85F, 0.85F);
    moveHelper = EntityGuardian::GuardianMoveHelper(this);
    clientSideTailAnimation = MathHelper::nextFloat(rand);
    clientSideTailAnimationO = clientSideTailAnimation;
}

void EntityGuardian::registerFixesGuardian(DataFixer fixer) {
    EntityLiving::registerFixesMob(fixer, EntityGuardian.class);
}

bool EntityGuardian::isMoving() {
    return dataManager.get(MOVING);
}

int32_t EntityGuardian::getAttackDuration() const{
    return 80;
}

bool EntityGuardian::hasTargetedEntity() {
    return dataManager.get(TARGET_ENTITY) != 0;
}

EntityLivingBase * EntityGuardian::getTargetedEntity() {
    if (!hasTargetedEntity()) {
         return nullptr;
      } else if (world->isRemote) {
         if (targetedEntity != nullptr) {
            return targetedEntity;
         } else {
            Entity* entity = world->getEntityByID(dataManager.get(TARGET_ENTITY));
            if (Util::instanceof<EntityLivingBase>(entity)) {
               targetedEntity = (EntityLivingBase*)entity;
               return targetedEntity;
            } else {
               return nullptr;
            }
         }
      } else {
         return getAttackTarget();
      }
}

void EntityGuardian::notifyDataManagerChange(DataParameter key) {
    EntityMob::notifyDataManagerChange(key);
      if (TARGET_ENTITY == key) {
         clientSideAttackTime = 0;
         targetedEntity = nullptr;
      }
}

int32_t EntityGuardian::getTalkInterval() {
    return 160;
}

float EntityGuardian::getEyeHeight() const {
    return height * 0.5F;
}

float EntityGuardian::getBlockPathWeight(BlockPos pos) {
    return world->getBlockState(pos)->getMaterial() == Material::WATER ? 10.0F + world->getLightBrightness(pos) - 0.5F : EntityMob::getBlockPathWeight(pos);
}

void EntityGuardian::onLivingUpdate() {
    if (world->isRemote) {
         clientSideTailAnimationO = clientSideTailAnimation;
         if (!isInWater()) {
            clientSideTailAnimationSpeed = 2.0F;
            if (motionY > 0.0 && clientSideTouchedGround && !isSilent()) {
               world->playSound(posX, posY, posZ, getFlopSound(), getSoundCategory(), 1.0F, 1.0F, false);
            }

            clientSideTouchedGround = motionY < 0.0 && world->isBlockNormalCube((BlockPos(this)).down(), false);
         } else if (isMoving()) {
            if (clientSideTailAnimationSpeed < 0.5F) {
               clientSideTailAnimationSpeed = 4.0F;
            } else {
               clientSideTailAnimationSpeed += (0.5F - clientSideTailAnimationSpeed) * 0.1F;
            }
         } else {
            clientSideTailAnimationSpeed += (0.125F - clientSideTailAnimationSpeed) * 0.2F;
         }

         clientSideTailAnimation += clientSideTailAnimationSpeed;
         clientSideSpikesAnimationO = clientSideSpikesAnimation;
         if (!isInWater()) {
            clientSideSpikesAnimation = MathHelper::nextFloat(rand);
         } else if (isMoving()) {
            clientSideSpikesAnimation += (0.0F - clientSideSpikesAnimation) * 0.25F;
         } else {
            clientSideSpikesAnimation += (1.0F - clientSideSpikesAnimation) * 0.06F;
         }

         if (isMoving() && isInWater()) {
            Vec3d vec3d = getLook(0.0F);

            for(int i = 0; i < 2; ++i) {
               world->spawnParticle(EnumParticleTypes::WATER_BUBBLE, posX + (MathHelper::nextDouble(rand) - 0.5) * (double)width - vec3d.getx() * 1.5, posY + MathHelper::nextDouble(rand) * (double)height - vec3d.gety() * 1.5, posZ + (MathHelper::nextDouble(rand) - 0.5) * (double)width - vec3d.getz() * 1.5, 0.0, 0.0, 0.0,{});
            }
         }

         if (hasTargetedEntity()) {
            if (clientSideAttackTime < getAttackDuration()) {
               ++clientSideAttackTime;
            }

            EntityLivingBase* entitylivingbase = getTargetedEntity();
            if (entitylivingbase != nullptr) {
               getLookHelper().setLookPositionWithEntity(entitylivingbase, 90.0F, 90.0F);
               getLookHelper().onUpdateLook();
               double d5 = (double)getAttackAnimationScale(0.0F);
               double d0 = entitylivingbase->posX - posX;
               double d1 = entitylivingbase->posY + (double)(entitylivingbase->height * 0.5F) - (posY + (double)getEyeHeight());
               double d2 = entitylivingbase->posZ - posZ;
               double d3 = MathHelper::sqrt(d0 * d0 + d1 * d1 + d2 * d2);
               d0 /= d3;
               d1 /= d3;
               d2 /= d3;
               double d4 = MathHelper::nextDouble(rand);

               while(d4 < d3) {
                  d4 += 1.8 - d5 + MathHelper::nextDouble(rand) * (1.7 - d5);
                  world->spawnParticle(EnumParticleTypes::WATER_BUBBLE, posX + d0 * d4, posY + d1 * d4 + (double)getEyeHeight(), posZ + d2 * d4, 0.0, 0.0, 0.0,{});
               }
            }
         }
      }

      if (inWater) {
         setAir(300);
      } else if (onGround) {
         motionY += 0.5;
         motionX += (double)((MathHelper::nextFloat(rand) * 2.0F - 1.0F) * 0.4F);
         motionZ += (double)((MathHelper::nextFloat(rand) * 2.0F - 1.0F) * 0.4F);
         rotationYaw = MathHelper::nextFloat(rand) * 360.0F;
         onGround = false;
         isAirBorne = true;
      }

      if (hasTargetedEntity()) {
         rotationYaw = rotationYawHead;
      }

      EntityMob::onLivingUpdate();
}

float EntityGuardian::getTailAnimation(float p_175471_1_) const {
    return clientSideTailAnimationO + (clientSideTailAnimation - clientSideTailAnimationO) * p_175471_1_;
}

float EntityGuardian::getSpikesAnimation(float p_175469_1_) const {
    return clientSideSpikesAnimationO + (clientSideSpikesAnimation - clientSideSpikesAnimationO) * p_175469_1_;
}

float EntityGuardian::getAttackAnimationScale(float p_175477_1_) const {
    return (clientSideAttackTime + p_175477_1_) / getAttackDuration();
}

bool EntityGuardian::isNotColliding() {
    return world->checkNoEntityCollision(getEntityBoundingBox(), this) && world->getCollisionBoxes(this, getEntityBoundingBox()).isEmpty();
}

bool EntityGuardian::getCanSpawnHere() {
    return (rand(20) == 0 || !world->canBlockSeeSky(BlockPos(this))) && EntityMob::getCanSpawnHere();
}

bool EntityGuardian::attackEntityFrom(DamageSource::DamageSource source, float amount) {
    if (!isMoving() && !source.isMagicDamage() && 
        Util::instanceof<EntityLivingBase>(source.getImmediateSource())) {
         EntityLivingBase* entitylivingbase = (EntityLivingBase*)source.getImmediateSource();
         if (!source.isExplosion()) {
            entitylivingbase->attackEntityFrom(DamageSource::causeThornsDamage(this), 2.0F);
         }
      }

      if (wander != nullptr) {
         wander->makeUpdate();
      }

      return EntityMob::attackEntityFrom(source, amount);
}

int32_t EntityGuardian::getVerticalFaceSpeed() {
    return 180;
}

void EntityGuardian::travel(float strafe, float vertical, float forward) {
    if (isServerWorld() && isInWater()) {
         moveRelative(strafe, vertical, forward, 0.1F);
         move(MoverType::SELF, motionX, motionY, motionZ);
         motionX *= 0.8999999761581421;
         motionY *= 0.8999999761581421;
         motionZ *= 0.8999999761581421;
         if (!isMoving() && getAttackTarget() == nullptr) {
            motionY -= 0.005;
         }
      } else {
         EntityMob::travel(strafe, vertical, forward);
      }
}

EntityGuardian::GuardianMoveHelper::GuardianMoveHelper(EntityGuardian *guardian):
    EntityMoveHelper(guardian), entityGuardian(guardian) {
}

void EntityGuardian::GuardianMoveHelper::onUpdateMoveHelper() {
    if (action == EntityMoveHelper::Action::MOVE_TO && !entityGuardian->getNavigator()->noPath()) {
        double d0 = posX - entityGuardian->posX;
        double d1 = posY - entityGuardian->posY;
        double d2 = posZ - entityGuardian->posZ;
        double d3 = (double)MathHelper::sqrt(d0 * d0 + d1 * d1 + d2 * d2);
        d1 /= d3;
        float f = (float)(MathHelper::atan2(d2, d0) * 57.29577951308232) - 90.0F;
        entityGuardian->rotationYaw = limitAngle(entityGuardian->rotationYaw, f, 90.0F);
        entityGuardian->renderYawOffset = entityGuardian->rotationYaw;
        float f1 = (float)(speed * entityGuardian->getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED.get())->
                                                   getAttributeValue());
        entityGuardian->setAIMoveSpeed(
            entityGuardian->getAIMoveSpeed() + (f1 - entityGuardian->getAIMoveSpeed()) * 0.125F);
        double d4 = MathHelper::sin((double)(entityGuardian->ticksExisted + entityGuardian->getEntityId()) * 0.5) *
                    0.05;
        double d5 = MathHelper::cos((double)(entityGuardian->rotationYaw * 0.017453292F));
        double d6 = MathHelper::sin((double)(entityGuardian->rotationYaw * 0.017453292F));
        entityGuardian->motionX += d4 * d5;
        entityGuardian->motionZ += d4 * d6;
        d4 = MathHelper::sin((double)(entityGuardian->ticksExisted + entityGuardian->getEntityId()) * 0.75) * 0.05;
        entityGuardian->motionY += d4 * (d6 + d5) * 0.25;
        entityGuardian->motionY += (double)entityGuardian->getAIMoveSpeed() * d1 * 0.1;
        EntityLookHelper entitylookhelper = entityGuardian->getLookHelper();
        double d7 = entityGuardian->posX + d0 / d3 * 2.0;
        double d8 = (double)entityGuardian->getEyeHeight() + entityGuardian->posY + d1 / d3;
        double d9 = entityGuardian->posZ + d2 / d3 * 2.0;
        double d10 = entitylookhelper.getLookPosX();
        double d11 = entitylookhelper.getLookPosY();
        double d12 = entitylookhelper.getLookPosZ();
        if (!entitylookhelper.getIsLooking()) {
            d10 = d7;
            d11 = d8;
            d12 = d9;
        }

        entityGuardian->getLookHelper().setLookPosition(d10 + (d7 - d10) * 0.125, d11 + (d8 - d11) * 0.125,
                                                        d12 + (d9 - d12) * 0.125, 10.0F, 40.0F);
        entityGuardian->setMoving(true);
    } else {
        entityGuardian->setAIMoveSpeed(0.0F);
        entityGuardian->setMoving(false);
    }

}

EntityGuardian::AIGuardianAttack::AIGuardianAttack(EntityGuardian *guardian):
    guardian(guardian), isElder(Util::instanceof<EntityElderGuardian>(guardian)) {
    setMutexBits(3);
}

bool EntityGuardian::AIGuardianAttack::shouldExecute() {
    EntityLivingBase *entitylivingbase = guardian->getAttackTarget();
    return entitylivingbase != nullptr && entitylivingbase->isEntityAlive();
}

bool EntityGuardian::AIGuardianAttack::shouldContinueExecuting() {
    return EntityAIBase::shouldContinueExecuting() && (
               isElder || guardian->getDistanceSq(guardian->getAttackTarget()) > 9.0);
}

void EntityGuardian::AIGuardianAttack::startExecuting() {
    tickCounter = -10;
    guardian->getNavigator()->clearPath();
    guardian->getLookHelper().setLookPositionWithEntity(guardian->getAttackTarget(), 90.0F, 90.0F);
    guardian->isAirBorne = true;
}

void EntityGuardian::AIGuardianAttack::resetTask() {
    guardian->setTargetedEntity(0);
    guardian->setAttackTarget(nullptr);
    guardian->wander->makeUpdate();
}

void EntityGuardian::AIGuardianAttack::updateTask() {
    EntityLivingBase *entitylivingbase = guardian->getAttackTarget();
    guardian->getNavigator()->clearPath();
    guardian->getLookHelper().setLookPositionWithEntity(entitylivingbase, 90.0F, 90.0F);
    if (!guardian->canEntityBeSeen(entitylivingbase)) {
        guardian->setAttackTarget(nullptr);
    } else {
        ++tickCounter;
        if (tickCounter == 0) {
            guardian->setTargetedEntity(guardian->getAttackTarget()->getEntityId());
            guardian->world->setEntityState(guardian, std::byte{21});
        } else if (tickCounter >= guardian->getAttackDuration()) {
            float f = 1.0F;
            if (guardian->world->getDifficulty() == EnumDifficulty::HARD) {
                f += 2.0F;
            }

            if (isElder) {
                f += 2.0F;
            }

            entitylivingbase->attackEntityFrom(DamageSource::causeIndirectMagicDamage(guardian, guardian), f);
            entitylivingbase->attackEntityFrom(DamageSource::causeMobDamage(guardian),
                                               (float)guardian->getEntityAttribute(
                                                   SharedMonsterAttributes::ATTACK_DAMAGE.get())->getAttributeValue());
            guardian->setAttackTarget(nullptr);
        }

        EntityAIBase::updateTask();
    }

}

void EntityGuardian::initEntityAI() {
    EntityAIMoveTowardsRestriction entityaimovetowardsrestriction = new EntityAIMoveTowardsRestriction(this, 1.0);
    wander = new EntityAIWander(this, 1.0, 80);
    tasks.addTask(4, new EntityGuardian::AIGuardianAttack(this));
    tasks.addTask(5, entityaimovetowardsrestriction);
    tasks.addTask(7, wander);
    tasks.addTask(8, new EntityAIWatchClosest(this, EntityPlayer.class, 8.0F));
    tasks.addTask(8, new EntityAIWatchClosest(this, EntityGuardian.class, 12.0F, 0.01F));
    tasks.addTask(9, new EntityAILookIdle(this));
    wander->setMutexBits(3);
    entityaimovetowardsrestriction.setMutexBits(3);
    targetTasks.addTask(1, new EntityAINearestAttackableTarget(this, EntityLivingBase.class, 10, true, false, GuardianTargetSelector(this)));
}

void EntityGuardian::applyEntityAttributes() {
    EntityMob::applyEntityAttributes();
    getEntityAttribute(SharedMonsterAttributes::ATTACK_DAMAGE.get())->setBaseValue(6.0);
    getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED.get())->setBaseValue(0.5);
    getEntityAttribute(SharedMonsterAttributes::FOLLOW_RANGE.get())->setBaseValue(16.0);
    getEntityAttribute(SharedMonsterAttributes::MAX_HEALTH.get())->setBaseValue(30.0);
}

PathNavigate * EntityGuardian::createNavigator(World *worldIn) {
    return new PathNavigateSwimmer(this, worldIn);
}

void EntityGuardian::entityInit() {
    EntityMob::entityInit();
    dataManager.registe(MOVING, false);
    dataManager.registe(TARGET_ENTITY, 0);
}

SoundEvent EntityGuardian::getAmbientSound() {
    return isInWater() ? SoundEvents::ENTITY_GUARDIAN_AMBIENT : SoundEvents::ENTITY_GUARDIAN_AMBIENT_LAND;
}

SoundEvent EntityGuardian::getHurtSound(DamageSource::DamageSource damageSourceIn) {
    return isInWater() ? SoundEvents::ENTITY_GUARDIAN_HURT : SoundEvents::ENTITY_GUARDIAN_HURT_LAND;
}

SoundEvent EntityGuardian::getDeathSound() {
    return isInWater() ? SoundEvents::ENTITY_GUARDIAN_DEATH : SoundEvents::ENTITY_GUARDIAN_DEATH_LAND;
}

bool EntityGuardian::canTriggerWalking() {
    return false;
}

SoundEvent EntityGuardian::getFlopSound() {
    return SoundEvents::ENTITY_GUARDIAN_FLOP;
}

std::optional<ResourceLocation> EntityGuardian::getLootTable() {
    return LootTableList::ENTITIES_GUARDIAN;
}

bool EntityGuardian::isValidLightLevel() {
    return true;
}

void EntityGuardian::setMoving(bool moving) {
    dataManager.set(MOVING, moving);
}

void EntityGuardian::setTargetedEntity(int32_t entityId) {
    dataManager.set(TARGET_ENTITY, entityId);
}
