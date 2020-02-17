#pragma once
#include "IBlockStatePalette.h"
#include "IBlockStatePaletteResizer.h"
#include <vector>

class BlockStatePaletteLinear :public IBlockStatePalette
{
public:
	BlockStatePaletteLinear(int32_t bitsIn, IBlockStatePaletteResizer* resizeHandlerIn);
	int32_t idFor(IBlockState* state) override;
	IBlockState* getBlockState(int32_t indexKey) override;
	void read(PacketBuffer buf) override;
	void write(PacketBuffer buf) override;
	int32_t getSerializedSize() override;
private:
	std::vector<IBlockState*> states;
	IBlockStatePaletteResizer* resizeHandler;
	int32_t bits;
	int32_t arraySize;
};
