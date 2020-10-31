#pragma once
#include "IMob.h"
#include "../EntityFlying.h"

class EntityGhast :public EntityFlying, public IMob {
public:
protected:
private:
    static DataParameter ATTACKING;
    int32_t explosionStrength = 1;
};