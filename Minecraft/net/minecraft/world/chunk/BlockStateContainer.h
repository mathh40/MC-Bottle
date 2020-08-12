#pragma once
#include "IBlockStatePaletteResizer.h"
#include "IBlockStatePalette.h"
#include "../../util/BitArray.h"
#include <optional>
#include "NibbleArray.h"

class BlockStateContainer :public IBlockStatePaletteResizer
{
public:
	BlockStateContainer();
	int32_t onResize(int32_t bits, IBlockState* state) override;
	void set(int32_t x, int32_t y, int32_t z, IBlockState* state);
	IBlockState* get(int32_t x, int32_t y, int32_t z);
	void read(const PacketBuffer& buf);
	void write(const PacketBuffer& buf);
	std::optional<NibbleArray> getDataForNBT(std::vector<unsigned char> blockIds, NibbleArray data);
	void setDataFromNBT(std::vector<unsigned char> blockIds, NibbleArray data, std::optional<NibbleArray> blockIdExtension);
	int32_t getSerializedSize();
protected:
	static IBlockState* AIR_BLOCK_STATE;
	BitArray storage;
	IBlockStatePalette* palette;
	void set(int32_t index, IBlockState* state);
	IBlockState* get(int32_t index);
private:
	static IBlockStatePalette* REGISTRY_BASED_PALETTE;
	int32_t bits;
	static int32_t getIndex(int32_t x, int32_t y, int32_t z);
	void setBits(int32_t bitsIn);
};
