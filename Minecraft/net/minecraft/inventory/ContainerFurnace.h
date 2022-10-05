#pragma once
#include "Container.h"

class ContainerFurnace : public Container
{
  public:
    ContainerFurnace(InventoryPlayer *playerInventory, IInventory *furnaceInventory);
    void addListener(IContainerListener *listener) override;
    void detectAndSendChanges() override;
    void updateProgressBar(int32_t id, int32_t data) override;
    bool canInteractWith(EntityPlayer *playerIn) override;
    ItemStack transferStackInSlot(EntityPlayer *playerIn, int32_t index) override;

  private:
    IInventory *tileFurnace;
    int32_t cookTime;
    int32_t totalCookTime;
    int32_t furnaceBurnTime;
    int32_t currentItemBurnTime;
};
