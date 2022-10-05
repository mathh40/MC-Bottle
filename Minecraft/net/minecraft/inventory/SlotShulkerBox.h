#pragma once
#include "Slot.h"

class SlotShulkerBox : public Slot
{
  public:
    SlotShulkerBox(IInventory *p_i47265_1_, int32_t slotIndexIn, int32_t xPosition, int32_t yPosition);
    bool isItemValid(ItemStack stack) override;
};
