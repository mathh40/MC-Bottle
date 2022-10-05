#include "TextComponentKeybind.h"

TextComponentKeybind::TextComponentKeybind(const std::string &keybind) : keybind(keybind)
{
}

std::string TextComponentKeybind::getUnformattedText() const
{
    return keybind;
}

std::string TextComponentKeybind::toString() const
{
    return "KeybindComponent{keybind='" + keybind + '\'' + ", siblings=" + siblings + ", style=" + getStyle() + '}';
}

std::string TextComponentKeybind::getKeybind() const
{
    return keybind;
}
