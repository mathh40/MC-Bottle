#include "ContainerMerchant.h"

ContainerMerchant::ContainerMerchant(InventoryPlayer *playerInventory, IMerchant *merchant, World *worldIn)
    :merchant(merchant),world(worldIn),merchantInventory(new InventoryMerchant(playerInventory->player, merchant))
{
    addSlotToContainer(Slot(merchantInventory, 0, 36, 53));
    addSlotToContainer(Slot(merchantInventory, 1, 62, 53));
    addSlotToContainer(SlotMerchantResult(playerInventory->player, merchant, merchantInventory, 2, 120, 53));

    for(auto k = 0; k < 3; ++k) 
    {
        for(auto j = 0; j < 9; ++j) 
        {
            addSlotToContainer(Slot(playerInventory, j + k * 9 + 9, 8 + j * 18, 84 + k * 18));
        }
    }

    for(auto k = 0; k < 9; ++k) 
    {
        addSlotToContainer(Slot(playerInventory, k, 8 + k * 18, 142));
    }
}

InventoryMerchant * ContainerMerchant::getMerchantInventory() const
{
    return merchantInventory;
}

void ContainerMerchant::onCraftMatrixChanged(IInventory *inventoryIn)
{
    merchantInventory->resetRecipeAndSlots();
    Container::onCraftMatrixChanged(inventoryIn);
}

void ContainerMerchant::setCurrentRecipeIndex(int32_t currentRecipeIndex)
{
    merchantInventory->setCurrentRecipeIndex(currentRecipeIndex);
}

bool ContainerMerchant::canInteractWith(EntityPlayer *playerIn)
{
    return merchant->getCustomer() == playerIn;
}

ItemStack ContainerMerchant::transferStackInSlot(EntityPlayer *playerIn, int32_t index)
{
    ItemStack itemstack = ItemStack::EMPTY;
    auto slot = inventorySlots[index];
    if (slot.has_value() && slot->getHasStack()) 
    {
        ItemStack itemstack1 = slot->getStack();
        itemstack = itemstack1.copy();
        if (index == 2) 
        {
            if (!mergeItemStack(itemstack1, 3, 39, true)) 
            {
                return ItemStack::EMPTY;
            }

            slot->onSlotChange(itemstack1, itemstack);
        }
        else if (index != 0 && index != 1) 
        {
            if (index >= 3 && index < 30) 
            {
                if (!mergeItemStack(itemstack1, 30, 39, false)) 
                {
                    return ItemStack::EMPTY;
                }
            }
            else if (index >= 30 && index < 39 && !mergeItemStack(itemstack1, 3, 30, false)) 
            {
                return ItemStack::EMPTY;
            }
        }
        else if (!mergeItemStack(itemstack1, 3, 39, false)) 
        {
            return ItemStack::EMPTY;
        }

        if (itemstack1.isEmpty()) 
        {
            slot->putStack(ItemStack::EMPTY);
        }
        else 
        {
            slot->onSlotChanged();
        }

        if (itemstack1.getCount() == itemstack.getCount()) 
        {
            return ItemStack::EMPTY;
        }

        slot->onTake(playerIn, itemstack1);
    }

    return itemstack;
}

void ContainerMerchant::onContainerClosed(EntityPlayer *playerIn)
{
    Container::onContainerClosed(playerIn);
    merchant->setCustomer(nullptr);
    Container::onContainerClosed(playerIn);
    if (!world->isRemote) 
    {
        ItemStack itemstack = merchantInventory->removeStackFromSlot(0);
        if (!itemstack.isEmpty()) 
        {
            playerIn->dropItem(itemstack, false);
        }

        itemstack = merchantInventory->removeStackFromSlot(1);
        if (!itemstack.isEmpty()) 
        {
            playerIn->dropItem(itemstack, false);
        }
    }
}
