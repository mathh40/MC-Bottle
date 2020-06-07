#pragma once
#include "Container.h"

class ContainerHopper :public Container
{
public:
    ContainerHopper(InventoryPlayer* playerInventory, IInventory* hopperInventoryIn, EntityPlayer* player);
    bool canInteractWith(EntityPlayer* playerIn) override;
    ItemStack transferStackInSlot(EntityPlayer* playerIn, int32_t index) override;
    void onContainerClosed(EntityPlayer* playerIn) override;

private:
    IInventory* hopperInventory;
};