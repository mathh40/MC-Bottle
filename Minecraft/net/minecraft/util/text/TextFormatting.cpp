#include "TextFormatting.h"

#include "../../../../../compile-time-regular-expressions/single-header/ctre.hpp"

#include <StringUtils.h>
#include <algorithm>

static constexpr auto FORMATTING_CODE_PATTERN = ctll::fixed_string{R"(§[0-9a-fk-or])"};

TextFormatting TextFormatting::BLACK("BLACK", '0', 0);
TextFormatting TextFormatting::DARK_BLUE("DARK_BLUE", '1', 1);
TextFormatting TextFormatting::DARK_GREEN("DARK_GREEN", '2', 2);
TextFormatting TextFormatting::DARK_AQUA("DARK_AQUA", '3', 3);
TextFormatting TextFormatting::DARK_RED("DARK_RED", '4', 4);
TextFormatting TextFormatting::DARK_PURPLE("DARK_PURPLE", '5', 5);
TextFormatting TextFormatting::GOLD("GOLD", '6', 6);
TextFormatting TextFormatting::GRAY("GRAY", '7', 7);
TextFormatting TextFormatting::DARK_GRAY("DARK_GRAY", '8', 8);
TextFormatting TextFormatting::BLUE("BLUE", '9', 9);
TextFormatting TextFormatting::GREEN("GREEN", 'a', 10);
TextFormatting TextFormatting::AQUA("AQUA", 'b', 11);
TextFormatting TextFormatting::RED("RED", 'c', 12);
TextFormatting TextFormatting::LIGHT_PURPLE("LIGHT_PURPLE", 'd', 13);
TextFormatting TextFormatting::YELLOW("YELLOW", 'e', 14);
TextFormatting TextFormatting::WHITE("WHITE", 'f', 15);
TextFormatting TextFormatting::OBFUSCATED("OBFUSCATED", 'k', true);
TextFormatting TextFormatting::BOLD("BOLD", 'l', true);
TextFormatting TextFormatting::STRIKETHROUGH("STRIKETHROUGH", 'm', true);
TextFormatting TextFormatting::UNDERLINE("UNDERLINE", 'n', true);
TextFormatting TextFormatting::ITALIC("ITALIC", 'o', true);
TextFormatting TextFormatting::RESET("RESET", 'r', -1);

std::string lowercaseAlpha(std::string p_175745_0_)
{
    std::transform(p_175745_0_.begin(), p_175745_0_.end(), p_175745_0_.begin(), ::tolower);
    return p_175745_0_;
}

int32_t TextFormatting::getColorIndex() const
{
    return colorIndex;
}

bool TextFormatting::isFancyStyling() const
{
    return fancyStyling;
}

bool TextFormatting::isColor() const
{
    return !fancyStyling && *this != RESET;
}

std::string TextFormatting::getFriendlyName() const
{
    return name;
}

std::string TextFormatting::toString() const
{
    return controlString;
}

std::optional<std::string> TextFormatting::getTextWithoutFormattingCodes(std::optional<std::string> text) const
{
    auto temp           = ctre::search<FORMATTING_CODE_PATTERN>(text.value());
    std::string result  = "";
    std::string replace = temp.str();
    StringUtils::replaceFirstOccurrence(result, replace, "");

    return text.has_value() ? std::optional<std::string>(result) : std::nullopt;
}

std::optional<TextFormatting> TextFormatting::getValueByName(std::optional<std::string> friendlyName)
{
    return friendlyName.has_value() ? std::optional<TextFormatting>(*NAME_MAPPING[lowercaseAlpha(friendlyName.value())])
                                    : std::nullopt;
}

std::optional<TextFormatting> TextFormatting::fromColorIndex(int32_t index)
{
    if (index < 0)
    {
        return RESET;
    }
    else
    {
        for (auto *textformatting : format)
        {
            if (textformatting->getColorIndex() == index)
            {
                return *textformatting;
            }
        }

        return std::nullopt;
    }
}

std::vector<std::string> TextFormatting::getValidValues(bool isColor, bool isFancyStyling)
{
    std::vector<std::string> list;
    for (auto *textformatting : format)
    {
        if ((!textformatting->isColor() || isColor) && (!textformatting->isFancyStyling() || isFancyStyling))
        {
            list.push_back(textformatting->getFriendlyName());
        }
    }

    return list;
}

TextFormatting::TextFormatting(std::string formattingName, char formattingCodeIn, int32_t colorIndex)
    : TextFormatting(formattingName, formattingCodeIn, false, colorIndex)
{
}

TextFormatting::TextFormatting(std::string formattingName, char formattingCodeIn, bool fancyStylingIn)
    : TextFormatting(formattingName, formattingCodeIn, fancyStylingIn, -1)
{
}

TextFormatting::TextFormatting(std::string formattingName, char formattingCodeIn, bool fancyStylingIn,
                               int32_t colorIndex)
    : name(formattingName), formattingCode(formattingCodeIn), fancyStyling(fancyStylingIn), colorIndex(colorIndex),
      controlString("\u00A7")
{
    format.push_back(this);
    NAME_MAPPING.emplace(name, this);
}
