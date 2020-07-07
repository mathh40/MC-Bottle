#pragma once
#include "EntityMoveHelper.h"

class EntityFlyHelper :public EntityMoveHelper
{
public:
    explicit EntityFlyHelper(EntityLiving* p_i47418_1_);
    void onUpdateMoveHelper();
};


