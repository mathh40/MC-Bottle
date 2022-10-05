#include "TileEntityBed.h"

TileEntityBed::TileEntityBed() : color(EnumDyeColor::RED)
{
}

void TileEntityBed::setItemValues(ItemStack p_193051_1_)
{
    setColor(EnumDyeColor::byMetadata(p_193051_1_.getMetadata()));
}

void TileEntityBed::readFromNBT(NBTTagCompound *compound)
{
    TileEntity::readFromNBT(compound);
    if (compound->hasKey("color"))
    {
        color = EnumDyeColor::byMetadata(compound->getInteger("color"));
    }
}

NBTTagCompound *TileEntityBed::writeToNBT(NBTTagCompound *compound)
{
    TileEntity::writeToNBT(compound);
    compound->setInteger("color", color.getMetadata());
    return compound;
}

NBTTagCompound *TileEntityBed::getUpdateTag()
{
    return writeToNBT(new NBTTagCompound());
}

std::optional<SPacketUpdateTileEntity> TileEntityBed::getUpdatePacket()
{
    return SPacketUpdateTileEntity(pos, 11, getUpdateTag());
}

EnumDyeColor TileEntityBed::getColor() const
{
    return color;
}

void TileEntityBed::setColor(EnumDyeColor color)
{
    color = color;
    markDirty();
}

bool TileEntityBed::isHeadPiece()
{
    return BlockBed::isHeadPiece(getBlockMetadata());
}

ItemStack TileEntityBed::getItemStack()
{
    return ItemStack(Items::BED, 1, color.getMetadata());
}
