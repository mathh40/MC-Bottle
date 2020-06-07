#pragma once
#include "Container.h"

class ContainerEnchantment :public Container
{
public:
    ContainerEnchantment(InventoryPlayer* playerInv, World* worldIn);
    ContainerEnchantment(InventoryPlayer* playerInv, World* worldIn, BlockPos pos);
    void addListener(IContainerListener* listener) override;
    void detectAndSendChanges() override;
    void updateProgressBar(int32_t id, int32_t data) override;
    void onCraftMatrixChanged(IInventory* inventoryIn) override;
    bool enchantItem(EntityPlayer* playerIn, int32_t id) override;
    int32_t getLapisAmount() const;
    void onContainerClosed(EntityPlayer* playerIn) override;
    bool canInteractWith(EntityPlayer* playerIn) override;
    ItemStack transferStackInSlot(EntityPlayer* playerIn, int32_t index) override;

    IInventory* tableInventory;
    int32_t xpSeed;
    std::vector<int32_t> enchantLevels;
    std::vector<int32_t> enchantClue;
    std::vector<int32_t> worldClue;
protected:
    void broadcastData(IContainerListener* crafting);
private:
    std::vector<> getEnchantmentList(ItemStack stack, int32_t enchantSlot, int32_t level);

    World* world;
    BlockPos position;
    pcg32 rand;
};
