#pragma once
#include "Slot.h"

class SlotFurnaceOutput : public Slot
{
  public:
    SlotFurnaceOutput(EntityPlayer *player, IInventory *inventoryIn, int32_t slotIndex, int32_t xPosition,
                      int32_t yPosition);
    bool isItemValid(ItemStack stack) override;
    ItemStack decrStackSize(int32_t amount) override;
    ItemStack onTake(EntityPlayer *thePlayer, ItemStack stack) override;

  protected:
    void onCrafting(ItemStack stack, int32_t amount) override;
    void onCrafting(ItemStack stack) override;

  private:
    EntityPlayer *player;
    int32_t removeCount;
};
