#pragma once
#include "IRecipe.h"

class InventoryCrafting;
class RecipeBookCloning :public IRecipe
{
public:
    bool matches(InventoryCrafting* inv, World* worldIn) override;
    ItemStack getCraftingResult(InventoryCrafting* inv) override;
    ItemStack getRecipeOutput() override;
    std::vector<ItemStack> getRemainingItems(InventoryCrafting* inv) override;
    bool isDynamic() override;
    bool canFit(int32_t width, int32_t height) override;
};