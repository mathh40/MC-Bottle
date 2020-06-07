#pragma once
#include "../item/ItemStack.h"
#include "../world/IWorldNameable.h"

class IInventory :public IWorldNameable
{
public:
    virtual ~IInventory() = default;

    virtual int32_t getSizeInventory() const = 0;

    virtual bool isEmpty() const = 0 ;

    virtual ItemStack getStackInSlot(int32_t var1) = 0;

    virtual ItemStack decrStackSize(int32_t var1, int32_t var2) = 0;

    virtual ItemStack removeStackFromSlot(int32_t var1) = 0;

    virtual void setInventorySlotContents(int32_t var1, ItemStack var2) = 0;

    virtual int32_t getInventoryStackLimit() const = 0;

    virtual void markDirty() = 0;

    virtual bool isUsableByPlayer(EntityPlayer* var1) = 0;

    virtual void openInventory(EntityPlayer* var1) = 0;

    virtual void closeInventory(EntityPlayer* var1) = 0;

    virtual bool isItemValidForSlot(int32_t var1, ItemStack var2) = 0;

    virtual int32_t getField(int32_t var1) = 0;

    virtual void setField(int32_t var1, int32_t var2) = 0;

    virtual int32_t getFieldCount() = 0;

    virtual void clear() = 0;
};
