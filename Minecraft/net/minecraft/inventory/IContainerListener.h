#pragma once
#include <cstdint>
#include <vector>
class Container;
class ItemStack;
class IInventory;
class IContainerListener
{
public:
    virtual void sendAllContents(const Container& var1, std::vector<Slot> var2) = 0;

    virtual void sendSlotContents(const Container& var1, int32_t var2, const ItemStack& var3) = 0;

    virtual void sendWindowProperty(const Container& var1, int32_t var2, int32_t var3) = 0;

    virtual void sendAllWindowProperties(const Container& var1, const IInventory* var2) = 0;
};