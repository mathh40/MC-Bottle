#pragma once
#include "Container.h"

class ContainerChest :public Container
{
public:
    ContainerChest(IInventory* playerInventory, IInventory* chestInventory, EntityPlayer* player);
    bool canInteractWith(EntityPlayer* playerIn) override;
    ItemStack transferStackInSlot(EntityPlayer* playerIn, int32_t index) override;
    void onContainerClosed(EntityPlayer* playerIn) override;
    IInventory* getLowerChestInventory() const;

private:
    IInventory* lowerChestInventory;
    int32_t numRows;
};