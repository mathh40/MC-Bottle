#pragma once
#include "Item.h"

class ItemPotion :public Item
{
    ItemPotion();
    ItemStack getDefaultInstance() override;
    ItemStack onItemUseFinish(ItemStack stack, World* worldIn, EntityLivingBase* entityLiving) override;
    int32_t getMaxItemUseDuration(ItemStack stack) override;
    EnumAction getItemUseAction(ItemStack stack) override;
    ActionResult onItemRightClick(World* worldIn, EntityPlayer* playerIn, EnumHand handIn) override;
    std::string getItemStackDisplayName(ItemStack stack) const override;
    void addInformation(ItemStack stack, World* worldIn, std::vector<std::string>& tooltip, ITooltipFlag* flagIn) override;
    bool hasEffect(ItemStack stack) override;
    void getSubItems(const CreativeTabs& tab, std::vector<ItemStack>& items) override;


};
