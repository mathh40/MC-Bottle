#include "TileEntityNote.h"

NBTTagCompound *TileEntityNote::writeToNBT(NBTTagCompound *compound)
{
    TileEntity::writeToNBT(compound);
    compound->setByte("note", note);
    compound->setBoolean("powered", previousRedstoneState);
    return compound;
}

void TileEntityNote::readFromNBT(NBTTagCompound *compound)
{
    TileEntity::readFromNBT(compound);
    note                  = compound->getByte("note");
    note                  = MathHelper::clamp<uint8_t>(note, 0, 24);
    previousRedstoneState = compound->getBoolean("powered");
}

void TileEntityNote::changePitch()
{
    note = (note + 1) % 25;
    markDirty();
}

void TileEntityNote::triggerNote(World *worldIn, BlockPos posIn)
{
    if (worldIn->getBlockState(posIn.up())->getMaterial() == Material::AIR)
    {
        IBlockState *iblockstate = worldIn->getBlockState(posIn.down());
        Material material        = iblockstate->getMaterial();
        int i                    = 0;
        if (material == Material::ROCK)
        {
            i = 1;
        }

        if (material == Material::SAND)
        {
            i = 2;
        }

        if (material == Material::GLASS)
        {
            i = 3;
        }

        if (material == Material::WOOD)
        {
            i = 4;
        }

        Block block = iblockstate->getBlock();
        if (block == Blocks::CLAY)
        {
            i = 5;
        }

        if (block == Blocks::GOLD_BLOCK)
        {
            i = 6;
        }

        if (block == Blocks::WOOL)
        {
            i = 7;
        }

        if (block == Blocks::PACKED_ICE)
        {
            i = 8;
        }

        if (block == Blocks::BONE_BLOCK)
        {
            i = 9;
        }

        worldIn->addBlockEvent(posIn, Blocks::NOTEBLOCK, i, note);
    }
}
