#pragma once
#include "Item.h"

class ItemShield : public Item
{
  public:
    ItemShield();
    std::string getItemStackDisplayName(ItemStack stack) const override;
    void addInformation(ItemStack stack, World *worldIn, std::vector<std::string> &tooltip,
                        ITooltipFlag *flagIn) override;
    EnumAction getItemUseAction(ItemStack stack) override;
    int32_t getMaxItemUseDuration(ItemStack stack) override;
    ActionResult onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn) override;
    bool getIsRepairable(ItemStack toRepair, ItemStack repair) override;
};
