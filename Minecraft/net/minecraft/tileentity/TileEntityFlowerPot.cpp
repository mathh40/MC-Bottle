#include "TileEntityFlowerPot.h"

#include "datafix/DataFixer.h"

TileEntityFlowerPot::TileEntityFlowerPot(const Item &potItem, int32_t potData)
    : flowerPotItem(potItem), flowerPotData(potData)
{
}

void TileEntityFlowerPot::registerFixesFlowerPot(DataFixer fixer)
{
}

NBTTagCompound *TileEntityFlowerPot::writeToNBT(NBTTagCompound *compound)
{
    TileEntity::writeToNBT(compound);
    ResourceLocation resourcelocation = Item.REGISTRY.getNameForObject(flowerPotItem);
    compound->setString("Item", resourcelocation == nullptr ? "" : resourcelocation.to_string());
    compound->setInteger("Data", flowerPotData);
    return compound;
}

void TileEntityFlowerPot::readFromNBT(NBTTagCompound *compound)
{
    TileEntity::readFromNBT(compound);
    if (compound->hasKey("Item", 8))
    {
        flowerPotItem = Item.getByNameOrId(compound->getString("Item"));
    }
    else
    {
        flowerPotItem = Item.getItemById(compound->getInteger("Item"));
    }

    flowerPotData = compound->getInteger("Data");
}

std::optional<SPacketUpdateTileEntity> TileEntityFlowerPot::getUpdatePacket()
{
    return SPacketUpdateTileEntity(pos, 5, getUpdateTag());
}

NBTTagCompound *TileEntityFlowerPot::getUpdateTag()
{
    return writeToNBT(new NBTTagCompound());
}

void TileEntityFlowerPot::setItemStack(ItemStack stack)
{
    flowerPotItem = stack.getItem();
    flowerPotData = stack.getMetadata();
}

ItemStack TileEntityFlowerPot::getFlowerItemStack()
{
    return flowerPotItem == nullptr ? ItemStack::EMPTY : ItemStack(flowerPotItem, 1, flowerPotData);
}

Item TileEntityFlowerPot::getFlowerPotItem() const
{
    return flowerPotItem;
}

int32_t TileEntityFlowerPot::getFlowerPotData() const
{
    return flowerPotData;
}
