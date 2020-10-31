#pragma once
#include "Container.h"
#include "../world/ILockableContainer.h"

class InventoryPlayer;

class InventoryLargeChest :public ILockableContainer
{
public:
    InventoryLargeChest(std::string_view nameIn, ILockableContainer* upperChestIn, ILockableContainer* lowerChestIn);
    int32_t getSizeInventory() const override;
    bool isEmpty() const override;
    bool isPartOfLargeChest(IInventory* inventoryIn) const;
    std::string getName() const override;
    bool hasCustomName() const override;
    std::unique_ptr<ITextComponent> getDisplayName() const override;
    ItemStack getStackInSlot(int32_t index) override;
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
    bool isLocked() override;
    void setLockCode(const LockCode& code) const;
    LockCode getLockCode() override;
    std::string getGuiID() const override;
    Container* createContainer(InventoryPlayer* playerInventory, EntityPlayer* playerIn);
    void clear() override;


private:
    std::string name;
    ILockableContainer* upperChest;
    ILockableContainer* lowerChest;
};
