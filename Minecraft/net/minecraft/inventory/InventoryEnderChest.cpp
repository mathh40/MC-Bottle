#include "InventoryEnderChest.h"

#include "../nbt/NBTTagList.h"
#include "../tileentity/TileEntityEnderChest.h"

InventoryEnderChest::InventoryEnderChest() : InventoryBasic("container.enderchest", false, 27)
{
}

void InventoryEnderChest::setChestTileEntity(TileEntityEnderChest *chestTileEntity)
{
    associatedChest = chestTileEntity;
}

void InventoryEnderChest::loadInventoryFromNBT(NBTTagList *p_70486_1_)
{
    for (auto k = 0; k < getSizeInventory(); ++k)
    {
        setInventorySlotContents(k, ItemStack::EMPTY);
    }

    for (auto k = 0; k < p_70486_1_->tagCount(); ++k)
    {
        std::shared_ptr<NBTTagCompound> nbttagcompound = p_70486_1_->getCompoundTagAt(k);
        auto j                                         = nbttagcompound->getByte("Slot") & 255;
        if (j >= 0 && j < getSizeInventory())
        {
            setInventorySlotContents(j, ItemStack(nbttagcompound));
        }
    }
}

NBTTagList *InventoryEnderChest::saveInventoryToNBT() const
{
    NBTTagList *nbttaglist = new NBTTagList();

    for (auto i = 0; i < getSizeInventory(); ++i)
    {
        ItemStack itemstack = getStackInSlot(i);
        if (!itemstack.isEmpty())
        {
            NBTTagCompound *nbttagcompound = new NBTTagCompound();
            nbttagcompound->setByte("Slot", i);
            itemstack.writeToNBT(nbttagcompound);
            nbttaglist->appendTag(nbttagcompound);
        }
    }

    return nbttaglist;
}

bool InventoryEnderChest::isUsableByPlayer(EntityPlayer *player)
{
    return associatedChest != nullptr && !associatedChest->canBeUsed(player) ? false
                                                                             : InventoryBasic::isUsableByPlayer(player);
}

void InventoryEnderChest::openInventory(EntityPlayer *player)
{
    if (associatedChest != nullptr)
    {
        associatedChest->openChest();
    }

    InventoryBasic::openInventory(player);
}

void InventoryEnderChest::closeInventory(EntityPlayer *player)
{
    if (associatedChest != nullptr)
    {
        associatedChest->closeChest();
    }

    InventoryBasic::closeInventory(player);
    associatedChest = nullptr;
}
