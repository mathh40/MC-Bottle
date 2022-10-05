#pragma once
#include "EntityAIWatchClosest2.h"

class EntityVillager;

class EntityAIVillagerInteract : public EntityAIWatchClosest2
{
  public:
    explicit EntityAIVillagerInteract(EntityVillager *villagerIn);
    void startExecuting() override;
    void updateTask() override;

  private:
    int32_t interactionDelay;
    EntityVillager *villager;
};
