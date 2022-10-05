#pragma once
#include "../../../../../spdlog/include/spdlog/logger.h"
#include "EntityAIBase.h"

#include <functional>
#include <memory>
#include <typeindex>

class EntityLivingBase;
class EntityLiving;
class EntityAIFindEntityNearest : public EntityAIBase
{
  public:
    EntityAIFindEntityNearest(EntityLiving *mobIn, std::type_index p_i45884_2_);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void resetTask() override;

  protected:
    double getFollowRange();

  private:
    static std::shared_ptr<spdlog::logger> LOGGER;
    EntityLiving *mob;
    std::function<bool(EntityLivingBase *)> predicate;
    EntityAINearestAttackableTarget::Sorter sorter;
    EntityLivingBase *target;
    std::type_index classToCheck;
};
