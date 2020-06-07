#include "ContainerShulkerBox.h"

class SlotShulkerBox : public Slot
{
public:
    SlotShulkerBox(IInventory* inventoryIn, int32_t index, int32_t xPosition, int32_t yPosition);
};

SlotShulkerBox::SlotShulkerBox(IInventory *inventoryIn, int32_t index, int32_t xPosition, int32_t yPosition)
    :Slot(inventoryIn, index, xPosition, yPosition)
{
}

ContainerShulkerBox::ContainerShulkerBox(InventoryPlayer *playerInventoryIn, IInventory *inventoryIn,
                                         EntityPlayer *player)
        :inventory(inventoryIn)
{
    inventoryIn->openInventory(player);

    for(auto j1 = 0; j1 < 3; ++j1) 
    {
        for(auto k1 = 0; k1 < 9; ++k1) 
        {
            addSlotToContainer(SlotShulkerBox(inventoryIn, k1 + j1 * 9, 8 + k1 * 18, 18 + j1 * 18));
        }
    }

    for(auto j1 = 0; j1 < 3; ++j1) 
    {
        for(auto k1 = 0; k1 < 9; ++k1) 
        {
            addSlotToContainer(Slot(playerInventoryIn, k1 + j1 * 9 + 9, 8 + k1 * 18, 84 + j1 * 18));
        }
    }

    for(auto j1 = 0; j1 < 9; ++j1) 
    {
        addSlotToContainer(Slot(playerInventoryIn, j1, 8 + j1 * 18, 142));
    }
}

bool ContainerShulkerBox::canInteractWith(EntityPlayer *playerIn)
{
    return inventory->isUsableByPlayer(playerIn);
}

ItemStack ContainerShulkerBox::transferStackInSlot(EntityPlayer *playerIn, int32_t index)
{
    ItemStack itemstack = ItemStack::EMPTY;
    auto slot = inventorySlots[index];
    if (slot.has_value() && slot->getHasStack()) 
    {
        ItemStack itemstack1 = slot->getStack();
        itemstack = itemstack1.copy();
        if (index < inventory->getSizeInventory()) 
        {
            if (!mergeItemStack(itemstack1, inventory->getSizeInventory(), inventorySlots.size(), true)) 
            {
                return ItemStack::EMPTY;
            }
        }
        else if (!mergeItemStack(itemstack1, 0, inventory->getSizeInventory(), false)) 
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

void ContainerShulkerBox::onContainerClosed(EntityPlayer *playerIn)
{
    Container::onContainerClosed(playerIn);
    inventory->closeInventory(playerIn);
}
