#include "EntityAITempt.h"

#include "../../item/ItemStack.h"
#include "../../pathfinding/PathNavigateGround.h"
#include "Util.h"

#include <utility>

EntityAITempt::EntityAITempt(EntityCreature *temptedEntityIn, double speedIn, Item *temptItemIn,
                             bool scaredByPlayerMovementIn)
    : EntityAITempt(temptedEntityIn, speedIn, scaredByPlayerMovementIn, {temptItemIn})
{
}

EntityAITempt::EntityAITempt(EntityCreature *temptedEntityIn, double speedIn, bool scaredByPlayerMovementIn,
                             const std::unordered_set<Item *> &temptItemIn)
    : temptedEntity(temptedEntityIn), speed(speedIn), temptItem(temptItemIn),
      scaredByPlayerMovement(scaredByPlayerMovementIn)
{
    setMutexBits(3);
    if (!(Util:: instanceof <PathNavigateGround>(temptedEntityIn->getNavigator())))
    {
        throw std::logic_error("Unsupported mob type for TemptGoal");
    }
}

bool EntityAITempt::shouldExecute()
{
    if (delayTemptCounter > 0)
    {
        --delayTemptCounter;
        return false;
    }
    else
    {
        temptingPlayer = temptedEntity->world.getClosestPlayerToEntity(temptedEntity, 10.0);
        if (temptingPlayer == nullptr)
        {
            return false;
        }
        else
        {
            return isTempting(temptingPlayer->getHeldItemMainhand()) ||
                   isTempting(temptingPlayer->getHeldItemOffhand());
        }
    }
}

bool EntityAITempt::shouldContinueExecuting()
{
    if (scaredByPlayerMovement)
    {
        if (temptedEntity->getDistanceSq(temptingPlayer) < 36.0)
        {
            if (temptingPlayer->getDistanceSq(targetX, targetY, targetZ) > 0.010000000000000002)
            {
                return false;
            }

            if (MathHelper::abs((double)temptingPlayer->rotationPitch - pitch) > 5.0 ||
                MathHelper::abs((double)temptingPlayer->rotationYaw - yaw) > 5.0)
            {
                return false;
            }
        }
        else
        {
            targetX = temptingPlayer->posX;
            targetY = temptingPlayer->posY;
            targetZ = temptingPlayer->posZ;
        }

        pitch = (double)temptingPlayer->rotationPitch;
        yaw   = (double)temptingPlayer->rotationYaw;
    }

    return shouldExecute();
}

void EntityAITempt::startExecuting()
{
    targetX    = temptingPlayer->posX;
    targetY    = temptingPlayer->posY;
    targetZ    = temptingPlayer->posZ;
    bisRunning = true;
}

void EntityAITempt::resetTask()
{
    temptingPlayer = nullptr;
    temptedEntity->getNavigator().clearPath();
    delayTemptCounter = 100;
    bisRunning        = false;
}

void EntityAITempt::updateTask()
{
    temptedEntity->getLookHelper().setLookPositionWithEntity(temptingPlayer,
                                                             (float)(temptedEntity->getHorizontalFaceSpeed() + 20),
                                                             (float)temptedEntity->getVerticalFaceSpeed());
    if (temptedEntity->getDistanceSq(temptingPlayer) < 6.25)
    {
        temptedEntity->getNavigator().clearPath();
    }
    else
    {
        temptedEntity->getNavigator().tryMoveToEntityLiving(temptingPlayer, speed);
    }
}

bool EntityAITempt::isRunning() const
{
    return bisRunning;
}

bool EntityAITempt::isTempting(ItemStack &stack) const
{
    return temptItem.contains(stack.getItem());
}
