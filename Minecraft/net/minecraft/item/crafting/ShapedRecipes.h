#pragma once
#include "Ingredient.h"
#include "IRecipe.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ShapedRecipes :public IRecipe
{
public:
    ShapedRecipes(std::string group, int32_t width, int32_t height, std::vector<Ingredient> ingredients, ItemStack result);
    std::string getGroup() override;
    ItemStack getRecipeOutput() override;
    std::vector<ItemStack> getRemainingItems(InventoryCrafting* var1) override;
    std::vector<Ingredient> getIngredients() override;
    bool canFit(int32_t width, int32_t height) override;
    bool matches(InventoryCrafting* inv, World* worldIn) override;
    ItemStack getCraftingResult(InventoryCrafting* inv) override;
    int32_t getWidth() const;
    int32_t getHeight() const;

private:

    bool checkMatch(InventoryCrafting* craftingInventory, int32_t p_77573_2_, int32_t p_77573_3_, bool p_77573_4_);

    int32_t recipeWidth;
    int32_t recipeHeight;
    std::vector<Ingredient> recipeItems;
    ItemStack recipeOutput;
    std::string group;
};

std::vector<Ingredient> deserializeIngredients(std::vector<std::string>& pattern, std::unordered_map<std::string,Ingredient>& keys, int32_t patternWidth, int32_t patternHeight);
constexpr int32_t firstNonSpace(std::string_view str);
constexpr int32_t lastNonSpace(std::string_view str);
std::vector<std::string> patternFromJson(const json& jsonArr);
std::unordered_map<std::string,Ingredient> deserializeKey(const json& json);
Ingredient deserializeIngredient(const json& jsonElement);
ItemStack deserializeItem(const json& json, bool useCount);
std::vector<std::string> shrink(const std::vector<std::string>& toShrink);


namespace nlohmann
{
    template <>
    struct adl_serializer<ShapedRecipes>
    {
        static std::unique_ptr<ShapedRecipes> from_json(const json& j)
        {
            std::string group;
            j.at("group").get_to(group);
            auto map = deserializeKey(j.at("key"));
            auto astring = shrink(patternFromJson(j.at("pattern")));
            auto withe = astring[0].size();
            auto height = astring.size();
            auto nonnulllist = deserializeIngredients(astring, map, withe, height);
            ItemStack itemstack = deserializeItem(j.at("result"), true);
            return std::make_unique<ShapedRecipes>(group, withe, height, nonnulllist, itemstack);
        }
    };
} 