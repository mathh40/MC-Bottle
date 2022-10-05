#include "TextComponentString.h"

TextComponentString::TextComponentString(std::string_view msg) : text(msg)
{
}

std::string TextComponentString::getText() const
{
    return text;
}

std::string TextComponentString::getUnformattedText() const
{
    return text;
}

std::string TextComponentString::toString() const
{
    return "TextComponent{text='" + text + '\'' + ", siblings=" + siblings + ", style=" + getStyle() + '}';
}
