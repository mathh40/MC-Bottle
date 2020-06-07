#pragma once
#include <string_view>

class ITextComponent;

class ContainerHorseChest :private InventoryBasic
{
public:
    ContainerHorseChest(std::string_view inventoryTitle, int32_t slotCount);
    ContainerHorseChest(ITextComponent* inventoryTitle, int32_t slotCount);
};
