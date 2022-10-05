#include "RecipeBook.h"

void RecipeBook::copyFrom(RecipeBook that)
{
    recipes.clear();
    newRecipes.clear();
    recipes |= that.recipes;
    newRecipes |= that.newRecipes;
}

void RecipeBook::unlock(IRecipe *recipe)
{
    if (!recipe->isDynamic())
    {
        recipes.set(getRecipeId(recipe));
    }
}

bool RecipeBook::isUnlocked(IRecipe *recipe)
{
    return recipes[getRecipeId(recipe)];
}

void RecipeBook::lock(IRecipe *recipe)
{
    int32_t i = getRecipeId(recipe);
    recipes.set(i, false);
    newRecipes.set(i, false);
}

bool RecipeBook::isNew(IRecipe *recipe)
{
    return newRecipes[getRecipeId(recipe)];
}

void RecipeBook::markSeen(IRecipe *recipe)
{
    newRecipes.set(getRecipeId(recipe), false);
}

void RecipeBook::markNew(IRecipe *recipe)
{
    newRecipes.set(getRecipeId(recipe));
}

bool RecipeBook::isGuiOpen() const
{
    return bisGuiOpen;
}

void RecipeBook::setGuiOpen(bool open)
{
    bisGuiOpen = open;
}

bool RecipeBook::isFilteringCraftable() const
{
    return bisFilteringCraftable;
}

void RecipeBook::setFilteringCraftable(bool shouldFilter) const
{
    bisFilteringCraftable = shouldFilter;
}

int32_t RecipeBook::getRecipeId(IRecipe *recipe)
{
    return CraftingManager::REGISTRY.getIDForObject(recipe);
}
