#pragma once
#include "ShapedRecipes.h"

class MapData;

class RecipesMapExtending : public ShapedRecipes
{
  public:
    RecipesMapExtending();
    bool matches(InventoryCrafting *inv, World *worldIn) override;
    ItemStack getCraftingResult(InventoryCrafting *inv) override;
    bool isDynamic() override;

  private:
    bool isExplorationMap(MapData p_190934_1_);
};
