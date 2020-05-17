#pragma once
#include "IRecipe.h"
#include "../../tileentity/BannerPattern.h"

namespace RecipesBanners
{
    class RecipeDuplicatePattern :public IRecipe
    {
    public:
        bool matches(InventoryCrafting* inv, World* worldIn) override;
        ItemStack getCraftingResult(InventoryCrafting* inv) override;
        ItemStack getRecipeOutput() override;
        std::vector<ItemStack> getRemainingItems(InventoryCrafting* inv) override;
        bool isDynamic() override;
        bool canFit(int32_t width, int32_t height) override;
    };

    class RecipeAddPattern :public IRecipe
    {
    public:
        bool matches(InventoryCrafting* inv, World* worldIn) override;
        ItemStack getCraftingResult(InventoryCrafting* inv) override;
        ItemStack getRecipeOutput() override;
        std::vector<ItemStack> getRemainingItems(InventoryCrafting* inv) override;
        bool isDynamic() override;
        bool canFit(int32_t width, int32_t height) override;

    private:
        std::optional<BannerPattern> matchPatterns(InventoryCrafting* p_190933_1_);
    };
}
