#include "InventoryMerchant.h"

#include "ItemStackHelper.h"
#include "../village/MerchantRecipeList.h"
#include "text/TextComponentString.h"
#include "text/TextComponentTranslation.h"

InventoryMerchant::InventoryMerchant(EntityPlayer *thePlayerIn, IMerchant *theMerchantIn)
    :slots(3, ItemStack::EMPTY),player(thePlayerIn),merchant(theMerchantIn),currentRecipe(ItemStack::EMPTY,ItemStack::EMPTY)
{
}

int32_t InventoryMerchant::getSizeInventory() const
{
    return slots.size();
}

bool InventoryMerchant::isEmpty()
{
    if(slots.empty())
    {
        return true;
    }

    bool isEmpty = true;
    for(auto itemstack : slots)
    {
        isEmpty = itemstack.isEmpty();
    }
    return isEmpty;
}

ItemStack InventoryMerchant::getStackInSlot(int32_t index)
{
    return slots[index];
}

ItemStack InventoryMerchant::decrStackSize(int32_t index, int32_t count)
{
    const ItemStack itemstack = slots[index];
    if (index == 2 && !itemstack.isEmpty()) 
    {
        return ItemStackHelper::getAndSplit(slots, index, itemstack.getCount());
    }
    else 
    {
        const ItemStack itemstack1 = ItemStackHelper::getAndSplit(slots, index, count);
        if (!itemstack1.isEmpty() && inventoryResetNeededOnSlotChange(index)) 
        {
            resetRecipeAndSlots();
        }

        return itemstack1;
    }
}

ItemStack InventoryMerchant::removeStackFromSlot(int32_t index)
{
    return ItemStackHelper::getAndRemove(slots, index);
}

void InventoryMerchant::setInventorySlotContents(int32_t index, ItemStack stack)
{
    slots[index] = stack;
    if (!stack.isEmpty() && stack.getCount() > getInventoryStackLimit()) 
    {
        stack.setCount(getInventoryStackLimit());
    }

    if (inventoryResetNeededOnSlotChange(index)) 
    {
        resetRecipeAndSlots();
    }
}

std::string InventoryMerchant::getName() const
{
    return "mob.villager";
}

bool InventoryMerchant::hasCustomName() const
{
    return false;
}

std::unique_ptr<ITextComponent> InventoryMerchant::getDisplayName() const
{
    return (ITextComponent)(hasCustomName() ? new TextComponentString(getName()) : new TextComponentTranslation(getName(), new Object[0]));
}

int32_t InventoryMerchant::getInventoryStackLimit() const
{
    return 64;
}

bool InventoryMerchant::isUsableByPlayer(EntityPlayer *player)
{
    return merchant->getCustomer() == player;
}

void InventoryMerchant::openInventory(EntityPlayer *player)
{
}

void InventoryMerchant::closeInventory(EntityPlayer *player)
{
}

bool InventoryMerchant::isItemValidForSlot(int32_t index, ItemStack stack)
{
    return true;
}

void InventoryMerchant::markDirty()
{
    resetRecipeAndSlots();
}

void InventoryMerchant::resetRecipeAndSlots()
{
    ItemStack itemstack = slots[0];
    ItemStack itemstack1 = slots[1];
    if (itemstack.isEmpty()) 
    {
        itemstack = itemstack1;
        itemstack1 = ItemStack::EMPTY;
    }

    if (itemstack.isEmpty()) 
    {
        setInventorySlotContents(2, ItemStack::EMPTY);
    }
    else {
        MerchantRecipeList merchantrecipelist = merchant->getRecipes(player);
        if (merchantrecipelist != nullptr) 
        {
            MerchantRecipe merchantrecipe = merchantrecipelist.canRecipeBeUsed(itemstack, itemstack1, this.currentRecipeIndex);
            if (merchantrecipe != nullptr && !merchantrecipe.isRecipeDisabled()) 
            {
                currentRecipe = merchantrecipe;
                setInventorySlotContents(2, merchantrecipe.getItemToSell().copy());
            }
            else if (!itemstack1.isEmpty()) 
            {
                merchantrecipe = merchantrecipelist.canRecipeBeUsed(itemstack1, itemstack, this.currentRecipeIndex);
                if (merchantrecipe != nullptr && !merchantrecipe.isRecipeDisabled()) 
                {
                    currentRecipe = merchantrecipe;
                    setInventorySlotContents(2, merchantrecipe.getItemToSell().copy());
                }
                else 
                {
                    setInventorySlotContents(2, ItemStack::EMPTY);
                }
            }
            else 
            {
                setInventorySlotContents(2, ItemStack::EMPTY);
            }
        }

        merchant->verifySellingItem(getStackInSlot(2));
    }
}

MerchantRecipe InventoryMerchant::getCurrentRecipe() const
{
    return currentRecipe;
}

void InventoryMerchant::setCurrentRecipeIndex(int32_t currentRecipeIndexIn)
{
    currentRecipeIndex = currentRecipeIndexIn;
    resetRecipeAndSlots();
}

int32_t InventoryMerchant::getField(int32_t id)
{
    return 0;
}

void InventoryMerchant::setField(int32_t id, int32_t value)
{
}

int32_t InventoryMerchant::getFieldCount()
{
    return 0;
}

void InventoryMerchant::clear()
{
    slots.clear();
}

bool InventoryMerchant::inventoryResetNeededOnSlotChange(int32_t slotIn)
{
    return slotIn == 0 || slotIn == 1;
}
