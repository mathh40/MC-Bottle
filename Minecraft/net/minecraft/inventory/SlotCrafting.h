#pragma once
#include "Slot.h"

class InventoryCrafting;

class SlotCrafting : public Slot
{
  public:
    SlotCrafting(EntityPlayer *player, InventoryCrafting *craftingInventory, IInventory *inventoryIn, int32_t slotIndex,
                 int32_t xPosition, int32_t yPosition);
    bool isItemValid(ItemStack stack) override;
    ItemStack decrStackSize(int32_t amount);
    ItemStack onTake(EntityPlayer *thePlayer, ItemStack stack) override;

  protected:
    void onCrafting(ItemStack stack, int32_t amount) override;
    void onSwapCraft(int32_t p_190900_1_) override;
    void onCrafting(ItemStack stack) override;

  private:
    InventoryCrafting *craftMatrix;
    EntityPlayer *player;
    int32_t amountCrafted;
};
