#include "NBTTagDouble.h"

#include "NBTSizeTracker.h"

#include <istream>
#include <string>

NBTTagDouble::NBTTagDouble(double data) : data(data)
{
}

void NBTTagDouble::write(std::ostream &output)
{
    output.write(reinterpret_cast<char *>(&data), sizeof(data));
}

void NBTTagDouble::read(std::istream &input, int depth, NBTSizeTracker sizeTracker)
{
    sizeTracker.read(128L);
    input.read(reinterpret_cast<char *>(&data), sizeof(data));
}

uint8_t NBTTagDouble::getId() const
{
    return 6;
}

std::string NBTTagDouble::to_string() const
{
    return std::to_string(data) + "d";
}

int64_t NBTTagDouble::getLong() const
{
    return std::floor(data);
}

int32_t NBTTagDouble::getInt() const
{
    return MathHelper::floor(data);
}

int16_t NBTTagDouble::getShort() const
{
    return (MathHelper::floor(data) & '\uffff');
}

uint8_t NBTTagDouble::getByte() const
{
    return (MathHelper::floor(data) & 255);
}

double NBTTagDouble::getDouble() const
{
    return data;
}

float NBTTagDouble::getFloat() const
{
    return data;
}

bool operator==(const NBTTagDouble &a, const NBTTagDouble &b)
{
    return a.data == b.data;
}
