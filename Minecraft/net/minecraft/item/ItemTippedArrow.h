#pragma once
#include "ItemArrow.h"

class ItemTippedArrow :public ItemArrow
{
public:
    ItemStack getDefaultInstance() override;
    EntityArrow* createArrow(World* worldIn, ItemStack stack, EntityLivingBase* shooter) override;
    void getSubItems(const CreativeTabs& tab, std::vector<ItemStack>& items) override;
    void addInformation(ItemStack stack, World* worldIn, std::vector<std::string>& tooltip, ITooltipFlag* flagIn) override;
    std::string getItemStackDisplayName(ItemStack stack) const override;
};