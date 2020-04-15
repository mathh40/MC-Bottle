#pragma once
#include "Item.h"

class ItemBucketMilk :public Item
{
public:
    ItemBucketMilk();
    ItemStack onItemUseFinish(ItemStack stack, World* worldIn, EntityLivingBase* entityLiving) override;
    int32_t getMaxItemUseDuration(ItemStack stack) override;
    EnumAction getItemUseAction(ItemStack stack) override;
    ActionResult onItemRightClick(World* worldIn, EntityPlayer* playerIn, EnumHand handIn) override;
};