#include "ChunkPrimer.h"

#include "Block.h"

IBlockState *ChunkPrimer::DEFAULT_STATE = Blocks.AIR.getDefaultState();

IBlockState *ChunkPrimer::getBlockState(int32_t x, int32_t y, int32_t z)
{
    auto iblockstate = Block::BLOCK_STATE_IDS.getByValue(data[getBlockIndex(x, y, z)]);
    return iblockstate == nullptr ? DEFAULT_STATE : iblockstate;
}

void ChunkPrimer::setBlockState(int32_t x, int32_t y, int32_t z, IBlockState *state)
{
    data[getBlockIndex(x, y, z)] = Block::BLOCK_STATE_IDS.get(state);
}

int32_t ChunkPrimer::findGroundBlockIdx(int32_t x, int32_t z)
{
    auto i = (x << 12 | z << 8) + 256 - 1;

    for (auto j = 255; j >= 0; --j)
    {
        auto iblockstate = Block::BLOCK_STATE_IDS.getByValue(data[i + j]);
        if (iblockstate != nullptr && iblockstate != DEFAULT_STATE)
        {
            return j;
        }
    }

    return 0;
}

int32_t ChunkPrimer::getBlockIndex(int32_t x, int32_t y, int32_t z)
{
    return x << 12 | z << 8 | y;
}
