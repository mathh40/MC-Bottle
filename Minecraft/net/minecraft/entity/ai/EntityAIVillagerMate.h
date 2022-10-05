#pragma once
#include "EntityAIBase.h"
#include "EntityAILookAtVillager.h"

class World;
class Village;

class EntityAIVillagerMate : public EntityAIBase
{
  public:
    explicit EntityAIVillagerMate(EntityVillager *villagerIn);
    bool shouldExecute() override;
    void startExecuting() override;
    void resetTask() override;
    bool shouldContinueExecuting() override;
    void updateTask() override;

  private:
    bool checkSufficientDoorsPresentForNewVillager() const;
    void giveBirth();

    EntityVillager *villager;
    EntityVillager *mate;
    World *world;
    int32_t matingTimeout;
    Village *village;
};
