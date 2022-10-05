#pragma once

#include "IPhase.h"

#include <functional>
#include <string_view>
#include <vector>

class EntityDragon;
class PhaseList
{
  public:
    static std::unique_ptr<PhaseList> HOLDING_PATTERN;
    static std::unique_ptr<PhaseList> STRAFE_PLAYER;
    static std::unique_ptr<PhaseList> LANDING_APPROACH;
    static std::unique_ptr<PhaseList> LANDING;
    static std::unique_ptr<PhaseList> TAKEOFF;
    static std::unique_ptr<PhaseList> SITTING_FLAMING;
    static std::unique_ptr<PhaseList> SITTING_SCANNING;
    static std::unique_ptr<PhaseList> SITTING_ATTACKING;
    static std::unique_ptr<PhaseList> CHARGING_PLAYER;
    static std::unique_ptr<PhaseList> DYING;
    static std::unique_ptr<PhaseList> HOVER;
    std::unique_ptr<IPhase> createPhase(EntityDragon *dragon) const;
    int32_t getId() const;
    std::string toString();
    static PhaseList *getById(int32_t idIn);
    static int32_t getTotalPhases();

  private:
    PhaseList(int32_t idIn, std::function<std::unique_ptr<IPhase>(EntityDragon *)> clazzIn, std::string_view nameIn);
    static std::unique_ptr<PhaseList> create(std::function<std::unique_ptr<IPhase>(EntityDragon *)> phaseIn,
                                             std::string_view nameIn);
    static std::vector<PhaseList *> phases;
    std::function<std::unique_ptr<IPhase>(EntityDragon *)> clazz;
    int32_t id;
    std::string_view name;
};
