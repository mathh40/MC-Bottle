#pragma once
#include <cstdint>

class Block;

class IBlockBehaviors {
protected:
    ~IBlockBehaviors() = default;
public:
    virtual bool onBlockEventReceived(World *var1, const BlockPos &var2, int32_t var3, int32_t var4) = 0;
    virtual void neighborChanged(World *var1, const BlockPos &var2, Block *var3, const BlockPos &var4) = 0;
};
