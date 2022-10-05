#pragma once
#include "../passive/IAnimals.h"

class Entity;

class IMob : public IAnimals
{
  public:
    bool MOB_SELECTOR(Entity *p_apply_1_) const;
    bool VISIBLE_MOB_SELECTOR(Entity *p_apply_1_) const;
};
