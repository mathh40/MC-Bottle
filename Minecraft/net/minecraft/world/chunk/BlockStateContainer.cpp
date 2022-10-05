#include "BlockStateContainer.h"

#include "../../block/Block.h"
#include "../../util/math/MathHelper.h"

IBlockState *BlockStateContainer::AIR_BLOCK_STATE = Blocks.AIR.getDefaultState();

BlockStateContainer::BlockStateContainer() : bits(4)
{
}

int32_t BlockStateContainer::onResize(int32_t bits, IBlockState *state)
{
    auto &bitarray          = storage;
    auto iblockstatepalette = palette;
    setBits(bits);

    for (auto i = 0; i < bitarray.size(); ++i)
    {
        auto iblockstate = iblockstatepalette->getBlockState(bitarray.getAt(i));
        if (iblockstate != nullptr)
        {
            set(i, iblockstate);
        }
    }

    return palette->idFor(state);
}

void BlockStateContainer::set(int32_t x, int32_t y, int32_t z, IBlockState *state)
{
    set(getIndex(x, y, z), state);
}

IBlockState *BlockStateContainer::get(int32_t x, int32_t y, int32_t z)
{
    return get(getIndex(x, y, z));
}

void BlockStateContainer::set(int32_t index, IBlockState *state)
{
    auto i = palette->idFor(state);
    storage.setAt(index, i);
}

IBlockState *BlockStateContainer::get(int32_t index)
{
    IBlockState *iblockstate = palette->getBlockState(storage.getAt(index));
    return iblockstate == nullptr ? AIR_BLOCK_STATE : iblockstate;
}

int32_t BlockStateContainer::getIndex(int32_t x, int32_t y, int32_t z)
{
    return y << 8 | z << 4 | x;
}

void BlockStateContainer::setBits(int32_t bitsIn)
{
    if (bitsIn != bits)
    {
        bits = bitsIn;
        if (bits <= 4)
        {
            bits    = 4;
            palette = new BlockStatePaletteLinear(bits, this);
        }
        else if (bits <= 8)
        {
            palette = new BlockStatePaletteHashMap(bits, this);
        }
        else
        {
            palette = REGISTRY_BASED_PALETTE;
            bits    = MathHelper::log2DeBruijn(Block::BLOCK_STATE_IDS.size());
        }

        palette->idFor(AIR_BLOCK_STATE);
        storage = BitArray(bits, 4096);
    }
}

void BlockStateContainer::read(PacketBuffer buf)
{
    auto i = buf.readByte();
    if (bits != i)
    {
        setBits(i);
    }

    palette->read(buf);
    buf.readLongArray(storage.getBackingLongArray());
}

void BlockStateContainer::write(PacketBuffer buf)
{
    buf.writeByte(bits);
    palette->write(buf);
    buf.writeLongArray(storage.getBackingLongArray());
}

NibbleArray BlockStateContainer::getDataForNBT(std::vector<unsigned char> blockIds, NibbleArray data)
{
    NibbleArray nibblearray;

    for (auto i = 0; i < 4096; ++i)
    {
        auto j  = Block::BLOCK_STATE_IDS.find(get(i))->second;
        auto k  = i & 15;
        auto l  = i >> 8 & 15;
        auto i1 = i >> 4 & 15;
        if ((j >> 12 & 15) != 0)
        {
            nibblearray.set(k, l, i1, j >> 12 & 15);
        }

        blockIds[i] = j >> 4 & 255;
        data.set(k, l, i1, j & 15);
    }

    return nibblearray;
}

void BlockStateContainer::setDataFromNBT(std::vector<unsigned char> blockIds, NibbleArray data,
                                         std::optional<NibbleArray> blockIdExtension)
{
    for (auto i = 0; i < 4096; ++i)
    {
        auto j  = i & 15;
        auto k  = i >> 8 & 15;
        auto l  = i >> 4 & 15;
        auto i1 = blockIdExtension == std::nullopt ? 0 : blockIdExtension.get(j, k, l);
        auto j1 = i1 << 12 | (blockIds[i] & 255) << 4 | data.get(j, k, l);
        set(i, Block::BLOCK_STATE_IDS.getByValue(j1));
    }
}

int32_t BlockStateContainer::getSerializedSize()
{
    return 1 + palette->getSerializedSize() + PacketBuffer.getVarIntSize(storage.size()) +
           storage.getBackingLongArray().size() * 8;
}
