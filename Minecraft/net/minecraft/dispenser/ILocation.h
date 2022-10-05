#pragma once
#include "IPosition.h"
class World;
class ILocation : public IPosition
{
  public:
    virtual World *getWorld() = 0;
};
