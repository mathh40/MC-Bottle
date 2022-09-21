#include "ChatAllowedCharacters.h"
#include <algorithm>

constexpr bool ChatAllowedCharacters::isAllowedCharacter(char character)
{
	return character != '&' && character >= ' ' && character != 127;
}

std::string ChatAllowedCharacters::filterAllowedCharacters(std::string_view input)
{
	std::string temp(input.size());
	auto it = std::copy_if(input.begin(),input.end(),input.begin(),&isAllowedCharacter);
	temp.resize(std::distance(temp.begin(),it));
	return temp;
}
