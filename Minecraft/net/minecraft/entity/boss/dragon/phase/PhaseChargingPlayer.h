#pragma once
#include "PhaseBase.h"

class PhaseChargingPlayer :public PhaseBase {
public:
    PhaseChargingPlayer(EntityDragon* dragonIn);
    void doLocalUpdate() override;
    void initPhase() override;
    void setTarget(const Vec3d& p_188668_1_);
    float getMaxRiseOrFall() override;
    std::optional<Vec3d> getTargetLocation() override;
    PhaseList* getType() override;

private:
    static std::shared_ptr<spdlog::logger> LOGGER;
    std::optional<Vec3d> targetLocation;
    int32_t timeSinceCharge;
};