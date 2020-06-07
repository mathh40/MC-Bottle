#include "InventoryCrafting.h"



#include "Container.h"
#include "ItemStackHelper.h"
#include "text/TextComponentString.h"
#include "text/TextComponentTranslation.h"

InventoryCrafting::InventoryCrafting(Container *eventHandlerIn, int32_t width, int32_t height)
    :stackList(width * height, ItemStack::EMPTY),eventHandler(eventHandlerIn),inventoryWidth(width),inventoryHeight(height)
{
}

int32_t InventoryCrafting::getSizeInventory()
{
    return stackList.size();
}

bool InventoryCrafting::isEmpty()
{
    if(stackList.empty())
    {
        return true;
    }

    bool isEmpty = true;
    for(auto itemstack : stackList)
    {
        isEmpty = itemstack.isEmpty();
    }
    return isEmpty;
}

ItemStack InventoryCrafting::getStackInSlot(int32_t index)
{
    return index >= getSizeInventory() ? ItemStack::EMPTY : stackList[index];
}

ItemStack InventoryCrafting::getStackInRowAndColumn(int32_t row, int32_t column)
{
    return row >= 0 && row < inventoryWidth && column >= 0 && column <= inventoryHeight ? getStackInSlot(row + column * inventoryWidth) : ItemStack::EMPTY;
}

std::string InventoryCrafting::getName() const
{
    return "container.crafting";
}

bool InventoryCrafting::hasCustomName() const
{
    return false;
}

std::unique_ptr<ITextComponent> InventoryCrafting::getDisplayName() const
{
    return (ITextComponent)(hasCustomName() ? new TextComponentString(getName()) : new TextComponentTranslation(getName(), new Object[0]));
}

ItemStack InventoryCrafting::removeStackFromSlot(int32_t index)
{
    return ItemStackHelper::getAndRemove(stackList, index);
}

ItemStack InventoryCrafting::decrStackSize(int32_t index, int32_t count)
{
    const ItemStack itemstack = ItemStackHelper::getAndSplit(stackList, index, count);
    if (!itemstack.isEmpty()) 
    {
        eventHandler->onCraftMatrixChanged(this);
    }

    return itemstack;
}

void InventoryCrafting::setInventorySlotContents(int32_t index, ItemStack stack)
{
    stackList[index] = stack;
    eventHandler->onCraftMatrixChanged(this);
}

int32_t InventoryCrafting::getInventoryStackLimit() const
{
    return 64;
}

void InventoryCrafting::markDirty()
{
}

bool InventoryCrafting::isUsableByPlayer(EntityPlayer *player)
{
    return true;
}

void InventoryCrafting::openInventory(EntityPlayer *player)
{
}

void InventoryCrafting::closeInventory(EntityPlayer *player)
{
}

bool InventoryCrafting::isItemValidForSlot(int32_t index, ItemStack stack)
{
    return true;
}

int32_t InventoryCrafting::getField(int32_t id)
{
    return 0;
}

void InventoryCrafting::setField(int32_t id, int32_t value)
{

}

int32_t InventoryCrafting::getFieldCount()
{
    return 0;
}

void InventoryCrafting::clear()
{
    stackList.clear();
}

int32_t InventoryCrafting::getHeight() const
{
    return inventoryHeight;
}

int32_t InventoryCrafting::getWidth() const
{
    return inventoryWidth;
}

void InventoryCrafting::fillStackedContents(const RecipeItemHelper& helper)
{
    for(auto& itemstack : stackList)
    {
        helper.accountStack(itemstack);
    }
}