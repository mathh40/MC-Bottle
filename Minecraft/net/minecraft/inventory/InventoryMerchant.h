#pragma once
#include "../village/MerchantRecipe.h"
#include "IInventory.h"

class IMerchant;

class InventoryMerchant : public IInventory
{
  public:
    InventoryMerchant(EntityPlayer *thePlayerIn, IMerchant *theMerchantIn);
    int32_t getSizeInventory() const override;
    bool isEmpty() const override;
    ItemStack getStackInSlot(int32_t index) override;
    ItemStack decrStackSize(int32_t index, int32_t count) override;
    ItemStack removeStackFromSlot(int32_t index) override;
    void setInventorySlotContents(int32_t index, ItemStack stack) override;
    std::string getName() const override;
    bool hasCustomName() const override;
    std::unique_ptr<ITextComponent> getDisplayName() const override;
    int32_t getInventoryStackLimit() const override;
    bool isUsableByPlayer(EntityPlayer *player) override;
    void openInventory(EntityPlayer *player) override;
    void closeInventory(EntityPlayer *player) override;
    bool isItemValidForSlot(int32_t index, ItemStack stack) override;
    void markDirty() override;
    void resetRecipeAndSlots();
    MerchantRecipe getCurrentRecipe() const;
    void setCurrentRecipeIndex(int32_t currentRecipeIndexIn);
    int32_t getField(int32_t id) override;
    void setField(int32_t id, int32_t value) override;
    int32_t getFieldCount() override;
    void clear() override;

  private:
    bool inventoryResetNeededOnSlotChange(int32_t slotIn);

    IMerchant *merchant;
    std::vector<ItemStack> slots;
    EntityPlayer *player;
    MerchantRecipe currentRecipe;
    int32_t currentRecipeIndex;
};
