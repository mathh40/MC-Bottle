#pragma once
#include "Item.h"

class ItemBoat : public Item
{
  public:
    explicit ItemBoat(EntityBoat::Type typeIn);
    ActionResult onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn) override;

  private:
    EntityBoat::Type type;
};
