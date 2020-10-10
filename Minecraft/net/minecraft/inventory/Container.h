#pragma once
#include <cstdint>
#include <unordered_set>
#include "ClickType.h"
#include "Slot.h"
#include "../item/ItemStack.h"

class InventoryCrafting;
class InventoryCraftResult;
class InventoryPlayer;
class IInventory;
class IContainerListener;

class Container
{
public:
    virtual void addListener(IContainerListener* listener);
    void removeListener(IContainerListener* listener);
    std::vector<Slot> getInventory();
    virtual void detectAndSendChanges();
    virtual bool enchantItem(EntityPlayer* playerIn, int32_t id);
    std::optional<Slot> getSlotFromInventory(const IInventory& inv, int32_t slotIn);
    Slot getSlot(int32_t slotId);
    virtual ItemStack transferStackInSlot(EntityPlayer* playerIn, int32_t index);
    ItemStack slotClick(int32_t slotId, int32_t dragType, ClickType clickTypeIn, EntityPlayer* player);
    virtual bool canMergeSlot(ItemStack stack, Slot slotIn);
    virtual void onContainerClosed(EntityPlayer* playerIn);
    virtual void onCraftMatrixChanged(IInventory* inventoryIn);
    void putStackInSlot(int32_t slotID, ItemStack stack);
    void setAll(std::vector<ItemStack>& p_190896_1_);
    virtual void updateProgressBar(int32_t id, int32_t data);
    int16_t getNextTransactionID(InventoryPlayer* invPlayer);
    bool getCanCraft(EntityPlayer* player);
    void setCanCraft(EntityPlayer* player, bool canCraft);
    virtual bool canInteractWith(EntityPlayer* var1) = 0;
    int32_t extractDragMode(int32_t eventButton);
    int32_t getDragEvent(int32_t clickedButton);
    int32_t getQuickcraftMask(int32_t p_94534_0_, int32_t p_94534_1_);
    bool isValidDragMode(int32_t dragModeIn, EntityPlayer* player);
    bool canAddItemToSlot(std::optional<Slot> slotIn, ItemStack stack, bool stackSizeMatters);
    void computeStackSize(std::unordered_set<Slot>& dragSlotsIn, int32_t dragModeIn, ItemStack stack, int32_t slotStackSize);
    bool canDragIntoSlot(Slot slotIn);
    int32_t calcRedstone(TileEntity* te) const;
    static int32_t calcRedstoneFromInventory(IInventory* inv);
    virtual ~Container();

    std::vector<ItemStack> inventoryItemStacks;
    std::vector<std::optional<Slot>> inventorySlots;
    int32_t windowId;
protected:
    Slot addSlotToContainer(const Slot& slotIn);
    void clearContainer(EntityPlayer* playerIn, World* worldIn, IInventory* inventoryIn) const;
    bool mergeItemStack(ItemStack stack, int32_t startIndex, int32_t endIndex, bool reverseDirection);
    void resetDrag();
    void slotChangedCraftingGrid(World* p_192389_1_, EntityPlayer* p_192389_2_, InventoryCrafting* p_192389_3_, InventoryCraftResult* p_192389_4_);

    std::vector<IContainerListener*> listeners;
private:
    int16_t transactionID;
    int32_t dragMode = -1;
    int32_t dragEvent;
    std::unordered_set<Slot> dragSlots;

    std::unordered_set<EntityPlayer*> playerList;
};
