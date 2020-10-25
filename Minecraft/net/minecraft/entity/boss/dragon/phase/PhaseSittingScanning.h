#pragma once
#include "PhaseSittingBase.h"

class PhaseSittingScanning :public PhaseSittingBase {
public:
    PhaseSittingScanning(EntityDragon* dragonIn);
    void doLocalUpdate() override;
    void initPhase() override;
    PhaseList* getType() override;


private:
    int32_t scanningTime;
};