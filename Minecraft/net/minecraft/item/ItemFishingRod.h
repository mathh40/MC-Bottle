#pragma once
#include "Item.h"

class ItemFishingRod :public Item
{
public:
    ItemFishingRod();
    bool isFull3D() const override;
    bool shouldRotateAroundWhenRendering() override;
    ActionResult onItemRightClick(World* worldIn, EntityPlayer* playerIn, EnumHand handIn) override;
    int32_t getItemEnchantability() override;
};