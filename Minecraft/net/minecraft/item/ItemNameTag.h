#pragma once
#include "Item.h"

class ItemNameTag :public Item
{
public:
    ItemNameTag();
    bool itemInteractionForEntity(ItemStack stack, EntityPlayer* playerIn, EntityLivingBase* target, EnumHand hand) override;
};
