#pragma once
#include "../item/ItemStack.h"

namespace ItemStackHelper
{
    ItemStack getAndSplit(std::vector<ItemStack>& stacks, int32_t index, int32_t amount);
    ItemStack getAndRemove(std::vector<ItemStack>& stacks, int32_t index);
    NBTTagCompound* saveAllItems(NBTTagCompound* tag, const std::vector<ItemStack>& list);
    NBTTagCompound* saveAllItems(NBTTagCompound* tag, const std::vector<ItemStack>& list, bool saveEmpty);
    void loadAllItems(NBTTagCompound* tag, const std::vector<ItemStack>& list);
}
