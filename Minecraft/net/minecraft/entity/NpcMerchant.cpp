#include "NpcMerchant.h"

#include "text/TextComponentTranslation.h"

NpcMerchant::NpcMerchant(EntityPlayer *customerIn, ITextComponent *nameIn)
    :customer(customerIn),name(nameIn),merchantInventory(customerIn, this)
{

}

EntityPlayer * NpcMerchant::getCustomer() {
    return customer;
}

void NpcMerchant::setCustomer(EntityPlayer *player) {

}

std::optional<MerchantRecipeList> NpcMerchant::getRecipes(EntityPlayer *player) {
    return recipeList;
}

void NpcMerchant::setRecipes(std::optional<MerchantRecipeList> recipeListIn) {
    recipeList = recipeListIn;
}

void NpcMerchant::useRecipe(MerchantRecipe recipe) {
    recipe.incrementToolUses();
}

void NpcMerchant::verifySellingItem(ItemStack stack) {

}

ITextComponent * NpcMerchant::getDisplayName() {
    return (ITextComponent*)(name != nullptr ? name : new TextComponentTranslation("entity.Villager.name", new Object[0]));
}

World * NpcMerchant::getWorld() {
    return customer->world;
}

BlockPos NpcMerchant::getPos() {
    return BlockPos(customer);
}
