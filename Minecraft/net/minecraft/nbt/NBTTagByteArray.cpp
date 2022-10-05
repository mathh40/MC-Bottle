#include "NBTTagByteArray.h"

#include "NBTSizeTracker.h"

#include <istream>
#include <sstream>
#include <utility>

NBTTagByteArray::NBTTagByteArray(const ByteBuffer &data) : data(std::move(data))
{
}

void NBTTagByteArray::write(std::ostream &output)
{
    int32_t size = data.size();
    output.write(reinterpret_cast<char *>(size), sizeof(size));
    const auto dataptr = data.data();
    output.write(reinterpret_cast<char *>(dataptr), sizeof(uint8_t) * size);
}

void NBTTagByteArray::read(std::istream &input, int depth, NBTSizeTracker sizeTracker)
{
    sizeTracker.read(192L);
    int32_t i;
    input.read(reinterpret_cast<char *>(&i), sizeof(i));
    sizeTracker.read((int64_t)(8 * i));
    data.resize(i);
    auto dataptr = data.data();
    input.read(reinterpret_cast<char *>(&dataptr), sizeof(uint8_t) * i);
}

uint8_t NBTTagByteArray::getId() const
{
    return 7;
}

std::string NBTTagByteArray::to_string() const
{
    std::stringstream stringbuilder("[B;");

    for (int i = 0; i < data.size(); ++i)
    {
        if (i != 0)
        {
            stringbuilder << (',');
        }

        stringbuilder << (data[i]) << 'B';
    }
    stringbuilder << ']';
    return stringbuilder.str();
}

const ByteBuffer NBTTagByteArray::getByteArray() const
{
    return data;
}

bool operator==(const NBTTagByteArray &a, const NBTTagByteArray &b)
{
    return a.data == b.data;
}
