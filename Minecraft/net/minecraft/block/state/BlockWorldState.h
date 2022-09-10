#pragma once
#include "Block.h"

class BlockWorldState {
public:
    BlockWorldState(World *worldIn, const BlockPos &posIn, bool forceLoadIn);
    IBlockState *getBlockState() const;
    std::optional<TileEntity> getTileEntity() const;
    BlockPos &getPos() const;
private:
    World* world;
    BlockPos pos;
    bool forceLoad;
    IBlockState *state;
    TileEntity* tileEntity;
    bool tileEntityInitialized;
protected:
};
