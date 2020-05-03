#pragma once
#include "Item.h"

class ItemSeeds :public Item
{
public:
    ItemSeeds(Block* crops, Block* soil);
    EnumActionResult onItemUse(EntityPlayer* player, World* worldIn, BlockPos pos, EnumHand hand, EnumFacing facing, float hitX, float hitY, float hitZ) override;
private:
    Block* crops;
    Block* soilBlockID;
};