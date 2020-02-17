#pragma once
#include "IBlockStatePalette.h"

class BlockStatePaletteRegistry :public IBlockStatePalette
{
public:
	int32_t idFor(IBlockState* state) override;
	IBlockState* getBlockState(int32_t indexKey) override;
	void read(PacketBuffer buf) override;
	void write(PacketBuffer buf) override;
	int32_t getSerializedSize() override;
};
