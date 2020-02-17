#pragma once
#include "IBlockStatePalette.h"
#include "IBlockStatePaletteResizer.h"
#include "../../util/IntIdentityHashBiMap.h"

class BlockStatePaletteHashMap :public IBlockStatePalette
{
public:
	BlockStatePaletteHashMap(int32_t bitsIn, IBlockStatePaletteResizer* paletteResizerIn);
	int32_t idFor(IBlockState* state) override;
	IBlockState* getBlockState(int32_t indexKey) override;
	void read(PacketBuffer buf) override;
	void write(PacketBuffer buf) override;
	int32_t getSerializedSize() override;
private:
	IntIdentityHashBiMap<int32_t, IBlockState*> statePaletteMap;
	IBlockStatePaletteResizer* paletteResizer;
	int32_t bits;
};
