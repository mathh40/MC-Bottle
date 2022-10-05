#pragma once
#include "IInventory.h"

class IInventoryChangedListener;

class InventoryBasic : public IInventory
{
  public:
    InventoryBasic(std::string_view title, bool customName, int32_t slotCount);
    InventoryBasic(ITextComponent *title, int32_t slotCount);
    void addInventoryChangeListener(IInventoryChangedListener *listener);
    void removeInventoryChangeListener(IInventoryChangedListener *listener);
    ItemStack getStackInSlot(int32_t index) override;
    ItemStack decrStackSize(int32_t index, int32_t count) override;
    ItemStack addItem(ItemStack stack);
    ItemStack removeStackFromSlot(int32_t index) override;
    void setInventorySlotContents(int32_t index, ItemStack stack) override;
    int32_t getSizeInventory() const override;
    bool isEmpty() const override;
    std::string getName() const override;
    bool hasCustomName() const override;
    void setCustomName(std::string_view inventoryTitleIn);
    ITextComponent *getDisplayName() const override;
    int32_t getInventoryStackLimit() const override;
    void markDirty() override;
    bool isUsableByPlayer(EntityPlayer *player) override;
    void openInventory(EntityPlayer *player) override;
    void closeInventory(EntityPlayer *player) override;
    bool isItemValidForSlot(int32_t index, ItemStack stack) override;
    int32_t getField(int32_t id) override;
    void setField(int32_t id, int32_t value) override;
    int32_t getFieldCount() override;
    void clear() override;

  private:
    std::string inventoryTitle;
    int32_t slotsCount;
    std::vector<ItemStack> inventoryContents;
    std::vector<IInventoryChangedListener *> changeListeners;
    bool bhasCustomName;
};
