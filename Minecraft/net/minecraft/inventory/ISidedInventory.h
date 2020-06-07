#pragma once
#include "IInventory.h"

class ISidedInventory :public IInventory
{
public:
    virtual std::vector<int32_t> getSlotsForFace(EnumFacing var1) = 0;

    virtual bool canInsertItem(int32_t var1, ItemStack var2, EnumFacing var3) = 0;

    virtual bool canExtractItem(int32_t var1, ItemStack var2, EnumFacing var3) = 0;
};