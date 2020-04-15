#include "EnumDyeColor.h"

EnumDyeColor EnumDyeColor::WHITE(0, 15, "white", "white", 16383998, TextFormatting::WHITE);
EnumDyeColor EnumDyeColor::ORANGE(1, 14, "orange", "orange", 16351261, TextFormatting::GOLD);
EnumDyeColor EnumDyeColor::MAGENTA(2, 13, "magenta", "magenta", 13061821, TextFormatting::AQUA);
EnumDyeColor EnumDyeColor::LIGHT_BLUE(3, 12, "light_blue", "lightBlue", 3847130, TextFormatting::BLUE);
EnumDyeColor EnumDyeColor::YELLOW(4, 11, "yellow", "yellow", 16701501, TextFormatting::YELLOW);
EnumDyeColor EnumDyeColor::LIME(5, 10, "lime", "lime", 8439583, TextFormatting::GREEN);
EnumDyeColor EnumDyeColor::PINK(6, 9, "pink", "pink", 15961002, TextFormatting::LIGHT_PURPLE);
EnumDyeColor EnumDyeColor::GRAY(7, 8, "gray", "gray", 4673362, TextFormatting::DARK_GRAY);
EnumDyeColor EnumDyeColor::SILVER(8, 7, "silver", "silver", 10329495, TextFormatting::GRAY);
EnumDyeColor EnumDyeColor::CYAN(9, 6, "cyan", "cyan", 1481884, TextFormatting::DARK_AQUA);
EnumDyeColor EnumDyeColor::PURPLE(10, 5, "purple", "purple", 8991416, TextFormatting::DARK_PURPLE);
EnumDyeColor EnumDyeColor::BLUE(11, 4, "blue", "blue", 3949738, TextFormatting::DARK_BLUE);
EnumDyeColor EnumDyeColor::BROWN(12, 3, "brown", "brown", 8606770, TextFormatting::GOLD);
EnumDyeColor EnumDyeColor::GREEN(13, 2, "green", "green", 6192150, TextFormatting::DARK_GREEN);
EnumDyeColor EnumDyeColor::RED(14, 1, "red", "red", 11546150, TextFormatting::DARK_RED);
EnumDyeColor EnumDyeColor::BLACK(15, 0, "black", "black", 1908001, TextFormatting::BLACK);

EnumDyeColor::EnumDyeColor(int32_t metaIn, int32_t dyeDamageIn, std::string nameIn, std::string unlocalizedNameIn,
    int32_t colorValueIn, TextFormatting chatColorIn)
        :meta(metaIn),dyeDamage(dyeDamageIn),name(nameIn),translationKey(unlocalizedNameIn),colorValue(colorValueIn),chatColor(chatColorIn),
    colorComponentValues({((colorValueIn & 16711680) >> 16) / 255.0F, ((colorValueIn & 0xff00) >> 8) / 255.0F,
                          ((colorValueIn & 255) >> 0) / 255.0F})
{
    VALUES.emplace_back(this);
    META_LOOKUP.emplace(meta,this);
    DYE_DMG_LOOKUP.emplace(dyeDamage,this);
}

int32_t EnumDyeColor::getMetadata() const
{
    return meta;
}

int32_t EnumDyeColor::getDyeDamage() const
{
    return dyeDamage;
}

std::string EnumDyeColor::getDyeColorName() const
{
    return name;
}

std::string EnumDyeColor::getTranslationKey() const
{
    return translationKey;
}

int32_t EnumDyeColor::getColorValue() const
{
    return colorValue;
}

std::array<float, 3> EnumDyeColor::getColorComponentValues() const
{
    return colorComponentValues;
}

EnumDyeColor* EnumDyeColor::byDyeDamage(int32_t damage)
{
    if (damage < 0 || damage >= DYE_DMG_LOOKUP.size()) 
    {
        damage = 0;
    }

    return DYE_DMG_LOOKUP[damage];
}

EnumDyeColor* EnumDyeColor::byMetadata(int32_t meta)
{
    if (meta < 0 || meta >= META_LOOKUP.size()) 
    {
        meta = 0;
    }

    return META_LOOKUP[meta];
}

std::string EnumDyeColor::toString() const
{
    return translationKey;
}

std::string EnumDyeColor::getName() const
{
    return name;
}

