#include "BlockStatePaletteLinear.h"

#include "Block.h"

BlockStatePaletteLinear::BlockStatePaletteLinear(int32_t bitsIn, IBlockStatePaletteResizer *resizeHandlerIn)
    : bits(bitsIn), resizeHandler(resizeHandlerIn)
{
    states.resize(1 << bitsIn);
}

int32_t BlockStatePaletteLinear::idFor(IBlockState *state)
{
    for (auto j = 0; j < arraySize; ++j)
    {
        if (states[j] == state)
        {
            return j;
        }
    }

    auto j = arraySize;
    if (j < states.size())
    {
        states[j] = state;
        ++arraySize;
        return j;
    }
    else
    {
        return resizeHandler->onResize(bits + 1, state);
    }
}

IBlockState *BlockStatePaletteLinear::getBlockState(int32_t indexKey)
{
    return indexKey >= 0 && indexKey < arraySize ? states[indexKey] : nullptr;
}

void BlockStatePaletteLinear::read(PacketBuffer buf)
{
    arraySize = buf.readVarInt();

    for (auto i = 0; i < arraySize; ++i)
    {
        states[i] = Block::BLOCK_STATE_IDS.getByValue(buf.readVarInt());
    }
}

void BlockStatePaletteLinear::write(PacketBuffer buf)
{
    buf.writeVarInt(arraySize);

    for (int i = 0; i < arraySize; ++i)
    {
        buf.writeVarInt(Block::BLOCK_STATE_IDS.get(states[i]));
    }
}

int32_t BlockStatePaletteLinear::getSerializedSize()
{
    auto i = PacketBuffer.getVarIntSize(arraySize);

    for (auto j = 0; j < arraySize; ++j)
    {
        i += PacketBuffer.getVarIntSize(Block::BLOCK_STATE_IDS.get(states[j]));
    }

    return i;
}
