#pragma once
#include "Item.h"

class ItemSaddle :public Item
{
public:
    ItemSaddle();
    bool itemInteractionForEntity(ItemStack stack, EntityPlayer* playerIn, EntityLivingBase* target, EnumHand hand) override;
};