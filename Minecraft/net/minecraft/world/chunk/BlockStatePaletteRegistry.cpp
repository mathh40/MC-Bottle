#include "BlockStatePaletteRegistry.h"

#include "Block.h"

int32_t BlockStatePaletteRegistry::idFor(IBlockState *state)
{
    auto i = Block::BLOCK_STATE_IDS.get(state);
    return i == -1 ? 0 : i;
}

IBlockState *BlockStatePaletteRegistry::getBlockState(int32_t indexKey)
{
    IBlockState *iblockstate = Block::BLOCK_STATE_IDS.getByValue(indexKey);
    return iblockstate == nullptr ? Blocks::AIR.getDefaultState() : iblockstate;
}

void BlockStatePaletteRegistry::read(PacketBuffer buf)
{
    buf.readVarInt();
}

void BlockStatePaletteRegistry::write(PacketBuffer buf)
{
    buf.writeVarInt(0);
}

int32_t BlockStatePaletteRegistry::getSerializedSize()
{
    return PacketBuffer.getVarIntSize(0);
}
