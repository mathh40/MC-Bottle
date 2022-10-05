#include "NBTTagFloat.h"

#include "NBTSizeTracker.h"

#include <istream>
#include <string>

NBTTagFloat::NBTTagFloat(float data) : data(data)
{
}

void NBTTagFloat::write(std::ostream &output)
{
    output.write(reinterpret_cast<char *>(&data), sizeof(data));
}

void NBTTagFloat::read(std::istream &input, int depth, NBTSizeTracker sizeTracker)
{
    sizeTracker.read(96L);
    input.read(reinterpret_cast<char *>(&data), sizeof(data));
}

uint8_t NBTTagFloat::getId() const
{
    return 5;
}

std::string NBTTagFloat::to_string() const
{
    return std::to_string(data) + "f";
}

int64_t NBTTagFloat::getLong() const
{
    return static_cast<long>(data);
}

int32_t NBTTagFloat::getInt() const
{
    return MathHelper::floor(data);
}

int16_t NBTTagFloat::getShort() const
{
    return (MathHelper::floor(data) & '\uffff');
}

uint8_t NBTTagFloat::getByte() const
{
    return (MathHelper::floor(data) & 255);
}

double NBTTagFloat::getDouble() const
{
    return static_cast<double>(data);
}

float NBTTagFloat::getFloat() const
{
    return data;
}

bool operator==(const NBTTagFloat &a, const NBTTagFloat &b)
{
    return a.data == b.data;
}
