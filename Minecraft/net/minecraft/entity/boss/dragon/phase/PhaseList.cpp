#include "PhaseList.h"

#include "PhaseChargingPlayer.h"

std::unique_ptr<PhaseList> PhaseList::HOLDING_PATTERN = PhaseList::create([](EntityDragon* dragon){return std::make_unique<PhaseHoldingPattern>(dragon);}, "HoldingPattern");
std::unique_ptr<PhaseList> PhaseList::STRAFE_PLAYER = PhaseList::create([](EntityDragon* dragon){return std::make_unique<PhaseStrafePlayer>(dragon);}, "StrafePlayer");
std::unique_ptr<PhaseList> PhaseList::LANDING_APPROACH = PhaseList::create([](EntityDragon* dragon){return std::make_unique<PhaseLandingApproach>(dragon);}, "LandingApproach");
std::unique_ptr<PhaseList> PhaseList::LANDING = PhaseList::create([](EntityDragon* dragon){return std::make_unique<PhaseLanding>(dragon);}, "Landing");
std::unique_ptr<PhaseList> PhaseList::TAKEOFF = PhaseList::create([](EntityDragon* dragon){return std::make_unique<PhaseTakeoff>(dragon);}, "Takeoff");
std::unique_ptr<PhaseList> PhaseList::SITTING_FLAMING = PhaseList::create([](EntityDragon* dragon){return std::make_unique<PhaseSittingFlaming>(dragon);}, "SittingFlaming");
std::unique_ptr<PhaseList> PhaseList::SITTING_SCANNING = PhaseList::create([](EntityDragon* dragon){return std::make_unique<PhaseSittingScanning>(dragon);}, "SittingScanning");
std::unique_ptr<PhaseList> PhaseList::SITTING_ATTACKING = PhaseList::create([](EntityDragon* dragon){return std::make_unique<PhaseSittingAttacking>(dragon);}, "SittingAttacking");
std::unique_ptr<PhaseList> PhaseList::CHARGING_PLAYER = PhaseList::create([](EntityDragon* dragon){return std::make_unique<PhaseChargingPlayer>(dragon);}, "ChargingPlayer");
std::unique_ptr<PhaseList> PhaseList::DYING = PhaseList::create([](EntityDragon* dragon){return std::make_unique<PhaseDying>(dragon);}, "Dying");
std::unique_ptr<PhaseList> PhaseList::HOVER = PhaseList::create([](EntityDragon* dragon){return std::make_unique<PhaseHover>(dragon);}, "Hover");

std::unique_ptr<IPhase> PhaseList::createPhase(EntityDragon* dragon) const {
    return clazz(dragon);
}

int32_t PhaseList::getId() const {
    return id;
}

std::string PhaseList::toString() {
    return name + " (#" + id + ")";
}

PhaseList* PhaseList::getById(int32_t idIn) {
    return idIn >= 0 && idIn < phases.size() ? phases[idIn] : HOLDING_PATTERN.get();
}

int32_t PhaseList::getTotalPhases() {
    return phases.size();
}

PhaseList::PhaseList(int32_t idIn, std::function<std::unique_ptr<IPhase>(EntityDragon *)> clazzIn, std::string_view nameIn)
    :id(idIn),clazz(clazzIn),name(nameIn){

}

std::unique_ptr<PhaseList> PhaseList::create(std::function<std::unique_ptr<IPhase>(EntityDragon *)> phaseIn, std::string_view nameIn) {
    auto phaselist = std::make_unique<PhaseList>(phases.size(), phaseIn, nameIn);
    phases.resize(phases.size() + 1);
    phases[phaselist->getId()] = phaselist.get();
    return phaselist;
}
