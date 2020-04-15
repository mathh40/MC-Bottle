#pragma once
#include "ItemBlock.h"

class NBTTagList;
class EnumDyeColor;

class ItemBanner :public ItemBlock
{
public:
    ItemBanner();
    EnumActionResult onItemUse(EntityPlayer* player, World* worldIn, BlockPos pos, EnumHand hand, EnumFacing facing, float hitX, float hitY, float hitZ) override;
    std::string getItemStackDisplayName(ItemStack stack) const override;
    static void appendHoverTextFromTileEntityTag(ItemStack stack, std::vector<std::string>& p_185054_1_);
    void addInformation(ItemStack stack, World* worldIn, std::vector<std::string>& tooltip, ITooltipFlag* flagIn) override;
    void getSubItems(const CreativeTabs& tab, std::vector<ItemStack>& items) override;
    static ItemStack makeBanner(EnumDyeColor color, NBTTagList* patterns);
    CreativeTabs& getCreativeTab() override;
    static EnumDyeColor getBaseColor(ItemStack stack);
};
