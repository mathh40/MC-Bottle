#include "BitArray.h"
#include <stdexcept>
#include "../world/World.h"

BitArray::BitArray(uint32_t bitsPerEntryIn, size_t arraySizeIn)
{
	if (bitsPerEntryIn >= 1 && bitsPerEntryIn <= 32)
	{
		arraySize = arraySizeIn;
		bitsPerEntry = bitsPerEntryIn;
		maxEntryValue = (1L << bitsPerEntryIn) - 1L;
		longArray.resize(MathHelper::roundUp(arraySizeIn * bitsPerEntryIn, 64) / 64);
	}
	else
		throw std::logic_error("bitsPerEntryIn muss zwischen 1 und 32 bit liegen");

}

void BitArray::setAt(size_t index, uint64_t value)
{
	const auto i = index * bitsPerEntry;
	const auto j = i / 64;
	const auto k = ((index + 1) * bitsPerEntry - 1) / 64;
	const auto l = i % 64;
	longArray[j] = longArray[j] & ~(maxEntryValue << l) | (value & maxEntryValue) << l;
	if (j != k) {
		const auto i1 = 64 - l;
		const auto j1 = bitsPerEntry - i1;
		longArray[k] = longArray[k] >> j1 << j1 | ((value) & maxEntryValue) >> i1;
	}
}

uint32_t BitArray::getAt(size_t index)
{
	const auto i = index * bitsPerEntry;
	const auto j = i / 64;
	const auto k = ((index + 1) * bitsPerEntry - 1) / 64;
	const auto l = i % 64;
	if (j == k) {
		return (longArray[j] >> l & maxEntryValue);
	}
	else {
		const auto i1 = 64 - l;
		return ((longArray[j] >> l | longArray[k] << i1) & maxEntryValue);
	}
}

std::vector<uint64_t> BitArray::getBackingLongArray() const
{
	return longArray;
}

size_t BitArray::size() const
{
	return arraySize;
}
