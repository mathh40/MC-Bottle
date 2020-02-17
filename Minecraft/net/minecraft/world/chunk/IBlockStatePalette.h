#pragma once
#include <cstdint>
class IBlockState;

class IBlockStatePalette
{
public:
	virtual int idFor(IBlockState* var1) = 0;

	virtual IBlockState* getBlockState(int32_t var1) = 0;

	virtual void read(PacketBuffer var1) = 0;

	virtual void write(PacketBuffer var1) = 0;

	virtual int32_t getSerializedSize() = 0;
};
