#pragma once
#include "Container.h"

class ContainerShulkerBox : public Container
{
  public:
    ContainerShulkerBox(InventoryPlayer *playerInventoryIn, IInventory *inventoryIn, EntityPlayer *player);
    bool canInteractWith(EntityPlayer *playerIn) override;
    ItemStack transferStackInSlot(EntityPlayer *playerIn, int32_t index) override;
    void onContainerClosed(EntityPlayer *playerIn) override;

  private:
    IInventory *inventory;
};
