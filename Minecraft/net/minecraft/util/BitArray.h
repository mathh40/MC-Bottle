#pragma once
#include <vector>

class BitArray
{
public:
	BitArray() = default;
	BitArray(uint32_t bitsPerEntryIn, size_t arraySizeIn);
	void setAt(size_t index, uint64_t value);
	uint32_t getAt(size_t index);
	std::vector<uint64_t> getBackingLongArray() const;
	size_t size() const;
private:
	std::vector<uint64_t> longArray;
	const uint32_t bitsPerEntry;
	const uint64_t maxEntryValue;
	const size_t  arraySize;
};