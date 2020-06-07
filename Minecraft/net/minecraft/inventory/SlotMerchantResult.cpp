#include "SlotMerchantResult.h"

#include "../item/ItemStack.h"
#include "../stats/StatList.h"
#include "../village/MerchantRecipe.h"
#include "math/MathHelper.h"

SlotMerchantResult::SlotMerchantResult(EntityPlayer *player, IMerchant *merchant, InventoryMerchant *merchantInventoryIn,
                                       int32_t slotIndex, int32_t xPosition, int32_t yPosition)
        :Slot(merchantInventoryIn, slotIndex, xPosition, yPosition),player(player),merchant(merchant),merchantInventory(merchantInventoryIn)
{
}

bool SlotMerchantResult::isItemValid(ItemStack stack)
{
    return false;
}

ItemStack SlotMerchantResult::decrStackSize(int32_t amount)
{
    if (getHasStack()) 
    {
        removeCount += MathHelper::min(amount, getStack().getCount());
    }

    return Slot::decrStackSize(amount);
}

ItemStack SlotMerchantResult::onTake(EntityPlayer *thePlayer, ItemStack stack)
{
    onCrafting(stack);
    MerchantRecipe merchantrecipe = merchantInventory->getCurrentRecipe();
    ItemStack itemstack = merchantInventory->getStackInSlot(0);
    ItemStack itemstack1 = merchantInventory->getStackInSlot(1);
    if (doTrade(merchantrecipe, itemstack, itemstack1) || doTrade(merchantrecipe, itemstack1, itemstack)) 
    {
        merchant->useRecipe(merchantrecipe);
        thePlayer->addStat(StatList::TRADED_WITH_VILLAGER);
        merchantInventory->setInventorySlotContents(0, itemstack);
        merchantInventory->setInventorySlotContents(1, itemstack1);
    }

    return stack;
}

void SlotMerchantResult::onCrafting(ItemStack stack, int32_t amount)
{
    removeCount += amount;
    onCrafting(stack);
}

void SlotMerchantResult::onCrafting(ItemStack stack)
{
    stack.onCrafting(player->world, player, removeCount);
    removeCount = 0;
}

bool SlotMerchantResult::doTrade(const MerchantRecipe& trade, ItemStack firstItem, ItemStack secondItem)
{
    ItemStack itemstack = trade.getItemToBuy();
    ItemStack itemstack1 = trade.getSecondItemToBuy();
    if (firstItem.getItem() == itemstack.getItem() && firstItem.getCount() >= itemstack.getCount()) 
    {
        if (!itemstack1.isEmpty() && !secondItem.isEmpty() && itemstack1.getItem() == secondItem.getItem() && secondItem.getCount() >= itemstack1.getCount()) 
        {
            firstItem.shrink(itemstack.getCount());
            secondItem.shrink(itemstack1.getCount());
            return true;
        }

        if (itemstack1.isEmpty() && secondItem.isEmpty()) 
        {
            firstItem.shrink(itemstack.getCount());
            return true;
        }
    }

    return false;
}
