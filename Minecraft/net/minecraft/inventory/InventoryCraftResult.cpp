#include "InventoryCraftResult.h"



#include "ItemStackHelper.h"
#include "text/TextComponentString.h"
#include "text/TextComponentTranslation.h"

InventoryCraftResult::InventoryCraftResult()
    :stackResult(1, ItemStack::EMPTY)
{
}

int32_t InventoryCraftResult::getSizeInventory()
{
    return 1;
}

bool InventoryCraftResult::isEmpty()
{
    if(stackResult.empty())
    {
        return true;
    }

    bool isEmpty = true;
    for(auto itemstack : stackResult)
    {
        isEmpty = itemstack.isEmpty();
    }
    return isEmpty;
}

ItemStack InventoryCraftResult::getStackInSlot(int32_t index)
{
    return stackResult[0];
}

std::string InventoryCraftResult::getName() const
{
    return "Result";
}

bool InventoryCraftResult::hasCustomName() const
{
    return false;
}

std::unique_ptr<ITextComponent> InventoryCraftResult::getDisplayName() const
{
    return (ITextComponent)(hasCustomName() ? new TextComponentString(getName()) : new TextComponentTranslation(getName(), new Object[0]));
}

ItemStack InventoryCraftResult::decrStackSize(int32_t index, int32_t count)
{
    return ItemStackHelper::getAndRemove(stackResult, 0);
}

ItemStack InventoryCraftResult::removeStackFromSlot(int32_t index)
{
    return ItemStackHelper::getAndRemove(stackResult, 0);
}

void InventoryCraftResult::setInventorySlotContents(int32_t index, ItemStack stack)
{
    stackResult[0, stack];
}

int32_t InventoryCraftResult::getInventoryStackLimit() const
{
    return 64;
}

void InventoryCraftResult::markDirty()
{

}

bool InventoryCraftResult::isUsableByPlayer(EntityPlayer *player)
{
    return true;
}

void InventoryCraftResult::openInventory(EntityPlayer *player)
{
}

void InventoryCraftResult::closeInventory(EntityPlayer *player)
{
}

bool InventoryCraftResult::isItemValidForSlot(int32_t index, ItemStack stack)
{
    return true;
}

int32_t InventoryCraftResult::getField(int32_t id)
{
    return 0;
}

void InventoryCraftResult::setField(int32_t id, int32_t value)
{

}

int32_t InventoryCraftResult::getFieldCount()
{
    return 0;
}

void InventoryCraftResult::clear()
{
    stackResult.clear();
}

void InventoryCraftResult::setRecipeUsed(IRecipe *p_193056_1_)
{
    recipeUsed = p_193056_1_;
}

IRecipe * InventoryCraftResult::getRecipeUsed() const
{
    return recipeUsed;
}
