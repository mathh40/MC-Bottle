#pragma once
#include <array>

class NibbleArray
{
public:
	NibbleArray() = default;
	NibbleArray(std::array<unsigned char, 2048> storageArray);
	int32_t get(int32_t x, int32_t y, int32_t z);
	void set(int32_t x, int32_t y, int32_t z, int32_t value);
	int32_t getFromIndex(int32_t index);
	void setIndex(int32_t index, int32_t value);
	std::array<unsigned char, 2048> getData() const;
private:
	std::array<unsigned char, 2048> data;
	static int32_t getCoordinateIndex(int32_t x, int32_t y, int32_t z);
	static bool isLowerNibble(int32_t index);
	static int32_t getNibbleIndex(int32_t index);
};
