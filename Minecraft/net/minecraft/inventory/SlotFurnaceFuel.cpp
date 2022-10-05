#include "SlotFurnaceFuel.h"

#include "../item/ItemStack.h"
#include "../tileentity/TileEntityFurnace.h"

SlotFurnaceFuel::SlotFurnaceFuel(IInventory *inventoryIn, int32_t slotIndex, int32_t xPosition, int32_t yPosition)
    : Slot(inventoryIn, slotIndex, xPosition, yPosition)
{
}

bool SlotFurnaceFuel::isItemValid(ItemStack stack)
{
    return TileEntityFurnace::isItemFuel(stack) || isBucket(stack);
}

int32_t SlotFurnaceFuel::getItemStackLimit(ItemStack stack)
{
    return isBucket(stack) ? 1 : Slot::getItemStackLimit(stack);
}

bool SlotFurnaceFuel::isBucket(ItemStack stack)
{
    return stack.getItem() == Items::BUCKET;
}
