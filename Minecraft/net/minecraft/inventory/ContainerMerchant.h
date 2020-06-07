#pragma once
#include "Container.h"

class IMerchant;
class InventoryMerchant;
class ContainerMerchant :public Container
{
public:
    ContainerMerchant(InventoryPlayer* playerInventory, IMerchant* merchant, World* worldIn);
    InventoryMerchant* getMerchantInventory() const;
    void onCraftMatrixChanged(IInventory* inventoryIn) override;
    void setCurrentRecipeIndex(int32_t currentRecipeIndex);
    bool canInteractWith(EntityPlayer* playerIn) override;
    ItemStack transferStackInSlot(EntityPlayer* playerIn, int32_t index) override;
    void onContainerClosed(EntityPlayer* playerIn) override;

private:
    IMerchant* merchant;
    InventoryMerchant* merchantInventory;
    World* world;
};