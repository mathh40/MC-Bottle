#pragma once
#include "../block/BlockAnvil.h"
#include "../block/state/IBlockState.h"
#include "WorldType.h"
#include "biome/Biome.h"
class BlockPos;
class EnumFacing;
class TileEntity;

class IBlockAccess
{
  public:
    virtual ~IBlockAccess() = default;

    virtual TileEntity *getTileEntity(const BlockPos &var1) = 0;

    virtual uint32_t getCombinedLight(const BlockPos &var1, uint32_t var2) = 0;

    virtual IBlockState *getBlockState(const BlockPos &var1) = 0;

    virtual bool isAirBlock(const BlockPos &var1) = 0;

    virtual Biome &getBiome(const BlockPos &var1) = 0;

    virtual uint32_t getStrongPower(const BlockPos &var1, EnumFacing &var2) = 0;

    virtual WorldType getWorldType() = 0;
};
