#pragma once
#include "Container.h"

class AbstractHorse;
class ContainerHorseInventory : public Container
{
  public:
    ContainerHorseInventory(IInventory *playerInventory, IInventory *horseInventoryIn, const AbstractHorse *horse,
                            EntityPlayer *player);
    bool canInteractWith(EntityPlayer *playerIn) override;
    ItemStack transferStackInSlot(EntityPlayer *playerIn, int32_t index) override;
    void onContainerClosed(EntityPlayer *playerIn) override;

  private:
    IInventory *horseInventory;
    const AbstractHorse *horse;
};
