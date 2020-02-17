#include "NBTTagLong.h"
#include "NBTSizeTracker.h"
#include <istream>
#include <string>

NBTTagLong::NBTTagLong(int64_t data)
	:data(data)
{
}

void NBTTagLong::write(std::ostream& output) const
{
	output.write(reinterpret_cast<const char*>(&data),sizeof(data));
}

void NBTTagLong::read(std::istream& input, int depth, NBTSizeTracker sizeTracker)
{
	sizeTracker.read(128L);
	input.read(reinterpret_cast<char*>(&data), sizeof(data));
}

uint8_t NBTTagLong::getId() const
{
	return 4;
}

std::string NBTTagLong::to_string() const
{
	return std::to_string(data) + "L";
}

int64_t NBTTagLong::getLong() const
{
	return data;
}

int32_t NBTTagLong::getInt() const
{
	return data & -1L;
}

int16_t NBTTagLong::getShort() const
{
	return data & 65535L;
}

uint8_t NBTTagLong::getByte() const
{
	return data & 255L;
}

double NBTTagLong::getDouble() const
{
	return data;
}

float NBTTagLong::getFloat() const
{
	return data;
}

bool operator==(const NBTTagLong& a, const NBTTagLong& b)
{
	return a.data == b.data;
}
