#pragma once
#include "EntityAIBase.h"
#include "../../village/VillageDoorInfo.h"

class EntityCreature;

class EntityAIRestrictOpenDoor :public EntityAIBase
{
public:
    EntityAIRestrictOpenDoor(EntityCreature* creatureIn);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void resetTask() override;
    void updateTask() override;

private:
    EntityCreature* entity;
    std::optional<VillageDoorInfo> frontDoor;
};
