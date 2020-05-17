#pragma once
#include <functional>

#include "../ItemStack.h"

class Ingredient
{
public:
    using IngredientPredicate = std::function<bool(ItemStack)>;

    static Ingredient EMPTY;
    const std::vector<ItemStack>& getMatchingStacks() const;
    const std::vector<int32_t>& getValidItemStacksPacked();

    static Ingredient fromItem(const Item* p_193367_0_);
    static Ingredient fromItems(const std::vector<Item*>& items);
    static Ingredient fromStacks(const std::vector<ItemStack>& stacks);

    friend bool operator==(const ItemStack& other);

private:
    std::vector<ItemStack> matchingStacks;
    std::vector<int32_t> matchingStacksPacked;

    IngredientPredicate apply;

    Ingredient(std::vector<ItemStack> p_i47503_1_);
    Ingredient(std::vector<ItemStack> p_i47503_1_,IngredientPredicate predicate);

};
