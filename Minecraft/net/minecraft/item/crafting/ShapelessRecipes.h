#pragma once
#include "../../../../../nlohmann_json/include/nlohmann/json_fwd.hpp"
#include "IRecipe.h"
#include "ShapedRecipes.h"

class ShapelessRecipes : public IRecipe
{
  public:
    ShapelessRecipes(std::string_view group, ItemStack output, const std::vector<Ingredient> &ingredients);
    std::string getGroup() override;
    ItemStack getRecipeOutput() override;
    std::vector<Ingredient> getIngredients() override;
    std::vector<ItemStack> getRemainingItems(InventoryCrafting *inv) override;
    bool matches(InventoryCrafting *inv, World *worldIn) override;
    ItemStack getCraftingResult(InventoryCrafting *inv) override;
    bool canFit(int32_t width, int32_t height) override;

  private:
    ItemStack recipeOutput;
    std::vector<Ingredient> recipeItems;
    std::string group;
};

std::vector<Ingredient> deserializeIngredients(nlohmann::json array);

namespace ns
{
void from_json(const nlohmann::json &json, ShapelessRecipes &p)
{
    std::string s    = json.value("group", "");
    auto nonnulllist = deserializeIngredients(json.at("ingredients"));
    if (nonnulllist.empty())
    {
        throw std::logic_error("No ingredients for shapeless recipe");
    }
    else if (nonnulllist.size() > 9)
    {
        throw std::logic_error("Too many ingredients for shapeless recipe");
    }
    else
    {
        ItemStack itemstack = deserializeItem(json.at("result"), true);
        p                   = ShapelessRecipes(s, itemstack, nonnulllist);
    }
}
} // namespace ns
