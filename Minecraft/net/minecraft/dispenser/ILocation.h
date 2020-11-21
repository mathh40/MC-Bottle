#pragma once
#include "IPosition.h"
class World;
class ILocation :public IPosition {
   virtual World* getWorld() = 0;
};