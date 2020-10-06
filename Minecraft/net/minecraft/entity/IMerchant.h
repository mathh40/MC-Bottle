#pragma once
#include "math/BlockPos.h"

#include <optional>

class ItemStack;
class MerchantRecipe;
class MerchantRecipeList;
class EntityPlayer;
class World;
class ITextComponent;

class IMerchant
{
    virtual void setCustomer(EntityPlayer* var1) = 0;

    virtual EntityPlayer* getCustomer() = 0;

    virtual std::optional<MerchantRecipeList> getRecipes(EntityPlayer* var1) = 0;

    virtual void setRecipes(MerchantRecipeList var1) = 0;

    virtual void useRecipe(MerchantRecipe var1) = 0;

    virtual void verifySellingItem(ItemStack var1) = 0;

    virtual ITextComponent* getDisplayName() = 0;

    virtual World* getWorld() = 0;

    virtual BlockPos getPos() = 0;
};
