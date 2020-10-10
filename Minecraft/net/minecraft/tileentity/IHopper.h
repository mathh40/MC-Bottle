#pragma once
#include "../world/World.h"

class IHopper : public IInventory
{
public:
   virtual World* getWorld() = 0;

   virtual double getXPos() = 0;

   virtual double getYPos() = 0;

   virtual double getZPos() = 0;
};
