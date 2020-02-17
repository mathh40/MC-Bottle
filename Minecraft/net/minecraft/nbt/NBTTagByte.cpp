#include "NBTTagByte.h"
#include "NBTSizeTracker.h"
#include <istream>
#include <string>

NBTTagByte::NBTTagByte()
{
}

NBTTagByte::NBTTagByte(uint8_t data)
	:data(data)
{

}

void NBTTagByte::write(std::ostream& output)
{
	output.write(reinterpret_cast<char*>(&data), sizeof(data));
}

void NBTTagByte::read(std::istream& input, int depth, NBTSizeTracker sizeTracker)
{
	sizeTracker.read(72L);
	input.read(reinterpret_cast<char*>(&data), sizeof(data));
}

uint8_t NBTTagByte::getId() const
{
	return 1;
}

std::string NBTTagByte::to_string() const
{
	return std::to_string(data) + "b";
}

int64_t NBTTagByte::getLong() const
{
	return data;
}

int32_t NBTTagByte::getInt() const
{
	return data;
}

int16_t NBTTagByte::getShort() const
{
	return data;
}

uint8_t NBTTagByte::getByte() const
{
	return data;
}

double NBTTagByte::getDouble() const
{
	return data;
}

float NBTTagByte::getFloat() const
{
	return data;
}

bool operator==(const NBTTagByte& a, const NBTTagByte& b)
{
	return a.data == b.data;
}
