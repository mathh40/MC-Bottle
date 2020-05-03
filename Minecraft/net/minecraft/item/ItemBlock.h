#pragma once
#include "Item.h"

class ItemBlock :public Item
{
public:
    ItemBlock(Block* block);
    EnumActionResult onItemUse(EntityPlayer* player, World* worldIn, BlockPos pos, EnumHand hand, EnumFacing facing, float hitX, float hitY, float hitZ) override;
    static bool setTileEntityNBT(World* worldIn, EntityPlayer* player, BlockPos pos, ItemStack stackIn);
    virtual bool canPlaceBlockOnSide(World* worldIn, BlockPos pos, EnumFacing side, EntityPlayer* player, ItemStack stack);
    std::string getTranslationKey(ItemStack stack) const override;
    std::string getTranslationKey() const override;
    CreativeTabs& getCreativeTab() override;
    void getSubItems(const CreativeTabs& tab, std::vector<ItemStack>& items) override;
    void addInformation(ItemStack stack, World* worldIn, std::vector<std::string>& tooltip, ITooltipFlag* flagIn) override;
    Block* getBlock() const;


protected:
    Block* block;
private:
};