#include "NBTTagLongArray.h"

#include "NBTSizeTracker.h"

#include <istream>
#include <sstream>

NBTTagLongArray::NBTTagLongArray(std::vector<int64_t> p_i47524_1_) : data(p_i47524_1_)
{
}

void NBTTagLongArray::write(std::ostream &output) const
{
    uint32_t size = data.size();
    output.write(reinterpret_cast<char *>(&size), sizeof(size));

    for (auto i : data)
    {
        output.write(reinterpret_cast<char *>(&i), sizeof(i));
    }
}

void NBTTagLongArray::read(std::istream &input, int var2, NBTSizeTracker sizeTracker)
{
    sizeTracker.read(192L);

    uint32_t size = 0;
    input.read(reinterpret_cast<char *>(&size), sizeof(size));
    sizeTracker.read((long)(64 * size));
    data.clear();
    data.resize(size);

    for (int j = 0; j < size; ++j)
    {
        input.read(reinterpret_cast<char *>(&data[j]), sizeof(int64_t));
    }
}

uint8_t NBTTagLongArray::getId() const
{
    return 12;
}

std::string NBTTagLongArray::to_string() const
{
    std::stringstream stringbuilder("[L;");

    for (int i = 0; i < data.size(); ++i)
    {
        if (i != 0)
        {
            stringbuilder << ',';
        }

        stringbuilder << data[i] << 'L';
    }
    stringbuilder << ']';

    return stringbuilder.str();
}

bool operator==(const NBTTagLongArray &a, const NBTTagLongArray &b)
{
    return a.data == b.data;
}
