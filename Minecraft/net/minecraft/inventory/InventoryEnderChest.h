#pragma once
#include "InventoryBasic.h"

class NBTTagList;
class TileEntityEnderChest;

class InventoryEnderChest :public InventoryBasic
{
public:
    InventoryEnderChest();
    void setChestTileEntity(TileEntityEnderChest* chestTileEntity);
    void loadInventoryFromNBT(NBTTagList* p_70486_1_);
    NBTTagList* saveInventoryToNBT() const;
    bool isUsableByPlayer(EntityPlayer* player) override;
    void openInventory(EntityPlayer* player) override;
    void closeInventory(EntityPlayer* player) override;

private:
    TileEntityEnderChest* associatedChest;
};