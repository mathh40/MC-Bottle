#include "EnumRarity.h"

EnumRarity EnumRarity::COMMON(TextFormatting::WHITE, "Common");
EnumRarity EnumRarity::UNCOMMON(TextFormatting::YELLOW, "Uncommon");
EnumRarity EnumRarity::RARE(TextFormatting::AQUA, "Rare");
EnumRarity EnumRarity::EPIC(TextFormatting::LIGHT_PURPLE, "Epic");

EnumRarity::EnumRarity(TextFormatting color, std::string_view name) : color(color), rarityName(name)
{
}
