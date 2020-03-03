#pragma once
#include <string>
#include <unordered_map>
#include <regex>
#include <optional>
#include <ostream>

class TextFormatting
{
public:
	int32_t getColorIndex() const;
	bool isFancyStyling() const;
	bool isColor() const;
	std::string getFriendlyName() const;
	std::string toString() const;
	std::optional<std::string> getTextWithoutFormattingCodes(std::optional<std::string> text) const;
	static std::optional <TextFormatting> getValueByName(std::optional<std::string> friendlyName);
	static std::optional <TextFormatting> fromColorIndex(int32_t index);
	static std::vector<std::string> getValidValues(bool isColor, bool isFancyStyling);

	static TextFormatting BLACK;
	static TextFormatting DARK_BLUE;
	static TextFormatting DARK_GREEN;
	static TextFormatting DARK_AQUA;
	static TextFormatting DARK_RED;
	static TextFormatting DARK_PURPLE;
	static TextFormatting GOLD;
	static TextFormatting GRAY;
	static TextFormatting DARK_GRAY;
	static TextFormatting BLUE;
	static TextFormatting GREEN;
	static TextFormatting AQUA;
	static TextFormatting RED;
	static TextFormatting LIGHT_PURPLE;
	static TextFormatting YELLOW;
	static TextFormatting WHITE;
	static TextFormatting OBFUSCATED;
	static TextFormatting BOLD;
	static TextFormatting STRIKETHROUGH;
	static TextFormatting UNDERLINE;
	static TextFormatting ITALIC;
	static TextFormatting RESET;

private:
	static std::unordered_map<std::string,TextFormatting*> NAME_MAPPING;
	static std::vector<TextFormatting*> format;
	static std::regex FORMATTING_CODE_PATTERN;

	TextFormatting(std::string formattingName, char formattingCodeIn, int32_t colorIndex);
	TextFormatting(std::string formattingName, char formattingCodeIn, bool fancyStylingIn);
	TextFormatting(std::string formattingName, char formattingCodeIn, bool fancyStylingIn, int32_t colorIndex);

	std::string name;
	char formattingCode;
	bool fancyStyling;
	std::string controlString;
	int32_t colorIndex;
};

auto operator==(const TextFormatting& lhs, const TextFormatting& rhs) -> bool
{
	return lhs.getColorIndex() == rhs.getColorIndex();
}

auto operator!=(const TextFormatting& lhs, const TextFormatting& rhs) -> bool
{
	return !(lhs == rhs);
}

auto operator<<(std::ostream& lhs, const TextFormatting& rhs)->std::ostream&
{
	return lhs << rhs.toString();
}