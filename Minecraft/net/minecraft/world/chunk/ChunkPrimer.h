#pragma once

class IBlockState;
class ChunkPrimer
{
  public:
    IBlockState *getBlockState(int32_t x, int32_t y, int32_t z);
    void setBlockState(int32_t x, int32_t y, int32_t z, IBlockState *state);
    int32_t findGroundBlockIdx(int32_t x, int32_t z);

  private:
    static IBlockState *DEFAULT_STATE;
    std::array<char, 65536> data;

    static int32_t getBlockIndex(int32_t x, int32_t y, int32_t z);
};
