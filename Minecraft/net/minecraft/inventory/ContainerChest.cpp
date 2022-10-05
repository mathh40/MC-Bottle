#include "ContainerChest.h"

#include "IInventory.h"

ContainerChest::ContainerChest(IInventory *playerInventory, IInventory *chestInventory, EntityPlayer *player)
    : lowerChestInventory(chestInventory), numRows(chestInventory->getSizeInventory() / 9)
{
    chestInventory->openInventory(player);
    auto i = (numRows - 4) * 18;

    for (auto i1 = 0; i1 < numRows; ++i1)
    {
        for (auto j1 = 0; j1 < 9; ++j1)
        {
            addSlotToContainer(Slot(chestInventory, j1 + i1 * 9, 8 + j1 * 18, 18 + i1 * 18));
        }
    }

    for (auto i1 = 0; i1 < 3; ++i1)
    {
        for (auto j1 = 0; j1 < 9; ++j1)
        {
            addSlotToContainer(Slot(playerInventory, j1 + i1 * 9 + 9, 8 + j1 * 18, 103 + i1 * 18 + i));
        }
    }

    for (auto i1 = 0; i1 < 9; ++i1)
    {
        addSlotToContainer(Slot(playerInventory, i1, 8 + i1 * 18, 161 + i));
    }
}

bool ContainerChest::canInteractWith(EntityPlayer *playerIn)
{
    return lowerChestInventory->isUsableByPlayer(playerIn);
}

ItemStack ContainerChest::transferStackInSlot(EntityPlayer *playerIn, int32_t index)
{
    ItemStack itemstack = ItemStack::EMPTY;
    auto slot           = inventorySlots[index];
    if (slot.has_value() && slot->getHasStack())
    {
        ItemStack itemstack1 = slot->getStack();
        itemstack            = itemstack1.copy();
        if (index < numRows * 9)
        {
            if (!mergeItemStack(itemstack1, numRows * 9, inventorySlots.size(), true))
            {
                return ItemStack::EMPTY;
            }
        }
        else if (!mergeItemStack(itemstack1, 0, numRows * 9, false))
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

void ContainerChest::onContainerClosed(EntityPlayer *playerIn)
{
    Container::onContainerClosed(playerIn);
    lowerChestInventory->closeInventory(playerIn);
}

IInventory *ContainerChest::getLowerChestInventory() const
{
    return lowerChestInventory;
}
