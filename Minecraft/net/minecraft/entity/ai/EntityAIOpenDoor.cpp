#include "EntityAIOpenDoor.h"

EntityAIOpenDoor::EntityAIOpenDoor(EntityLiving *entitylivingIn, bool shouldClose)
    :EntityAIDoorInteract(entitylivingIn),entity(entitylivingIn),closeDoor(shouldClose)
{

}

bool EntityAIOpenDoor::shouldContinueExecuting()
{
    return closeDoor && closeDoorTemporisation > 0 && EntityAIDoorInteract::shouldContinueExecuting();
}

void EntityAIOpenDoor::startExecuting()
{
    closeDoorTemporisation = 20;
    doorBlock->toggleDoor(entity->world, doorPosition, true);
}

void EntityAIOpenDoor::resetTask()
{
    if (closeDoor) 
    {
        doorBlock->toggleDoor(entity->world, doorPosition, false);
    }
}

void EntityAIOpenDoor::updateTask()
{
    --closeDoorTemporisation;
    EntityAIDoorInteract::updateTask();
}
