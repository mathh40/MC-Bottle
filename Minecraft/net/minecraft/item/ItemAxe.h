#pragma once
#include "ItemTool.h"

class ItemAxe : public ItemTool
{
  public:
    float getDestroySpeed(ItemStack stack, IBlockState *state) override;

  protected:
    explicit ItemAxe(ToolMaterial material);

  private:
    static constexpr float ATTACK_DAMAGES[] = {6.0F, 8.0F, 8.0F, 8.0F, 6.0F};
    static constexpr float ATTACK_SPEEDS[]  = {-3.2F, -3.2F, -3.1F, -3.0F, -3.0F};
    static std::unordered_set<Block *> EFFECTIVE_ON;
};
