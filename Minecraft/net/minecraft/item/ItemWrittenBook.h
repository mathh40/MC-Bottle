#pragma once
#include "Item.h"

class ItemWrittenBook : public Item
{
  public:
    ItemWrittenBook();
    static bool validBookTagContents(NBTTagCompound *nbt);
    static int32_t getGeneration(ItemStack book);
    std::string getItemStackDisplayName(ItemStack stack) const override;
    void addInformation(ItemStack stack, World *worldIn, std::vector<std::string> &tooltip,
                        ITooltipFlag *flagIn) override;
    ActionResult onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn) override;
    bool hasEffect(ItemStack stack) override;

  private:
    void resolveContents(ItemStack stack, EntityPlayer *player);
};
