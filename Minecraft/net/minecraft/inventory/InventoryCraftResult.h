#pragma once
#include "IInventory.h"

class IRecipe;

class InventoryCraftResult :public IInventory
{
public:
    InventoryCraftResult();
    int32_t getSizeInventory() override;
    bool isEmpty() override;
    ItemStack getStackInSlot(int32_t index) override;
    std::string getName() const override;
    bool hasCustomName() const override;
    std::unique_ptr<ITextComponent> getDisplayName() const override;
    ItemStack decrStackSize(int32_t index, int32_t count) override;
    ItemStack removeStackFromSlot(int32_t index) override;
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
    void setRecipeUsed(IRecipe* p_193056_1_);
    IRecipe* getRecipeUsed() const;


private:
    std::vector<ItemStack> stackResult;
    IRecipe* recipeUsed;
};
