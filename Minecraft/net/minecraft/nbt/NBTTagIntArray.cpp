#include "NBTTagIntArray.h"

#include "NBTSizeTracker.h"

#include <istream>
#include <sstream>
#include <utility>

NBTTagIntArray::NBTTagIntArray(std::vector<int32_t> p_i45132_1_) : intArray(std::move(p_i45132_1_))
{
}

void NBTTagIntArray::write(std::ostream &output) const
{
    int32_t size = intArray.size();
    output.write(reinterpret_cast<char *>(&size), sizeof(size));
    for (auto i : intArray)
    {
        output.write(reinterpret_cast<char *>(&i), sizeof(i));
    }
}

void NBTTagIntArray::read(std::istream &input, int var2, NBTSizeTracker sizeTracker)
{
    sizeTracker.read(192L);
    int32_t size = 0;
    input.read(reinterpret_cast<char *>(&size), sizeof(size));
    sizeTracker.read(static_cast<long>(32 * size));
    intArray.resize(size);

    for (auto j = 0; j < size; ++j)
    {
        int32_t i = 0;
        input.read(reinterpret_cast<char *>(&i), sizeof(i));
        intArray[j] = i;
    }
}

uint8_t NBTTagIntArray::getId() const
{
    return 11;
}

std::string NBTTagIntArray::to_string() const
{
    std::stringstream stringbuilder("[I;");

    for (auto i = 0; i < intArray.size(); ++i)
    {
        if (i != 0)
        {
            stringbuilder << ',';
        }

        stringbuilder << (intArray[i]);
    }
    stringbuilder << ']';
    return stringbuilder.str();
}

std::vector<int32_t> NBTTagIntArray::getIntArray() const
{
    return intArray;
}

bool operator==(const NBTTagIntArray &a, const NBTTagIntArray &b)
{
    return a.intArray == b.intArray;
}
