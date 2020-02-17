#include "TextComponentTranslation.h"

std::regex TextComponentTranslation::STRING_VARIABLE_PATTERN = std::regex("%(?:(\\d+)\\$)?([A-Za-z%]|$)")