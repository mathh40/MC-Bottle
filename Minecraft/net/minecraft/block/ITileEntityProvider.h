#pragma once
#include <optional>

class ITileEntityProvider
{
  public:
    virtual std::optional<TileEntity> createNewTileEntity(World var1, int32_t var2) = 0;

  protected:
    ~ITileEntityProvider() = default;
};
