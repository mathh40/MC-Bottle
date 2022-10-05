#pragma once
#include "EntityAIBase.h"

class EntityLivingBase;
class EntityCreeper;

class EntityAICreeperSwell : public EntityAIBase
{
  public:
    explicit EntityAICreeperSwell(EntityCreeper *entitycreeperIn);
    bool shouldExecute() override;
    void startExecuting() override;
    void resetTask() override;
    void updateTask() override;

  private:
    EntityCreeper *swellingCreeper;
    EntityLivingBase *creeperAttackTarget;
};
