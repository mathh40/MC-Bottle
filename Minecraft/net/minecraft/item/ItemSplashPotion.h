#pragma once
#include "ItemPotion.h"

class ItemSplashPotion : public ItemPotion
{
  public:
    std::string getItemStackDisplayName(ItemStack stack) const override;
    ActionResult onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn) override;
};
