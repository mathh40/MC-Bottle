#pragma once
#include <array>

#include "text/TextFormatting.h"

class EnumDyeColor
{
public:
    static EnumDyeColor WHITE;
    static EnumDyeColor ORANGE;
    static EnumDyeColor MAGENTA;
    static EnumDyeColor LIGHT_BLUE;
    static EnumDyeColor YELLOW;
    static EnumDyeColor LIME;
    static EnumDyeColor PINK;
    static EnumDyeColor GRAY;
    static EnumDyeColor SILVER;
    static EnumDyeColor CYAN;
    static EnumDyeColor PURPLE;
    static EnumDyeColor BLUE;
    static EnumDyeColor BROWN;
    static EnumDyeColor GREEN;
    static EnumDyeColor RED;
    static EnumDyeColor BLACK;

    EnumDyeColor(int32_t metaIn, int32_t dyeDamageIn, std::string nameIn, std::string unlocalizedNameIn, int32_t colorValueIn, TextFormatting chatColorIn);
    int32_t getMetadata() const;
    int32_t getDyeDamage() const;
    std::string getDyeColorName() const;
    std::string getTranslationKey() const;
    int32_t getColorValue() const;
    std::array<float,3> getColorComponentValues() const;
    static EnumDyeColor* byDyeDamage(int32_t damage);
    static EnumDyeColor* byMetadata(int32_t meta);
    std::string toString() const;
    std::string getName() const;


private:
    static std::vector<EnumDyeColor*> VALUES;
    static std::unordered_map<int32_t,EnumDyeColor*> META_LOOKUP;
    static std::unordered_map<int32_t,EnumDyeColor*> DYE_DMG_LOOKUP;
    int32_t meta;
    int32_t dyeDamage;
    std::string name;
    std::string translationKey;
    int32_t colorValue;
    std::array<float,3> colorComponentValues;
    TextFormatting chatColor;
};
