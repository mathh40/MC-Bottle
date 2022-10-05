#include "ShapelessRecipes.h"

ShapelessRecipes::ShapelessRecipes(std::string_view group, ItemStack output, const std::vector<Ingredient> &ingredients)
    : group(group), recipeOutput(output), recipeItems(ingredients)
{
}

std::string ShapelessRecipes::getGroup()
{
    return group;
}

ItemStack ShapelessRecipes::getRecipeOutput()
{
    return recipeOutput;
}

std::vector<Ingredient> ShapelessRecipes::getIngredients()
{
    return recipeItems;
}

std::vector<ItemStack> ShapelessRecipes::getRemainingItems(InventoryCrafting *inv)
{
    std::vector<ItemStack> nonnulllist(inv->getSizeInventory(), ItemStack::EMPTY);

    for (auto i = 0; i < nonnulllist.size(); ++i)
    {
        ItemStack itemstack = inv->getStackInSlot(i);
        if (itemstack.getItem()->hasContainerItem())
        {
            nonnulllist[i] = ItemStack(itemstack.getItem()->getContainerItem());
        }
    }

    return nonnulllist;
}

bool ShapelessRecipes::matches(InventoryCrafting *inv, World *worldIn)
{
    auto list = recipeItems;

    for (auto i = 0; i < inv->getHeight(); ++i)
    {
        for (auto j = 0; j < inv->getWidth(); ++j)
        {
            ItemStack itemstack = inv->getStackInRowAndColumn(j, i);
            if (!itemstack.isEmpty())
            {
                bool flag = false;

                for (auto &ingredient : list)
                {
                    if (ingredient == itemstack)
                    {
                        flag = true;
                        list.remove(ingredient);
                        break;
                    }
                }

                if (!flag)
                {
                    return false;
                }
            }
        }
    }

    return list.empty();
}

ItemStack ShapelessRecipes::getCraftingResult(InventoryCrafting *inv)
{
    return recipeOutput.copy();
}

bool ShapelessRecipes::canFit(int32_t width, int32_t height)
{
    return width * height >= recipeItems.size();
}

std::vector<Ingredient> deserializeIngredients(nlohmann::json arrays)
{
    std::vector<Ingredient> nonnulllist;

    for (auto i = 0; i < arrays.size(); ++i)
    {
        Ingredient ingredient = deserializeIngredient(arrays[i]);
        if (ingredient != Ingredient::EMPTY)
        {
            nonnulllist.emplace_back(ingredient);
        }
    }

    return nonnulllist;
}
