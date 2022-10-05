#pragma once
#include "ItemPotion.h"

class ItemLingeringPotion : public ItemPotion
{
  public:
    std::string getItemStackDisplayName(ItemStack stack) const override;
    void addInformation(ItemStack stack, World *worldIn, std::vector<std::string> &tooltip,
                        ITooltipFlag *flagIn) override;
    ActionResult onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn) override;
};
