#include "Slot.h"

#include "../item/ItemStack.h"
#include "IInventory.h"

Slot::Slot(IInventory *inventoryIn, int32_t index, int32_t xPosition, int32_t yPosition)
    : inventory(inventoryIn), slotIndex(index), xPos(xPosition), yPos(yPosition)
{
}

void Slot::onSlotChange(ItemStack p_75220_1_, ItemStack p_75220_2_)
{
    int32_t i = p_75220_2_.getCount() - p_75220_1_.getCount();
    if (i > 0)
    {
        onCrafting(p_75220_2_, i);
    }
}

ItemStack Slot::onTake(EntityPlayer *thePlayer, ItemStack stack)
{
    onSlotChanged();
    return stack;
}

bool Slot::isItemValid(ItemStack stack)
{
    return true;
}

ItemStack Slot::getStack() const
{
    return inventory->getStackInSlot(slotIndex);
}

bool Slot::getHasStack() const
{
    return !getStack().isEmpty();
}

void Slot::putStack(ItemStack stack) const
{
    inventory->setInventorySlotContents(slotIndex, stack);
    onSlotChanged();
}

void Slot::onSlotChanged() const
{
    inventory->markDirty();
}

int32_t Slot::getSlotStackLimit() const
{
    return inventory->getInventoryStackLimit();
}

int32_t Slot::getItemStackLimit(ItemStack stack) const
{
    return getSlotStackLimit();
}

std::string Slot::getSlotTexture()
{
    return "";
}

ItemStack Slot::decrStackSize(int32_t amount) const
{
    return inventory->decrStackSize(slotIndex, amount);
}

bool Slot::isHere(IInventory *inv, int32_t slotIn) const
{
    return inv == inventory && slotIn == slotIndex;
}

bool Slot::canTakeStack(EntityPlayer *playerIn)
{
    return true;
}

bool Slot::isEnabled()
{
    return true;
}

void Slot::onCrafting(ItemStack stack, int32_t amount)
{
}

void Slot::onSwapCraft(int32_t p_190900_1_)
{
}

void Slot::onCrafting(ItemStack stack)
{
}
