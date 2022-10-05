#include "ContainerDispenser.h"

#include "IInventory.h"

ContainerDispenser::ContainerDispenser(IInventory *playerInventory, IInventory *dispenserInventoryIn)
    : dispenserInventory(dispenserInventoryIn)
{
    for (auto l = 0; l < 3; ++l)
    {
        for (auto i1 = 0; i1 < 3; ++i1)
        {
            addSlotToContainer(Slot(dispenserInventoryIn, i1 + l * 3, 62 + i1 * 18, 17 + l * 18));
        }
    }

    for (auto l = 0; l < 3; ++l)
    {
        for (auto i1 = 0; i1 < 9; ++i1)
        {
            addSlotToContainer(Slot(playerInventory, i1 + l * 9 + 9, 8 + i1 * 18, 84 + l * 18));
        }
    }

    for (auto l = 0; l < 9; ++l)
    {
        addSlotToContainer(Slot(playerInventory, l, 8 + l * 18, 142));
    }
}

bool ContainerDispenser::canInteractWith(EntityPlayer *playerIn)
{
    return dispenserInventory->isUsableByPlayer(playerIn);
}

ItemStack ContainerDispenser::transferStackInSlot(EntityPlayer *playerIn, int32_t index)
{
    ItemStack itemstack = ItemStack::EMPTY;
    auto slot           = inventorySlots[index];
    if (slot.has_value() && slot->getHasStack())
    {
        ItemStack itemstack1 = slot->getStack();
        itemstack            = itemstack1.copy();
        if (index < 9)
        {
            if (!mergeItemStack(itemstack1, 9, 45, true))
            {
                return ItemStack::EMPTY;
            }
        }
        else if (!mergeItemStack(itemstack1, 0, 9, false))
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
