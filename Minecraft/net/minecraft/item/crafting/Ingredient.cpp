#include "Ingredient.h"

Ingredient Ingredient::EMPTY = Ingredient({},[&](ItemStack stack)->bool{return stack.isEmpty();});

const std::vector<ItemStack>& Ingredient::getMatchingStacks() const
{
    return matchingStacks;
}

const std::vector<int32_t> & Ingredient::getValidItemStacksPacked()
{
    if (matchingStacksPacked.empty()) 
    {
        matchingStacksPacked.reserve(matchingStacks.size());

        for(auto& itemstack : matchingStacks) 
        {
            matchingStacksPacked.emplace_back(RecipeItemHelper::pack(itemstack));
        }

        std::sort(matchingStacksPacked.begin(),matchingStacksPacked.end());
    }

    return matchingStacksPacked;
}

Ingredient Ingredient::fromItem(const Item *p_193367_0_)
{
    return fromStacks({ItemStack(p_193367_0_, 1, 32767)});
}

Ingredient Ingredient::fromItems(const std::vector<Item*>& items)
{
    auto aitemstack = std::vector<ItemStack>(items.size());

    for(auto i = 0 ; i < items.size(); ++i) 
    {
        aitemstack[i] = ItemStack(items[i]);
    }

    return fromStacks(aitemstack);
}

Ingredient Ingredient::fromStacks(const std::vector<ItemStack>& stacks)
{
    if (!stacks.empty()) 
    {
        for(auto& itemstack : stacks) 
        {
            if (!itemstack.isEmpty()) 
            {
                return Ingredient(stacks);
            }
        }
    }

    return Ingredient::EMPTY;
}

Ingredient::Ingredient(std::vector<ItemStack> p_i47503_1_)
    :matchingStacks(p_i47503_1_)
{
    apply = [&](ItemStack stack)->bool
    {
        for(auto& itemstack : matchingStacks) 
        {
            if (itemstack.getItem() == stack.getItem()) 
            {
                auto i = itemstack.getMetadata();
                if (i == 32767 || i == stack.getMetadata()) 
                {
                    return true;
                }
            }
        }

        return false;
    };
}

Ingredient::Ingredient(std::vector<ItemStack> p_i47503_1_, IngredientPredicate predicate)
    :Ingredient(p_i47503_1_)
{
    apply = predicate;
}

bool operator==(const ItemStack &other)
{

}
