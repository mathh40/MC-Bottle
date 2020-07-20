#pragma once
#include "EntityAIDoorInteract.h"

class EntityAIOpenDoor :public EntityAIDoorInteract
{
public:
    EntityAIOpenDoor(EntityLiving* entitylivingIn, bool shouldClose);
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void resetTask() override;
    void updateTask() override;

private:
    bool closeDoor;
    int32_t closeDoorTemporisation;
};