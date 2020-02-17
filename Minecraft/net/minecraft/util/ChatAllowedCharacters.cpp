#include "ChatAllowedCharacters.h"
#include <algorithm>

bool ChatAllowedCharacters::isAllowedCharacter(char character)
{
	return character != '§' && character >= ' ' && character != 127;
}

std::string ChatAllowedCharacters::filterAllowedCharacters(std::string_view input)
{
	std::string temp;
	for(const auto& item : input)
	{
		if(isAllowedCharacter(item)) 
		{
			temp.push_back(item);
		}
	}
	return temp;
}
