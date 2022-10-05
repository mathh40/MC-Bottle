#include "NumberInvalidException.h"

NumberInvalidException::NumberInvalidException() : NumberInvalidException("commands.generic.num.invalid")
{
}

NumberInvalidException::NumberInvalidException(std::string_view message, std::vector<> &replacements)
    : CommandException(message, replacements)
{
}

std::exception &NumberInvalidException::fillInStackTrace()
{
    return *this;
}
