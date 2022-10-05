#pragma once
#include "../../../../../../../spdlog/include/spdlog/logger.h"
#include "IPhase.h"
#include "PhaseList.h"

#include <memory>

class EntityDragon;

class PhaseManager
{
  public:
    PhaseManager(EntityDragon *dragonIn);
    void setPhase(PhaseList *phaseIn);
    IPhase *getCurrentPhase() const;
    IPhase *getPhase(PhaseList *phaseIn);

  private:
    static std::shared_ptr<spdlog::logger> LOGGER;
    EntityDragon *dragon;
    std::vector<IPhase *> phases;
    IPhase *phase;
};
