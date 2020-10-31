#pragma once
#include "IInventory.h"

class Container;
class RecipeItemHelper;

class InventoryCrafting :public IInventory
{
public:
    InventoryCrafting(Container* eventHandlerIn, int32_t width, int32_t height);
    int32_t getSizeInventory() const override;
    bool isEmpty() const override;
    ItemStack getStackInSlot(int32_t index) override;
    ItemStack getStackInRowAndColumn(int32_t row, int32_t column);
    std::string getName() const override;
    bool hasCustomName() const override;
    ITextComponent* getDisplayName() const override;
    ItemStack removeStackFromSlot(int32_t index) override;
    ItemStack decrStackSize(int32_t index, int32_t count) override;
    void setInventorySlotContents(int32_t index, ItemStack stack) override;
    int32_t getInventoryStackLimit() const override;
    void markDirty() override;
    bool isUsableByPlayer(EntityPlayer* player) override;
    void openInventory(EntityPlayer* player) override;
    void closeInventory(EntityPlayer* player) override;
    bool isItemValidForSlot(int32_t index, ItemStack stack) override;
    int32_t getField(int32_t id) override;
    void setField(int32_t id, int32_t value) override;
    int32_t getFieldCount() override;
    void clear() override;
    int32_t getHeight() const;
    int32_t getWidth() const;
    void fillStackedContents(const RecipeItemHelper& helper);

private:
    std::vector<ItemStack> stackList;
    int32_t inventoryWidth;
    int32_t inventoryHeight;
    Container* eventHandler;
};