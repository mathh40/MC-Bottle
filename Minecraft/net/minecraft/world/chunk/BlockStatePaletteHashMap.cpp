#include "BlockStatePaletteHashMap.h"

#include "../../block/Block.h"

BlockStatePaletteHashMap::BlockStatePaletteHashMap(int32_t bitsIn, IBlockStatePaletteResizer *paletteResizerIn)
    : bits(bitsIn), paletteResizer(paletteResizerIn), statePaletteMap(1 << bitsIn)
{
}

int32_t BlockStatePaletteHashMap::idFor(IBlockState *state)
{
    int32_t i = statePaletteMap.getId(state);
    if (i == -1)
    {
        i = statePaletteMap.add(state);
        if (i >= 1 << bits)
        {
            i = paletteResizer->onResize(bits + 1, state);
        }
    }

    return i;
}

IBlockState *BlockStatePaletteHashMap::getBlockState(int32_t indexKey)
{
    return statePaletteMap.get(indexKey).value();
}

void BlockStatePaletteHashMap::read(PacketBuffer buf)
{
    statePaletteMap.clear();
    auto i = buf.readVarInt();

    for (auto j = 0; j < i; ++j)
    {
        statePaletteMap.add(Block::BLOCK_STATE_IDS.getByValue(buf.readVarInt()));
    }
}

void BlockStatePaletteHashMap::write(PacketBuffer buf)
{
    auto i = statePaletteMap.size();
    buf.writeVarInt(i);

    for (auto j = 0; j < i; ++j)
    {
        buf.writeVarInt(Block::BLOCK_STATE_IDS.get(statePaletteMap.get(j)));
    }
}

int32_t BlockStatePaletteHashMap::getSerializedSize()
{
    auto i = PacketBuffer.getVarIntSize(statePaletteMap.size());

    for (auto j = 0; j < statePaletteMap.size(); ++j)
    {
        i += PacketBuffer.getVarIntSize(Block::BLOCK_STATE_IDS.get(statePaletteMap.get(j)));
    }

    return i;
}
