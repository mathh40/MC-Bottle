#include "ContainerHorseChest.h"

ContainerHorseChest::ContainerHorseChest(std::string_view inventoryTitle, int32_t slotCount)
    : InventoryBasic(inventoryTitle, false, slotCount)
{
}

ContainerHorseChest::ContainerHorseChest(ITextComponent *inventoryTitle, int32_t slotCount)
    : InventoryBasic(inventoryTitle, slotCount)
{
}
