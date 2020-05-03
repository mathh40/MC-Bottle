#pragma once
#include "Item.h"

class ItemShears :public Item
{
public:
    ItemShears();
    bool onBlockDestroyed(ItemStack stack, World* worldIn, IBlockState* state, BlockPos pos, EntityLivingBase* entityLiving) override;
    bool canHarvestBlock(IBlockState* blockIn) override;
    float getDestroySpeed(ItemStack stack, IBlockState* state) override;

};