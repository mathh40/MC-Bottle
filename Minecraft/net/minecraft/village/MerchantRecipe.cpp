#include "MerchantRecipe.h"

#include "NBTTagCompound.h"

MerchantRecipe::MerchantRecipe(NBTTagCompound *tagCompound)
    : itemToBuy(ItemStack.EMPTY), secondItemToBuy(ItemStack.EMPTY), itemToSell(ItemStack.EMPTY),
      readFromTags(tagCompound)
{
}

MerchantRecipe::MerchantRecipe(ItemStack buy1, ItemStack buy2, ItemStack sell) : MerchantRecipe(buy1, buy2, sell, 0, 7)
{
}

MerchantRecipe::MerchantRecipe(ItemStack buy1, ItemStack buy2, ItemStack sell, int32_t toolUsesIn,
                               int32_t maxTradeUsesIn)
    : itemToBuy(ItemStack.EMPTY), secondItemToBuy(ItemStack.EMPTY), itemToSell(ItemStack.EMPTY), itemToBuy(buy1),
      secondItemToBuy(buy2), itemToSell(sell), toolUses(toolUsesIn), maxTradeUses(maxTradeUsesIn), rewardsExp(true)
{
}

MerchantRecipe::MerchantRecipe(ItemStack buy1, ItemStack sell) : MerchantRecipe(buy1, ItemStack.EMPTY, sell)
{
}

MerchantRecipe::MerchantRecipe(ItemStack buy1, Item sellItem) : MerchantRecipe(buy1, ItemStack(sellItem))
{
}

ItemStack MerchantRecipe::getItemToBuy() const
{
    return itemToBuy;
}

ItemStack MerchantRecipe::getSecondItemToBuy() const
{
    return secondItemToBuy;
}

bool MerchantRecipe::hasSecondItemToBuy() const
{
    return !secondItemToBuy.isEmpty();
}

ItemStack MerchantRecipe::getItemToSell() const
{
    return itemToSell;
}

int32_t MerchantRecipe::getToolUses() const
{
    return toolUses;
}

int32_t MerchantRecipe::getMaxTradeUses() const
{
    return maxTradeUses;
}

void MerchantRecipe::incrementToolUses()
{
    ++toolUses;
}

void MerchantRecipe::increaseMaxTradeUses(int32_t increment)
{
    maxTradeUses += increment;
}

bool MerchantRecipe::isRecipeDisabled() const
{
    return toolUses >= maxTradeUses;
}

void MerchantRecipe::compensateToolUses()
{
    toolUses = maxTradeUses;
}

bool MerchantRecipe::getRewardsExp() const
{
    return rewardsExp;
}

void MerchantRecipe::readFromTags(NBTTagCompound *tagCompound)
{
    NBTTagCompound *nbttagcompound  = tagCompound->getCompoundTag("buy");
    itemToBuy                       = ItemStack(nbttagcompound);
    NBTTagCompound *nbttagcompound1 = tagCompound->getCompoundTag("sell");
    itemToSell                      = ItemStack(nbttagcompound1);
    if (tagCompound->hasKey("buyB", 10))
    {
        secondItemToBuy = ItemStack(tagCompound->getCompoundTag("buyB"));
    }

    if (tagCompound->hasKey("uses", 99))
    {
        toolUses = tagCompound->getInteger("uses");
    }

    if (tagCompound->hasKey("maxUses", 99))
    {
        maxTradeUses = tagCompound->getInteger("maxUses");
    }
    else
    {
        maxTradeUses = 7;
    }

    if (tagCompound->hasKey("rewardExp", 1))
    {
        rewardsExp = tagCompound->getBoolean("rewardExp");
    }
    else
    {
        rewardsExp = true;
    }
}

NBTTagCompound *MerchantRecipe::writeToTags()
{
    NBTTagCompound *nbttagcompound = new NBTTagCompound();
    nbttagcompound->setTag("buy", itemToBuy.writeToNBT(new NBTTagCompound()));
    nbttagcompound->setTag("sell", itemToSell.writeToNBT(new NBTTagCompound()));
    if (!secondItemToBuy.isEmpty())
    {
        nbttagcompound->setTag("buyB", secondItemToBuy.writeToNBT(new NBTTagCompound()));
    }

    nbttagcompound->setInteger("uses", toolUses);
    nbttagcompound->setInteger("maxUses", maxTradeUses);
    nbttagcompound->setBoolean("rewardExp", rewardsExp);
    return nbttagcompound;
}
