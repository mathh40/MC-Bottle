#pragma once
#include <memory>

#include "Container.h"
#include "../../../../spdlog/include/spdlog/logger.h"

class ContainerRepair :public Container
{
public:
    ContainerRepair(InventoryPlayer* playerInventory, World* worldIn, EntityPlayer* player);
    ContainerRepair(InventoryPlayer* playerInventory, const World* worldIn, const BlockPos& blockPosIn, EntityPlayer* player);

    void onCraftMatrixChanged(IInventory* inventoryIn) override;
    void updateRepairOutput();
    void addListener(IContainerListener* listener) override;
    void updateProgressBar(int32_t id, int32_t data) override;
    void onContainerClosed(EntityPlayer* playerIn) override;
    bool canInteractWith(EntityPlayer* playerIn) override;
    ItemStack transferStackInSlot(EntityPlayer* playerIn, int32_t index) override;
    void updateItemName(std::string_view newName);


    int32_t maximumCost;
private:
    static std::shared_ptr<spdlog::logger> LOGGER;

    IInventory* outputSlot;
    IInventory* inputSlots;
    World* world;
    BlockPos pos;
    int32_t materialCost;
    std::string repairedItemName;
    EntityPlayer* player;
};
