#pragma once
#include <string>
#include <array>

namespace ChatAllowedCharacters 
{
	static const std::array<char,14> ILLEGAL_STRUCTURE_CHARACTERS = {'.', '\n', '\r', '\t', '\u0000', '\f', '`', '?', '*', '\\', '<', '>', '|', '"'};
	static const std::array<char,15> ILLEGAL_FILE_CHARACTERS[] = {'/', '\n', '\r', '\t', '\u0000', '\f', '`', '?', '*', '\\', '<', '>', '|', '"', ':'};
	
	bool isAllowedCharacter(char character);
	std::string filterAllowedCharacters(std::string_view input);
};


