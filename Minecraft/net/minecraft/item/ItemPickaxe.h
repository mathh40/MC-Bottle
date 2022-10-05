#pragma once
#include "ItemTool.h"

class ItemPickaxe : public ItemTool
{
  public:
    bool canHarvestBlock(IBlockState *blockIn) override;
    float getDestroySpeed(ItemStack stack, IBlockState *state) override;

  protected:
    ItemPickaxe(ToolMaterial material);

  private:
    static std::unordered_set<Block *> EFFECTIVE_ON;
};
