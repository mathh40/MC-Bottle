#pragma once
#include "Container.h"

class ContainerDispenser : public Container
{
  public:
    ContainerDispenser(IInventory *playerInventory, IInventory *dispenserInventoryIn);
    bool canInteractWith(EntityPlayer *playerIn) override;
    ItemStack transferStackInSlot(EntityPlayer *playerIn, int32_t index) override;

  private:
    IInventory *dispenserInventory;
};
