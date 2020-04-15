#pragma once
#include "Item.h"

class ItemCarrotOnAStick :public Item
{
public:
    ItemCarrotOnAStick();
    bool isFull3D() const override;
    bool shouldRotateAroundWhenRendering() override;
    ActionResult onItemRightClick(World* worldIn, EntityPlayer* playerIn, EnumHand handIn) override;
};