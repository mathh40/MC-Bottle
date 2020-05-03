#pragma once
#include "ItemTool.h"

class ItemSpade :public ItemTool
{
public:
    ItemSpade(ToolMaterial material);
    bool canHarvestBlock(IBlockState* blockIn) override;
    EnumActionResult onItemUse(EntityPlayer* player, World* worldIn, BlockPos pos, EnumHand hand, EnumFacing facing, float hitX, float hitY, float hitZ) override;
private:
    static std::unordered_set<Block*> EFFECTIVE_ON;
};