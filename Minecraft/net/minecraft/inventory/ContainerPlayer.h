#pragma once
#include "Container.h"

class ContainerPlayer : public Container
{
  public:
    ContainerPlayer(InventoryPlayer *playerInventory, bool localWorld, EntityPlayer *playerIn);

    InventoryCrafting *craftMatrix    = new InventoryCrafting(this, 2, 2);
    InventoryCraftResult *craftResult = new InventoryCraftResult();
    bool isLocalWorld;
    void onCraftMatrixChanged(IInventory *inventoryIn) override;
    void onContainerClosed(EntityPlayer *playerIn) override;
    bool canInteractWith(EntityPlayer *playerIn) override;
    ItemStack transferStackInSlot(EntityPlayer *playerIn, int32_t index) override;
    bool canMergeSlot(ItemStack stack, Slot slotIn) override;

  private:
    const static std::array<EntityEquipmentSlot, 4> VALID_EQUIPMENT_SLOTS = {
        EntityEquipmentSlot::HEAD, EntityEquipmentSlot::CHEST, EntityEquipmentSlot::LEGS, EntityEquipmentSlot::FEET};
    EntityPlayer *player;
};
