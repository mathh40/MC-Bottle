#pragma once
#include <cstdint>
class IBlockState;

class PacketBuffer;
class IBlockStatePalette
{
public:
	virtual int idFor(IBlockState* var1) = 0;

	virtual IBlockState* getBlockState(int32_t var1) = 0;

	virtual void read(const PacketBuffer& var1) = 0;

	virtual void write(const PacketBuffer& var1) = 0;

	virtual int32_t getSerializedSize() = 0;
};
