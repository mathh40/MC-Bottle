#pragma once
#include "EntityAINearestAttackableTarget.h"

class EntityTameable;

class EntityAITargetNonTamed :public EntityAINearestAttackableTarget
{
public:
    EntityAITargetNonTamed(EntityTameable* entityIn, std::type_index classTarget, bool checkSight, std::function<bool()> targetSelector);
    bool shouldExecute() override;
private:
    EntityTameable* tameable;
};
