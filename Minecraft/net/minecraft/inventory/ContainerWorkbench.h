#pragma once
#include "Container.h"

class ContainerWorkbench : public Container
{
  public:
    ContainerWorkbench(InventoryPlayer *playerInventory, World *worldIn, const BlockPos &posIn);
    void onCraftMatrixChanged(IInventory *inventoryIn) override;
    void onContainerClosed(EntityPlayer *playerIn) override;
    bool canInteractWith(EntityPlayer *playerIn) override;
    ItemStack transferStackInSlot(EntityPlayer *playerIn, int32_t index) override;

    InventoryCrafting *craftMatrix;
    InventoryCraftResult *craftResult;

  private:
    World *world;
    BlockPos pos;
    EntityPlayer *player;
};
