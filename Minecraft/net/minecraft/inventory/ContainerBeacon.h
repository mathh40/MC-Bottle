#pragma once
#include "Container.h"

class BeaconSlot : public Slot
{
  public:
    BeaconSlot(IInventory *inventoryIn, int32_t index, int32_t xIn, int32_t yIn);
    bool isItemValid(ItemStack stack) override;
    int32_t getSlotStackLimit() const override;

  private:
};

class ContainerBeacon : public Container
{
  public:
    ContainerBeacon(IInventory *playerInventory, IInventory *tileBeaconIn);
    void addListener(IContainerListener *listener) override;
    void updateProgressBar(int32_t id, int32_t data) override;
    IInventory *getTileEntity() const;
    void onContainerClosed(EntityPlayer *playerIn) override;
    bool canInteractWith(EntityPlayer *playerIn) override;
    ItemStack transferStackInSlot(EntityPlayer *playerIn, int32_t index) override;

  private:
    IInventory *tileBeacon;
    BeaconSlot beaconSlot;
};
