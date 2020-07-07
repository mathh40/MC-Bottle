#pragma once
#include <functional>
#include <memory>

#include "EntityAIBase.h"
#include "../../../../../spdlog/include/spdlog/logger.h"

class Entity;
class EntityLiving;
class EntityLivingBase;
class EntityAIFindEntityNearestPlayer :public EntityAIBase
{
public:
    EntityAIFindEntityNearestPlayer(EntityLiving* entityLivingIn);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void resetTask() override;

protected:
    double maxTargetRange();

private:
    static std::shared_ptr<spdlog::logger> LOGGER;
    EntityLiving* entityLiving;
    std::function<bool(Entity*)> predicate;
    EntityAINearestAttackableTarget::Sorter sorter;
    EntityLivingBase* entityTarget;
};