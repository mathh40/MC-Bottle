#include "NBTTagInt.h"
#include "NBTSizeTracker.h"
#include <istream>
#include <string>

NBTTagInt::NBTTagInt(int32_t data)
	:data(data)
{

}

void NBTTagInt::write(std::ostream& output)
{
	output.write(reinterpret_cast<char*>(&data),sizeof(data));
}

void NBTTagInt::read(std::istream& input, int depth, NBTSizeTracker sizeTracker)
{
	sizeTracker.read(96L);
	input.read(reinterpret_cast<char*>(&data), sizeof(data));
}

uint8_t NBTTagInt::getId() const
{
	return 3;
}

std::string NBTTagInt::to_string() const
{
	return std::to_string(data);
}

int64_t NBTTagInt::getLong() const
{
	return data;
}

int32_t NBTTagInt::getInt() const
{
	return data;
}

int16_t NBTTagInt::getShort() const
{
	return data & '\uffff';
}

uint8_t NBTTagInt::getByte() const
{
	return data & 255;
}

double NBTTagInt::getDouble() const
{
	return data;
}

float NBTTagInt::getFloat() const
{
	return data;
}

bool operator==(const NBTTagInt& a, const NBTTagInt& b)
{
	return a.data == b.data;
}
