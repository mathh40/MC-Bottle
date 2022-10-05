#pragma once
#include <unordered_map>

class Item;
class Block;
class ItemStack;

class FurnaceRecipes
{
  public:
    static FurnaceRecipes &instance();

    void addSmeltingRecipeForBlock(Block *input, ItemStack stack, float experience);
    void addSmelting(Item *input, ItemStack stack, float experience);
    void addSmeltingRecipe(ItemStack input, ItemStack stack, float experience);
    ItemStack getSmeltingResult(ItemStack stack);
    std::unordered_map<ItemStack, ItemStack> &getSmeltingList();
    float getSmeltingExperience(ItemStack stack);

  private:
    FurnaceRecipes();
    bool compareItemStacks(ItemStack stack1, ItemStack stack2);

    static FurnaceRecipes SMELTING_BASE;
    std::unordered_map<ItemStack, ItemStack> smeltingList{};
    std::unordered_map<ItemStack, float> experienceList{};
};
