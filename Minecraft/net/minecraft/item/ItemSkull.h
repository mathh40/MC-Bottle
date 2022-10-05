#pragma once
#include "Item.h"

class ItemSkull : public Item
{
  public:
    ItemSkull();
    EnumActionResult onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand, EnumFacing facing,
                               float hitX, float hitY, float hitZ) override;
    void getSubItems(const CreativeTabs &tab, std::vector<ItemStack> &items) override;
    int32_t getMetadata(int32_t damage) const override;
    std::string getTranslationKey(ItemStack stack) const override;
    std::string getItemStackDisplayName(ItemStack stack) const override;
    bool updateItemStackNBT(NBTTagCompound *nbt) override;

  private:
    static constexpr std::array<std::string_view, 6> SKULL_TYPES = {"skeleton", "wither",  "zombie",
                                                                    "char",     "creeper", "dragon"};
};
