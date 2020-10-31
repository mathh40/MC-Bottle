#include "EntityElderGuardian.h"

#include "../SharedMonsterAttributes.h"
#include "../ai/attributes/IAttributeInstance.h"

EntityElderGuardian::EntityElderGuardian(World *worldIn)
    :EntityGuardian(worldIn){
    setSize(width * 2.35F, height * 2.35F);
      enablePersistence();
      if (wander != nullptr) {
         wander->setExecutionChance(400);
      }
}

void EntityElderGuardian::registerFixesElderGuardian(DataFixer fixer) {
    EntityLiving::registerFixesMob(fixer, EntityElderGuardian.class);
}

int32_t EntityElderGuardian::getAttackDuration() {
    return 60;
}

void EntityElderGuardian::setGhost() {
    clientSideSpikesAnimation = 1.0F;
    clientSideSpikesAnimationO = clientSideSpikesAnimation;
}

void EntityElderGuardian::applyEntityAttributes() {
    EntityGuardian::applyEntityAttributes();
      getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED.get())->setBaseValue(0.30000001192092896);
      getEntityAttribute(SharedMonsterAttributes::ATTACK_DAMAGE.get())->setBaseValue(8.0);
      getEntityAttribute(SharedMonsterAttributes::MAX_HEALTH.get())->setBaseValue(80.0);
}

std::optional<ResourceLocation> EntityElderGuardian::getLootTable() {
    return LootTableList::ENTITIES_ELDER_GUARDIAN;
}

SoundEvent EntityElderGuardian::getAmbientSound() {
    return isInWater() ? SoundEvents::ENTITY_ELDER_GUARDIAN_AMBIENT : SoundEvents::ENTITY_ELDERGUARDIAN_AMBIENTLAND;
}

SoundEvent EntityElderGuardian::getHurtSound(DamageSource::DamageSource damageSourceIn) {
    return isInWater() ? SoundEvents::ENTITY_ELDER_GUARDIAN_HURT : SoundEvents::ENTITY_ELDER_GUARDIAN_HURT_LAND;
}

SoundEvent EntityElderGuardian::getDeathSound() {
    return isInWater() ? SoundEvents::ENTITY_ELDER_GUARDIAN_DEATH : SoundEvents::ENTITY_ELDER_GUARDIAN_DEATH_LAND;
}

SoundEvent EntityElderGuardian::getFlopSound() {
    return SoundEvents::ENTITY_ELDER_GUARDIAN_FLOP;
}

void EntityElderGuardian::updateAITasks() {
    EntityGuardian::updateAITasks();
      int i = true;
      if ((ticksExisted + getEntityId()) % 1200 == 0) {
         Potion potion = MobEffects::MINING_FATIGUE;
         auto list = world->getPlayers(EntityPlayerMP.class, [](EntityPlayerMP* p_apply_1_) {
               return EntityElderGuardian::getDistanceSq(p_apply_1_) < 2500.0 && p_apply_1_->interactionManager.survivalOrAdventure();
         });
         int j = true;
         int k = true;
         int l = true;
         Iterator var7 = list.iterator();

         label28:
         while(true) {
            EntityPlayerMP* entityplayermp;
            do {
               if (!var7.hasNext()) {
                  break label28;
               }

               entityplayermp = (EntityPlayerMP)var7.next();
            } while(entityplayermp->isPotionActive(potion) && entityplayermp->getActivePotionEffect(potion).getAmplifier() >= 2 && entityplayermp->getActivePotionEffect(potion).getDuration() >= 1200);

            entityplayermp->connection.sendPacket(new SPacketChangeGameState(10, 0.0F));
            entityplayermp->addPotionEffect(PotionEffect(potion, 6000, 2));
         }
      }

      if (!hasHome()) {
         setHomePosAndDistance(BlockPos(this), 16);
      }
}
