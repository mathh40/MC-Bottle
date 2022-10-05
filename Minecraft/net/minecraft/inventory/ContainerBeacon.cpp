#include "ContainerBeacon.h"

#include "IContainerListener.h"
#include "IInventory.h"

BeaconSlot::BeaconSlot(IInventory *inventoryIn, int32_t index, int32_t xIn, int32_t yIn)
    : Slot(inventoryIn, index, xIn, yIn)
{
}

bool BeaconSlot::isItemValid(ItemStack stack)
{
    Item *item = stack.getItem();
    return item == Items::EMERALD || item == Items::DIAMOND || item == Items::GOLD_INGOT || item == Items::IRON_INGOT;
}

int32_t BeaconSlot::getSlotStackLimit() const
{
    return 1;
}

ContainerBeacon::ContainerBeacon(IInventory *playerInventory, IInventory *tileBeaconIn)
    : tileBeacon(tileBeaconIn), beaconSlot(tileBeaconIn, 0, 136, 110)
{
    addSlotToContainer(beaconSlot);
    bool i = true;
    bool j = true;

    for (auto i1 = 0; i1 < 3; ++i1)
    {
        for (auto l = 0; l < 9; ++l)
        {
            addSlotToContainer(Slot(playerInventory, l + i1 * 9 + 9, 36 + l * 18, 137 + i1 * 18));
        }
    }

    for (auto i1 = 0; i1 < 9; ++i1)
    {
        addSlotToContainer(Slot(playerInventory, i1, 36 + i1 * 18, 195));
    }
}

void ContainerBeacon::addListener(IContainerListener *listener)
{
    Container::addListener(listener);
    listener->sendAllWindowProperties(*this, tileBeacon);
}

void ContainerBeacon::updateProgressBar(int32_t id, int32_t data)
{
    tileBeacon->setField(id, data);
}

IInventory *ContainerBeacon::getTileEntity() const
{
    return tileBeacon;
}

void ContainerBeacon::onContainerClosed(EntityPlayer *playerIn)
{
    Container::onContainerClosed(playerIn);
    if (!playerIn->world.isRemote)
    {
        ItemStack itemstack = beaconSlot.decrStackSize(beaconSlot.getSlotStackLimit());
        if (!itemstack.isEmpty())
        {
            playerIn->dropItem(itemstack, false);
        }
    }
}

bool ContainerBeacon::canInteractWith(EntityPlayer *playerIn)
{
    return tileBeacon->isUsableByPlayer(playerIn);
}

ItemStack ContainerBeacon::transferStackInSlot(EntityPlayer *playerIn, int32_t index)
{
    ItemStack itemstack = ItemStack::EMPTY;
    Slot slot           = inventorySlots[index];
    if (slot != nullptr && slot.getHasStack())
    {
        ItemStack itemstack1 = slot.getStack();
        itemstack            = itemstack1.copy();
        if (index == 0)
        {
            if (!mergeItemStack(itemstack1, 1, 37, true))
            {
                return ItemStack::EMPTY;
            }

            slot.onSlotChange(itemstack1, itemstack);
        }
        else if (!beaconSlot.getHasStack() && beaconSlot.isItemValid(itemstack1) && itemstack1.getCount() == 1)
        {
            if (!mergeItemStack(itemstack1, 0, 1, false))
            {
                return ItemStack::EMPTY;
            }
        }
        else if (index >= 1 && index < 28)
        {
            if (!mergeItemStack(itemstack1, 28, 37, false))
            {
                return ItemStack::EMPTY;
            }
        }
        else if (index >= 28 && index < 37)
        {
            if (!mergeItemStack(itemstack1, 1, 28, false))
            {
                return ItemStack::EMPTY;
            }
        }
        else if (!mergeItemStack(itemstack1, 1, 37, false))
        {
            return ItemStack::EMPTY;
        }

        if (itemstack1.isEmpty())
        {
            slot.putStack(ItemStack::EMPTY);
        }
        else
        {
            slot.onSlotChanged();
        }

        if (itemstack1.getCount() == itemstack.getCount())
        {
            return ItemStack::EMPTY;
        }

        slot.onTake(playerIn, itemstack1);
    }

    return itemstack;
}
