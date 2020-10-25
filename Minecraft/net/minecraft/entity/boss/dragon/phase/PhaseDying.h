#pragma once
#include "PhaseBase.h"

class PhaseDying :public PhaseBase {
public:
    PhaseDying(EntityDragon* dragonIn);
    void doClientRenderEffects() override;
    void doLocalUpdate() override;
    void initPhase() override;
    float getMaxRiseOrFall() override;
    std::optional<Vec3d> getTargetLocation() override;
    PhaseList* getType() override;
private:
    std::optional<Vec3d> targetLocation;
    int32_t time;
};