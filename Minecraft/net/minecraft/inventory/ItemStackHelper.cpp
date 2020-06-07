#include "ItemStackHelper.h"

#include "../nbt/NBTTagList.h"

namespace ItemStackHelper
{
    ItemStack getAndSplit(std::vector<ItemStack> &stacks, int32_t index, int32_t amount)
    {
        return index >= 0 && index < stacks.size() && !stacks[index].isEmpty() && amount > 0 ? stacks[index].splitStack(amount) : ItemStack::EMPTY;
    }

    ItemStack getAndRemove(std::vector<ItemStack> &stacks, int32_t index)
    {
        return index >= 0 && index < stacks.size() ? stacks[index] = ItemStack::EMPTY : ItemStack::EMPTY;
    }

    NBTTagCompound * saveAllItems(NBTTagCompound *tag, const std::vector<ItemStack> &list)
    {
        return saveAllItems(tag, list, true);
    }

    NBTTagCompound * saveAllItems(NBTTagCompound *tag, const std::vector<ItemStack> &list, bool saveEmpty)
    {
        NBTTagList* nbttaglist = new NBTTagList();

        for(int i = 0; i < list.size(); ++i) 
        {
            ItemStack itemstack = list[i];
            if (!itemstack.isEmpty()) 
            {
                NBTTagCompound* nbttagcompound = new NBTTagCompound();
                nbttagcompound->setByte("Slot", i);
                itemstack.writeToNBT(nbttagcompound);
                nbttaglist->appendTag(nbttagcompound);
            }
        }

        if (!nbttaglist->isEmpty() || saveEmpty) 
        {
            tag->setTag("Items", nbttaglist);
        }

        return tag;
    }

    void loadAllItems(NBTTagCompound *tag, const std::vector<ItemStack> &list)
    {
        NBTTagList nbttaglist = tag.getTagList("Items", 10);

        for(auto i = 0; i < nbttaglist.tagCount(); ++i) 
        {
            auto nbttagcompound = nbttaglist.getCompoundTagAt(i);
            int32_t j = nbttagcompound.getByte("Slot") & 255;
            if (j >= 0 && j < list.size()) 
            {
                list[j] = ItemStack(nbttagcompound);
            }
        }
    }
}
