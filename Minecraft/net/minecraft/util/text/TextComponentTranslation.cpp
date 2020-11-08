#include "TextComponentTranslation.h"
#include "../../../../../compile-time-regular-expressions/single-header/ctre.hpp"

static constexpr auto STRING_VARIABLE_PATTERN = ctll::fixed_string{ R"(%(?:(\d+)\$)?([A-Za-z%]|$))"};