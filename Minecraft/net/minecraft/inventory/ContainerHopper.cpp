#include "ContainerHopper.h"

#include "IInventory.h"

ContainerHopper::ContainerHopper(InventoryPlayer *playerInventory, IInventory *hopperInventoryIn, EntityPlayer *player)
    : hopperInventory(hopperInventoryIn)
{
    hopperInventoryIn->openInventory(player);

    for (auto i1 = 0; i1 < hopperInventoryIn->getSizeInventory(); ++i1)
    {
        addSlotToContainer(Slot(hopperInventoryIn, i1, 44 + i1 * 18, 20));
    }

    for (auto i1 = 0; i1 < 3; ++i1)
    {
        for (int k = 0; k < 9; ++k)
        {
            addSlotToContainer(Slot(playerInventory, k + i1 * 9 + 9, 8 + k * 18, i1 * 18 + 51));
        }
    }

    for (auto i1 = 0; i1 < 9; ++i1)
    {
        addSlotToContainer(Slot(playerInventory, i1, 8 + i1 * 18, 109));
    }
}

bool ContainerHopper::canInteractWith(EntityPlayer *playerIn)
{
    return hopperInventory->isUsableByPlayer(playerIn);
}

ItemStack ContainerHopper::transferStackInSlot(EntityPlayer *playerIn, int32_t index)
{
    ItemStack itemstack = ItemStack::EMPTY;
    auto slot           = inventorySlots[index];
    if (slot.has_value() && slot->getHasStack())
    {
        ItemStack itemstack1 = slot->getStack();
        itemstack            = itemstack1.copy();
        if (index < hopperInventory->getSizeInventory())
        {
            if (!mergeItemStack(itemstack1, hopperInventory->getSizeInventory(), inventorySlots.size(), true))
            {
                return ItemStack::EMPTY;
            }
        }
        else if (!mergeItemStack(itemstack1, 0, hopperInventory->getSizeInventory(), false))
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
    }

    return itemstack;
}

void ContainerHopper::onContainerClosed(EntityPlayer *playerIn)
{
    Container::onContainerClosed(playerIn);
    hopperInventory->closeInventory(playerIn);
}
