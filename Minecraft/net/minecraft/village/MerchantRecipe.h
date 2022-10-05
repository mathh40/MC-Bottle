#pragma once
#include "../item/ItemStack.h"

#include <cstdint>
class Item;
class NBTTagCompound;
class MerchantRecipe
{
  public:
    explicit MerchantRecipe(NBTTagCompound *tagCompound);
    MerchantRecipe(ItemStack buy1, ItemStack buy2, ItemStack sell);
    MerchantRecipe(ItemStack buy1, ItemStack buy2, ItemStack sell, int32_t toolUsesIn, int32_t maxTradeUsesIn);
    MerchantRecipe(ItemStack buy1, ItemStack sell);
    MerchantRecipe(ItemStack buy1, Item sellItem);
    ItemStack getItemToBuy() const;
    ItemStack getSecondItemToBuy() const;
    bool hasSecondItemToBuy() const;
    ItemStack getItemToSell() const;
    int32_t getToolUses() const;
    int32_t getMaxTradeUses() const;
    void incrementToolUses();
    void increaseMaxTradeUses(int32_t increment);
    bool isRecipeDisabled() const;
    void compensateToolUses();
    bool getRewardsExp() const;
    void readFromTags(NBTTagCompound *tagCompound);
    NBTTagCompound *writeToTags();

  private:
    ItemStack itemToBuy;
    ItemStack secondItemToBuy;
    ItemStack itemToSell;
    int32_t toolUses;
    int32_t maxTradeUses;
    bool rewardsExp;
};
