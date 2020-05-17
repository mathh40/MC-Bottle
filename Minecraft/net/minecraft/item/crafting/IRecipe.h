#pragma once
#include "Ingredient.h"
#include "../ItemStack.h"
class InventoryCrafting;
class World;

class IRecipe
{
public:
    IRecipe() = default;
    virtual ~IRecipe() = default;
    virtual bool matches(InventoryCrafting* var1, World* var2) = 0;
    virtual ItemStack getCraftingResult(InventoryCrafting* var1) = 0;
    virtual bool canFit(int32_t var1, int32_t var2) = 0;
    virtual ItemStack getRecipeOutput() = 0;
    virtual std::vector<ItemStack> getRemainingItems(InventoryCrafting* var1) = 0;

    virtual std::vector<Ingredient> getIngredients()
    {
        return std::vector<Ingredient>();
    }

    virtual bool isDynamic()
    {
        return false;
    }

    virtual std::string getGroup()
    {
        return "";
    }
};
