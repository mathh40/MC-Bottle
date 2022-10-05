#pragma once
#include "Item.h"

class ItemBed : public Item
{
  public:
    ItemBed();
    EnumActionResult onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand, EnumFacing facing,
                               float hitX, float hitY, float hitZ) override;
    std::string getTranslationKey(ItemStack stack) const override;
    void getSubItems(const CreativeTabs &tab, std::vector<ItemStack> &items) override;

  private:
};
