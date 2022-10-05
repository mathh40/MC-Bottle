#pragma once
#include "EntityAITarget.h"

class EntityIronGolem;
class EntityAIDefendVillage : public EntityAITarget
{
  public:
    explicit EntityAIDefendVillage(EntityIronGolem *ironGolemIn);
    bool shouldExecute() override;
    void startExecuting() override;

  private:
    EntityIronGolem *irongolem;
    EntityLivingBase *villageAgressorTarget;
};
