#pragma once
#include "Slot.h"

class MerchantRecipe;
class IMerchant;
class InventoryMerchant;

class SlotMerchantResult : public Slot
{
  public:
    SlotMerchantResult(EntityPlayer *player, IMerchant *merchant, InventoryMerchant *merchantInventory,
                       int32_t slotIndex, int32_t xPosition, int32_t yPosition);
    bool isItemValid(ItemStack stack) override;
    ItemStack decrStackSize(int32_t amount) override;
    ItemStack onTake(EntityPlayer *thePlayer, ItemStack stack) override;

  protected:
    void onCrafting(ItemStack stack, int32_t amount) override;
    void onCrafting(ItemStack stack) override;

  private:
    bool doTrade(const MerchantRecipe &trade, ItemStack firstItem, ItemStack secondItem);

    InventoryMerchant *merchantInventory;
    EntityPlayer *player;
    int removeCount;
    IMerchant *merchant;
};
