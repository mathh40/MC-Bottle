#include "PhaseManager.h"

#include "../../../../../../../spdlog/include/spdlog/spdlog.h"
#include "boss/EntityDragon.h"

std::shared_ptr<spdlog::logger> PhaseManager::LOGGER = spdlog::get("Minecraft")->clone("PhaseManager");

PhaseManager::PhaseManager(EntityDragon *dragonIn)
    :dragon(dragonIn),phases(PhaseList::getTotalPhases()){
    setPhase(PhaseList::HOVER.get());
}

void PhaseManager::setPhase(PhaseList *phaseIn) {
    if (phase == nullptr || phaseIn != phase->getType()) {
         if (phase != nullptr) {
            phase->removeAreaEffect();
         }

         phase = getPhase(phaseIn);
         if (!dragon->world->isRemote) {
            dragon->getDataManager().set(EntityDragon::PHASE, phaseIn->getId());
         }

         LOGGER->debug("Dragon is now in phase {} on the {}", phaseIn, dragon->world->isRemote ? "client" : "server");
         phase->initPhase();
      }
}

IPhase * PhaseManager::getCurrentPhase() const {
    return phase;
}

IPhase * PhaseManager::getPhase(PhaseList *phaseIn) {
    auto i = phaseIn->getId();
    return phases[i];
}
