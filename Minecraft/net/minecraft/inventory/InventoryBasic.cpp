#include "InventoryBasic.h"

#include "IInventoryChangedListener.h"
#include "ItemStackHelper.h"
#include "Util.h"
#include "math/MathHelper.h"
#include "text/TextComponentString.h"
#include "text/TextComponentTranslation.h"

InventoryBasic::InventoryBasic(std::string_view title, bool customName, int32_t slotCount)
    : inventoryTitle(title), bhasCustomName(customName), slotsCount(slotCount),
      inventoryContents(slotCount, ItemStack::EMPTY)
{
}

InventoryBasic::InventoryBasic(ITextComponent *title, int32_t slotCount)
    : InventoryBasic(title->getUnformattedText(), true, slotCount)
{
}

void InventoryBasic::addInventoryChangeListener(IInventoryChangedListener *listener)
{
    changeListeners.emplace_back(listener);
}

void InventoryBasic::removeInventoryChangeListener(IInventoryChangedListener *listener)
{
    Util::erase(changeListeners, listener);
}

ItemStack InventoryBasic::getStackInSlot(int32_t index)
{
    return index >= 0 && index < inventoryContents.size() ? inventoryContents[index] : ItemStack::EMPTY;
}

ItemStack InventoryBasic::decrStackSize(int32_t index, int32_t count)
{
    ItemStack itemstack = ItemStackHelper::getAndSplit(inventoryContents, index, count);
    if (!itemstack.isEmpty())
    {
        markDirty();
    }

    return itemstack;
}

ItemStack InventoryBasic::addItem(ItemStack stack)
{
    ItemStack itemstack = stack.copy();

    for (auto i = 0; i < slotsCount; ++i)
    {
        ItemStack itemstack1 = getStackInSlot(i);
        if (itemstack1.isEmpty())
        {
            setInventorySlotContents(i, itemstack);
            markDirty();
            return ItemStack::EMPTY;
        }

        if (ItemStack::areItemsEqual(itemstack1, itemstack))
        {
            auto j = MathHelper::min(getInventoryStackLimit(), itemstack1.getMaxStackSize());
            auto k = MathHelper::min(itemstack.getCount(), j - itemstack1.getCount());
            if (k > 0)
            {
                itemstack1.grow(k);
                itemstack.shrink(k);
                if (itemstack.isEmpty())
                {
                    markDirty();
                    return ItemStack::EMPTY;
                }
            }
        }
    }

    if (itemstack.getCount() != stack.getCount())
    {
        markDirty();
    }

    return itemstack;
}

ItemStack InventoryBasic::removeStackFromSlot(int32_t index)
{
    ItemStack itemstack = inventoryContents[index];
    if (itemstack.isEmpty())
    {
        return ItemStack::EMPTY;
    }
    else
    {
        inventoryContents.emplace_back(index, ItemStack::EMPTY);
        return itemstack;
    }
}

void InventoryBasic::setInventorySlotContents(int32_t index, ItemStack stack)
{
    inventoryContents.emplace_back(index, stack);
    if (!stack.isEmpty() && stack.getCount() > getInventoryStackLimit())
    {
        stack.setCount(getInventoryStackLimit());
    }

    markDirty();
}

int32_t InventoryBasic::getSizeInventory() const
{
    return slotsCount;
}

bool InventoryBasic::isEmpty() const
{
    return inventoryContents.empty();
}

std::string InventoryBasic::getName() const
{
    return inventoryTitle;
}

bool InventoryBasic::hasCustomName() const
{
    return bhasCustomName;
}

void InventoryBasic::setCustomName(std::string_view inventoryTitleIn)
{
    bhasCustomName = true;
    inventoryTitle = inventoryTitleIn;
}

int32_t InventoryBasic::getInventoryStackLimit() const
{
    return 64;
}

bool InventoryBasic::isUsableByPlayer(EntityPlayer *player)
{
    return true;
}

void InventoryBasic::openInventory(EntityPlayer *player)
{
}

void InventoryBasic::closeInventory(EntityPlayer *player)
{
}

bool InventoryBasic::isItemValidForSlot(int32_t index, ItemStack stack)
{
    return true;
}

int32_t InventoryBasic::getField(int32_t id)
{
    return 0;
}

void InventoryBasic::setField(int32_t id, int32_t value)
{
}

int32_t InventoryBasic::getFieldCount()
{
    return 0;
}

void InventoryBasic::clear()
{
    inventoryContents.clear();
}

ITextComponent *InventoryBasic::getDisplayName() const
{
    return (ITextComponent)(hasCustomName() ? new TextComponentString(this.getName())
                                            : new TextComponentTranslation(getName(), new Object[0]));
}

void InventoryBasic::markDirty()
{
    if (!changeListeners.empty())
    {
        for (auto listener : changeListeners)
        {
            listener->onInventoryChanged(this);
        }
    }
}
