#include "NBTTagShort.h"
#include "NBTSizeTracker.h"
#include <istream>
#include <string>

NBTTagShort::NBTTagShort(int16_t data)
	:data(data)
{
}

void NBTTagShort::write(std::ostream& output) const
{
	output.write(reinterpret_cast<const char*>(&data),sizeof(data));
}

void NBTTagShort::read(std::istream& input, int depth, NBTSizeTracker sizeTracker)
{
	sizeTracker.read(80L);
	input.read(reinterpret_cast<char*>(&data), sizeof(data));
}

uint8_t NBTTagShort::getId() const
{
	return 2;
}

std::string NBTTagShort::to_string() const
{
	return std::to_string(data) + "s";
}

int64_t NBTTagShort::getLong() const
{
	return data;
}

int32_t NBTTagShort::getInt() const
{
	return data;
}

int16_t NBTTagShort::getShort() const
{
	return data;
}

uint8_t NBTTagShort::getByte() const
{
	return data & 255;
}

double NBTTagShort::getDouble() const
{
	return data;
}

float NBTTagShort::getFloat() const
{
	return data;
}

bool operator==(const NBTTagShort& a, const NBTTagShort& b)
{
	return a.data == b.data;
}
