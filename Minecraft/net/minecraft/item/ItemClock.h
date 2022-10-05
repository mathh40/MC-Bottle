#pragma once
#include "../world/World.h"
#include "Item.h"
#include "ItemStack.h"

class ClockProperty
{
  public:
    float apply(ItemStack stack, World *worldIn, EntityLivingBase *entityIn);
    double wobble(World *p_185087_1_, double p_185087_2_);

  private:
    double rotation;
    double rota;
    long lastUpdateTick;
};

class ItemClock : public Item
{
  public:
    ItemClock();

  private:
    ClockProperty proberty;
};
