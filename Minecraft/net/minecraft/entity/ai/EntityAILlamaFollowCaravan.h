#pragma once
#include "EntityAIBase.h"

class EntityLlama;
class EntityAILlamaFollowCaravan : public EntityAIBase
{
  public:
    EntityAILlamaFollowCaravan(EntityLlama *llamaIn, double speedModifierIn);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void resetTask() override;
    void updateTask() override;

    EntityLlama *llama;

  private:
    bool firstIsLeashed(EntityLlama *p_190858_1_, int32_t p_190858_2_);

    double speedModifier;
    int32_t distCheckCounter;
};
