#pragma once
#include "ItemFood.h"

class ItemSeedFood :public ItemFood
{
public:
    ItemSeedFood(int32_t healAmount, float saturation, Block* crops, Block* soil);
    EnumActionResult onItemUse(EntityPlayer* player, World* worldIn, BlockPos pos, EnumHand hand, EnumFacing facing, float hitX, float hitY, float hitZ) override;

private:
    Block* crops;
    Block* soilId;
};
