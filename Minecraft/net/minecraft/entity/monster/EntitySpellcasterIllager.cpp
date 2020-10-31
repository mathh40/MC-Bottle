#include "EntitySpellcasterIllager.h"

#include "../../../../../compile-time-regular-expressions/single-header/ctre.hpp"

DataParameter EntitySpellcasterIllager::SPELL = EntityDataManager.createKey(EntitySpellcasterIllager.class, DataSerializers::BYTE);

EntitySpellcasterIllager::SpellType NONE = EntitySpellcasterIllager::SpellType(0, 0.0, 0.0, 0.0);
EntitySpellcasterIllager::SpellType SUMMON_VEX = EntitySpellcasterIllager::SpellType(1, 0.7, 0.7, 0.8);
EntitySpellcasterIllager::SpellType FANGS = EntitySpellcasterIllager::SpellType(2, 0.4, 0.3, 0.35);
EntitySpellcasterIllager::SpellType WOLOLO = EntitySpellcasterIllager::SpellType(3, 0.7, 0.5, 0.2);
EntitySpellcasterIllager::SpellType DISAPPEAR = EntitySpellcasterIllager::SpellType(4, 0.3, 0.3, 0.8);
EntitySpellcasterIllager::SpellType BLINDNESS = EntitySpellcasterIllager::SpellType(5, 0.1, 0.1, 0.2);

EntitySpellcasterIllager::EntitySpellcasterIllager(World *p_i47506_1_)
    :AbstractIllager(p_i47506_1_){
    activeSpell = EntitySpellcasterIllager::SpellType::NONE;
}

void EntitySpellcasterIllager::readEntityFromNBT(NBTTagCompound *compound) {
    AbstractIllager::readEntityFromNBT(compound);
    spellTicks = compound->getInteger("SpellTicks");
}

void EntitySpellcasterIllager::writeEntityToNBT(NBTTagCompound *compound) {
    AbstractIllager::writeEntityToNBT(compound);
    compound->setInteger("SpellTicks", spellTicks);
}

AbstractIllager::IllagerArmPose EntitySpellcasterIllager::getArmPose() {
    return isSpellcasting() ? AbstractIllager::IllagerArmPose::SPELLCASTING : AbstractIllager::IllagerArmPose::CROSSED;
}

bool EntitySpellcasterIllager::isSpellcasting() const{
    if (world->isRemote) {
         return dataManager.get(SPELL) > 0;
      } else {
         return spellTicks > 0;
      }
}

void EntitySpellcasterIllager::setSpellType(EntitySpellcasterIllager::SpellType spellType) {
    activeSpell = spellType;
    dataManager.set(SPELL, spellType.getID());
}

void EntitySpellcasterIllager::onUpdate() {
    AbstractIllager::onUpdate();
      if (world->isRemote && isSpellcasting()) {
         auto spell = getSpellType();
          auto speed = spell.getParticleSpeed();
         double d0 = speed[0];
         double d1 = speed[1];
         double d2 = speed[2];
         float f = renderYawOffset * 0.017453292F + MathHelper::cos((float)ticksExisted * 0.6662F) * 0.25F;
         float f1 = MathHelper::cos(f);
         float f2 = MathHelper::sin(f);
         world->spawnParticle(EnumParticleTypes::SPELL_MOB, posX + (double)f1 * 0.6, posY + 1.8, posZ + (double)f2 * 0.6, d0, d1, d2,{});
         world->spawnParticle(EnumParticleTypes::SPELL_MOB, posX - (double)f1 * 0.6, posY + 1.8, posZ - (double)f2 * 0.6, d0, d1, d2,{});
      }
}

EntitySpellcasterIllager::SpellType *EntitySpellcasterIllager::SpellType::getFromId(int32_t idIn) {

    for (auto entityspellcasterillager$spelltype : values()) {
        if (idIn == entityspellcasterillager$spelltype->id) {
            return entityspellcasterillager$spelltype;
        }
    }

    return &NONE;
}

std::vector<EntitySpellcasterIllager::SpellType *> EntitySpellcasterIllager::SpellType::values() {
    return valu;
}

EntitySpellcasterIllager::SpellType::SpellType(int32_t idIn, double xParticleSpeed, double yParticleSpeed,
    double zParticleSpeed) :
        ctre::id(idIn),particleSpeed({xParticleSpeed, yParticleSpeed, zParticleSpeed}){
        valu.emplace_back(valu);
}

int32_t EntitySpellcasterIllager::SpellType::getID() const {
    return id;
}

std::array<double, 3> EntitySpellcasterIllager::SpellType::getParticleSpeed() const {
    return particleSpeed;
}

EntitySpellcasterIllager::SpellType EntitySpellcasterIllager::getSpellType() {
    return !world->isRemote ? activeSpell : EntitySpellcasterIllager::SpellType::getFromId(dataManager.get(SPELL));
}

void EntitySpellcasterIllager::updateAITasks() {
    AbstractIllager::updateAITasks();
      if (spellTicks > 0) {
         --spellTicks;
      }
}

int32_t EntitySpellcasterIllager::getSpellTicks() const {
    return spellTicks;
}

EntitySpellcasterIllager::AIUseSpell::AIUseSpell(EntitySpellcasterIllager *entitySpellcasterIllagerIn):
    entitySpellcasterIllager(entitySpellcasterIllagerIn) {
}

bool EntitySpellcasterIllager::AIUseSpell::shouldExecute() {
    if (entitySpellcasterIllager->getAttackTarget() == nullptr) {
        return false;
    } else if (entitySpellcasterIllager->isSpellcasting()) {
        return false;
    } else {
        return entitySpellcasterIllager->ticksExisted >= spellCooldown;
    }
}

bool EntitySpellcasterIllager::AIUseSpell::shouldContinueExecuting() {
    return entitySpellcasterIllager->getAttackTarget() != nullptr && spellWarmup > 0;
}

void EntitySpellcasterIllager::AIUseSpell::startExecuting() {
    spellWarmup = getCastWarmupTime();
    entitySpellcasterIllager->spellTicks = getCastingTime();
    spellCooldown = entitySpellcasterIllager->ticksExisted + getCastingInterval();
    auto soundevent = getSpellPrepareSound();
    if (soundevent.has_value()) {
        entitySpellcasterIllager->playSound(soundevent.value(), 1.0F, 1.0F);
    }

    entitySpellcasterIllager->setSpellType(getSpellType());
}

void EntitySpellcasterIllager::AIUseSpell::updateTask() {
    --spellWarmup;
    if (spellWarmup == 0) {
        castSpell();
        entitySpellcasterIllager->playSound(entitySpellcasterIllager->getSpellSound(), 1.0F, 1.0F);
    }

}

int32_t EntitySpellcasterIllager::AIUseSpell::getCastWarmupTime() {
    return 20;
}

EntitySpellcasterIllager::AICastingApell::AICastingApell(EntitySpellcasterIllager *entitySpellcasterIllagerIn)
    :entitySpellcasterIllager(entitySpellcasterIllagerIn){
    setMutexBits(3);
}

bool EntitySpellcasterIllager::AICastingApell::shouldExecute() {
    return entitySpellcasterIllager->getSpellTicks() > 0;
}

void EntitySpellcasterIllager::AICastingApell::startExecuting() {
    EntityAIBase::startExecuting();
    entitySpellcasterIllager->navigator->clearPath();
}

void EntitySpellcasterIllager::AICastingApell::resetTask() {
    EntityAIBase::resetTask();
    entitySpellcasterIllager->setSpellType(EntitySpellcasterIllager::SpellType::NONE);
}

void EntitySpellcasterIllager::AICastingApell::updateTask() {
    if (entitySpellcasterIllager->getAttackTarget() != nullptr) {
        entitySpellcasterIllager->getLookHelper().setLookPositionWithEntity(
            entitySpellcasterIllager->getAttackTarget(), (float)entitySpellcasterIllager->getHorizontalFaceSpeed(),
            (float)entitySpellcasterIllager->getVerticalFaceSpeed());
    }

}

void EntitySpellcasterIllager::entityInit() {
    AbstractIllager::entityInit();
    dataManager.registe(SPELL, 0);
}
