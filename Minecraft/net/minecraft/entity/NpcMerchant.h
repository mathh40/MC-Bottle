#pragma once
#include "IMerchant.h"
#include "../inventory/InventoryMerchant.h"
#include "../village/MerchantRecipeList.h"

class NpcMerchant :public IMerchant {
public:
    NpcMerchant(EntityPlayer* customerIn, ITextComponent* nameIn);
    EntityPlayer* getCustomer() override;
    void setCustomer(EntityPlayer* player) override;
    std::optional<MerchantRecipeList> getRecipes(EntityPlayer* player) override;
    void setRecipes(std::optional<MerchantRecipeList> recipeListIn);
    void useRecipe(MerchantRecipe recipe) override;
    void verifySellingItem(ItemStack stack) override;
    ITextComponent* getDisplayName() override;
    World* getWorld() override;
    BlockPos getPos() override;


private:
    InventoryMerchant merchantInventory;
    EntityPlayer* customer;
    std::optional<MerchantRecipeList> recipeList;
    ITextComponent* name;
};
