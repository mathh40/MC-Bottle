#include "NBTTagString.h"
#include "NBTSizeTracker.h"
#include <istream>
#include <ostream>
#include <sstream>

NBTTagString::NBTTagString(std::string data)
	:data(data)
{

}

void NBTTagString::write(std::ostream& output) const
{
	output.write(data.data(),sizeof(data.size()));
}

void NBTTagString::read(std::istream& input, int var2, NBTSizeTracker sizeTracker)
{
	sizeTracker.read(288L);
	input >> data;
	sizeTracker.read((16 * data.size()));
}

uint8_t NBTTagString::getId() const
{
	return 8;
}

std::string NBTTagString::to_string() const
{
	return quoteAndEscape(data);
}

bool NBTTagString::isEmpty() const
{
	return data.empty();
}

std::string NBTTagString::getString() const
{
	return data;
}

std::string NBTTagString::quoteAndEscape(std::string p_193588_0_) const
{
	std::stringstream stringbuilder("\"");

	for(auto c0 : p_193588_0_)
	 {
		if (c0 == '\\' || c0 == '"') {
			stringbuilder<<'\\';
		}

		stringbuilder<<c0;
	}
	stringbuilder << '"';

	return stringbuilder.str();
}

bool operator==(const NBTTagString& a, const NBTTagString& b)
{
	return a.data == b.data;
}
