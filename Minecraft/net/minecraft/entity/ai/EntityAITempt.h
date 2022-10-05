#pragma once
#include "EntityAIBase.h"

#include <unordered_set>

class ItemStack;
class Item;
class EntityPlayer;
class EntityCreature;

class EntityAITempt : public EntityAIBase
{
  public:
    EntityAITempt(EntityCreature *temptedEntityIn, double speedIn, Item *temptItemIn, bool scaredByPlayerMovementIn);
    EntityAITempt(EntityCreature *temptedEntityIn, double speedIn, bool scaredByPlayerMovementIn,
                  const std::unordered_set<Item *> &temptItemIn);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void resetTask() override;
    void updateTask() override;
    bool isRunning() const;

  protected:
    bool isTempting(ItemStack &stack) const;

  private:
    EntityCreature *temptedEntity;
    double speed;
    double targetX;
    double targetY;
    double targetZ;
    double pitch;
    double yaw;
    EntityPlayer *temptingPlayer;
    int delayTemptCounter;
    bool bisRunning;
    std::unordered_set<Item *> temptItem;
    bool scaredByPlayerMovement;
};
