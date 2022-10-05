#pragma once
#include "Slot.h"

class SlotFurnaceFuel : public Slot
{
  public:
    SlotFurnaceFuel(IInventory *inventoryIn, int32_t slotIndex, int32_t xPosition, int32_t yPosition);
    bool isItemValid(ItemStack stack) override;
    int32_t getItemStackLimit(ItemStack stack);
    static bool isBucket(ItemStack stack);
};
