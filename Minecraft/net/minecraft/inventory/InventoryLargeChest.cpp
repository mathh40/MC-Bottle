#include "InventoryLargeChest.h"

#include "ContainerChest.h"
#include "LockCode.h"
#include "text/TextComponentString.h"
#include "text/TextComponentTranslation.h"

InventoryLargeChest::InventoryLargeChest(std::string_view nameIn, ILockableContainer *upperChestIn,
                                         ILockableContainer *lowerChestIn)
    : name(nameIn), upperChest(upperChestIn ? upperChestIn : lowerChestIn),
      lowerChest(lowerChestIn ? lowerChestIn : upperChestIn)
{
    if (upperChestIn->isLocked())
    {
        lowerChestIn->setLockCode(upperChestIn->getLockCode());
    }
    else if (lowerChestIn->isLocked())
    {
        upperChestIn->setLockCode(lowerChestIn->getLockCode());
    }
}

int32_t InventoryLargeChest::getSizeInventory() const
{
    return upperChest->getSizeInventory() + lowerChest->getSizeInventory();
}

bool InventoryLargeChest::isEmpty() const
{
    return upperChest->isEmpty() && lowerChest->isEmpty();
}

bool InventoryLargeChest::isPartOfLargeChest(IInventory *inventoryIn) const
{
    return upperChest == inventoryIn || lowerChest == inventoryIn;
}

std::string InventoryLargeChest::getName() const
{
    if (upperChest->hasCustomName())
    {
        return upperChest->getName();
    }
    else
    {
        return lowerChest->hasCustomName() ? lowerChest->getName() : name;
    }
}

bool InventoryLargeChest::hasCustomName() const
{
    return upperChest->hasCustomName() || lowerChest->hasCustomName();
}

std::unique_ptr<ITextComponent> InventoryLargeChest::getDisplayName() const
{
    return (ITextComponent)(hasCustomName() ? new TextComponentString(getName())
                                            : new TextComponentTranslation(getName(), new Object[0]));
}

ItemStack InventoryLargeChest::getStackInSlot(int32_t index)
{
    return index >= upperChest->getSizeInventory() ? lowerChest->getStackInSlot(index - upperChest->getSizeInventory())
                                                   : upperChest->getStackInSlot(index);
}

ItemStack InventoryLargeChest::decrStackSize(int32_t index, int32_t count)
{
    return index >= upperChest->getSizeInventory()
               ? lowerChest->decrStackSize(index - upperChest->getSizeInventory(), count)
               : upperChest->decrStackSize(index, count);
}

ItemStack InventoryLargeChest::removeStackFromSlot(int32_t index)
{
    return index >= upperChest->getSizeInventory()
               ? lowerChest->removeStackFromSlot(index - upperChest->getSizeInventory())
               : upperChest->removeStackFromSlot(index);
}

void InventoryLargeChest::setInventorySlotContents(int32_t index, ItemStack stack)
{
    if (index >= upperChest->getSizeInventory())
    {
        lowerChest->setInventorySlotContents(index - upperChest->getSizeInventory(), stack);
    }
    else
    {
        upperChest->setInventorySlotContents(index, stack);
    }
}

int32_t InventoryLargeChest::getInventoryStackLimit() const
{
    return upperChest->getInventoryStackLimit();
}

void InventoryLargeChest::markDirty()
{
    upperChest->markDirty();
    lowerChest->markDirty();
}

bool InventoryLargeChest::isUsableByPlayer(EntityPlayer *player)
{
    return upperChest->isUsableByPlayer(player) && lowerChest->isUsableByPlayer(player);
}

void InventoryLargeChest::openInventory(EntityPlayer *player)
{
    upperChest->openInventory(player);
    lowerChest->openInventory(player);
}

void InventoryLargeChest::closeInventory(EntityPlayer *player)
{
    upperChest->closeInventory(player);
    lowerChest->closeInventory(player);
}

bool InventoryLargeChest::isItemValidForSlot(int32_t index, ItemStack stack)
{
    return true;
}

int32_t InventoryLargeChest::getField(int32_t id)
{
    return 0;
}

void InventoryLargeChest::setField(int32_t id, int32_t value)
{
}

int32_t InventoryLargeChest::getFieldCount()
{
    return 0;
}

bool InventoryLargeChest::isLocked()
{
    return upperChest->isLocked() || lowerChest->isLocked();
}

void InventoryLargeChest::setLockCode(const LockCode &code) const
{
    upperChest->setLockCode(code);
    lowerChest->setLockCode(code);
}

LockCode InventoryLargeChest::getLockCode()
{
    return upperChest->getLockCode();
}

std::string InventoryLargeChest::getGuiID() const
{
    return upperChest->getGuiID();
}

Container *InventoryLargeChest::createContainer(InventoryPlayer *playerInventory, EntityPlayer *playerIn)
{
    return new ContainerChest(playerInventory, this, playerIn);
}

void InventoryLargeChest::clear()
{
    upperChest->clear();
    lowerChest->clear();
}
