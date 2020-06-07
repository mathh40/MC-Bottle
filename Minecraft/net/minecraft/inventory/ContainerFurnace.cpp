#include "ContainerFurnace.h"

#include "IContainerListener.h"
#include "IInventory.h"
#include "../item/crafting/FurnaceRecipes.h"
#include "../tileentity/TileEntityFurnace.h"

ContainerFurnace::ContainerFurnace(InventoryPlayer *playerInventory, IInventory *furnaceInventory)
    :tileFurnace(furnaceInventory)
{
    addSlotToContainer(Slot(furnaceInventory, 0, 56, 17));
    addSlotToContainer(SlotFurnaceFuel(furnaceInventory, 1, 56, 53));
    addSlotToContainer(SlotFurnaceOutput(playerInventory->player, furnaceInventory, 2, 116, 35));

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

void ContainerFurnace::addListener(IContainerListener *listener)
{
    Container::addListener(listener);
    listener->sendAllWindowProperties(*this, tileFurnace);
}

void ContainerFurnace::detectAndSendChanges()
{
    Container::detectAndSendChanges();

    for(auto i = 0; i < listeners.size(); ++i) 
    {
        IContainerListener* icontainerlistener = listeners[i];
        if (cookTime != tileFurnace->getField(2)) 
        {
            icontainerlistener->sendWindowProperty(*this, 2, tileFurnace->getField(2));
        }

        if (furnaceBurnTime != tileFurnace->getField(0)) 
        {
            icontainerlistener->sendWindowProperty(*this, 0, tileFurnace->getField(0));
        }

        if (currentItemBurnTime != tileFurnace->getField(1)) 
        {
            icontainerlistener->sendWindowProperty(*this, 1, tileFurnace->getField(1));
        }

        if (totalCookTime != tileFurnace->getField(3)) 
        {
            icontainerlistener->sendWindowProperty(*this, 3, tileFurnace->getField(3));
        }
    }

    cookTime = tileFurnace->getField(2);
    furnaceBurnTime = tileFurnace->getField(0);
    currentItemBurnTime = tileFurnace->getField(1);
    totalCookTime = tileFurnace->getField(3);
}

void ContainerFurnace::updateProgressBar(int32_t id, int32_t data)
{
    tileFurnace->setField(id, data);
}

bool ContainerFurnace::canInteractWith(EntityPlayer *playerIn)
{
    return tileFurnace->isUsableByPlayer(playerIn);
}

ItemStack ContainerFurnace::transferStackInSlot(EntityPlayer *playerIn, int32_t index)
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
        else if (index != 1 && index != 0) 
        {
            if (!FurnaceRecipes::instance().getSmeltingResult(itemstack1).isEmpty()) 
            {
                if (!mergeItemStack(itemstack1, 0, 1, false)) 
                {
                    return ItemStack::EMPTY;
                }
            }
            else if (TileEntityFurnace::isItemFuel(itemstack1)) 
            {
                if (!mergeItemStack(itemstack1, 1, 2, false)) 
                {
                    return ItemStack::EMPTY;
                }
            }
            else if (index >= 3 && index < 30) 
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
        } else if (!mergeItemStack(itemstack1, 3, 39, false)) 
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
