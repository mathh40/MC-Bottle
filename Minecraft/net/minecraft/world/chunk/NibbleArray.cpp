#include "NibbleArray.h"

NibbleArray::NibbleArray(std::array<unsigned char, 2048> storageArray) : data(storageArray)
{
}

int32_t NibbleArray::get(int32_t x, int32_t y, int32_t z)
{
    return getFromIndex(getCoordinateIndex(x, y, z));
}

void NibbleArray::set(int32_t x, int32_t y, int32_t z, int32_t value)
{
    setIndex(getCoordinateIndex(x, y, z), value);
}

int32_t NibbleArray::getFromIndex(int32_t index)
{
    auto i = getNibbleIndex(index);
    return isLowerNibble(index) ? data[i] & 15 : data[i] >> 4 & 15;
}

void NibbleArray::setIndex(int32_t index, int32_t value)
{
    auto i = getNibbleIndex(index);
    if (isLowerNibble(index))
    {
        data[i] = (data[i] & 240 | value & 15);
    }
    else
    {
        data[i] = (data[i] & 15 | (value & 15) << 4);
    }
}

std::array<unsigned char, 2048> NibbleArray::getData() const
{
    return data;
}

int32_t NibbleArray::getCoordinateIndex(int32_t x, int32_t y, int32_t z)
{
    return y << 8 | z << 4 | x;
}

bool NibbleArray::isLowerNibble(int32_t index)
{
    return (index & 1) == 0;
}

int32_t NibbleArray::getNibbleIndex(int32_t index)
{
    return index >> 1;
}
