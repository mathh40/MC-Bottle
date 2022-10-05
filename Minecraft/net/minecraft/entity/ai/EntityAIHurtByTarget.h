#pragma once
#include "EntityAITarget.h"

#include <typeindex>
#include <vector>

class EntityAIHurtByTarget : public EntityAITarget
{
  public:
    EntityAIHurtByTarget(EntityCreature *creatureIn, bool entityCallsForHelpIn,
                         std::initializer_list<std::type_index> excludedReinforcementTypes);
    bool shouldExecute() override;
    void startExecuting() override;

  protected:
    void alertOthers();
    void setEntityAttackTarget(EntityCreature *creatureIn, EntityLivingBase *entityLivingBaseIn);

  private:
    bool entityCallsForHelp;
    int32_t revengeTimerOld;
    std::vector<std::type_index> excludedReinforcementTypes;
};
