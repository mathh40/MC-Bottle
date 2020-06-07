#pragma once
#include "Container.h"

class PotionSlot :public Slot
{
public:
    PotionSlot(IInventory* p_i47598_1_, int32_t p_i47598_2_, int32_t p_i47598_3_, int32_t p_i47598_4_);
    bool isItemValid(ItemStack stack) override;
    int32_t getSlotStackLimit() const override;
    ItemStack onTake(EntityPlayer* thePlayer, ItemStack stack) override;
    bool canHoldPotion(ItemStack stack);
};

class IngredientSlot :public Slot
{
public:
    IngredientSlot(IInventory* iInventoryIn, int32_t index, int32_t xPosition, int32_t yPosition);
    bool isItemValid(ItemStack stack) override;
    int32_t getSlotStackLimit() const override;
};

class FuelSlot :public Slot
{
public:
    FuelSlot(IInventory* iInventoryIn, int32_t index, int32_t xPosition, int32_t yPosition);
    bool isItemValid(ItemStack stack) override;
    bool isValidBrewingFuel(ItemStack itemStackIn);
    int32_t getSlotStackLimit() const override;
};

class ContainerBrewingStand :public Container
{
public:
    ContainerBrewingStand(InventoryPlayer* playerInventory, IInventory* tileBrewingStandIn);
    void addListener(IContainerListener* listener) override;
    void detectAndSendChanges() override;
    void updateProgressBar(int32_t id, int32_t data) override;
    bool canInteractWith(EntityPlayer* playerIn) override;
    ItemStack transferStackInSlot(EntityPlayer* playerIn, int32_t index) override;


private:
    IInventory* tileBrewingStand;
    Slot slot;
    int32_t prevBrewTime;
    int32_t prevFuel;
};