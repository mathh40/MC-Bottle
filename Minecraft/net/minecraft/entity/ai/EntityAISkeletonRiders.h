#pragma once
#include "EntityAIBase.h"
class DifficultyInstance;
class EntitySkeletonHorse;
class AbstractHorse;
class EntitySkeleton;
class EntityAISkeletonRiders : public EntityAIBase
{
  public:
    explicit EntityAISkeletonRiders(EntitySkeletonHorse *horseIn);
    bool shouldExecute() override;
    void updateTask() override;

  private:
    AbstractHorse *createHorse(const DifficultyInstance &p_188515_1_) const;
    EntitySkeleton *createSkeleton(const DifficultyInstance p_188514_1_, const AbstractHorse *p_188514_2_);

    EntitySkeletonHorse *horse;
};
