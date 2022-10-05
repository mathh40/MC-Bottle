#pragma once
#include "text/TextFormatting.h"

class EnumRarity
{
  public:
    static EnumRarity COMMON;
    static EnumRarity UNCOMMON;
    static EnumRarity RARE;
    static EnumRarity EPIC;
    EnumRarity(TextFormatting color, std::string_view name);
    const TextFormatting color;
    const std::string rarityName;
};
