#pragma once
#include <cstdint>
class IBlockState;

class IBlockStatePaletteResizer
{
  public:
    virtual ~IBlockStatePaletteResizer()                      = default;
    virtual int32_t onResize(int32_t var1, IBlockState *var2) = 0;
};
