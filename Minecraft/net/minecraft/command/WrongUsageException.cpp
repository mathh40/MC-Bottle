#include "WrongUsageException.h"

WrongUsageException::WrongUsageException(std::string_view message, const std::vector<> &replacements)
{
    SyntaxErrorException(message, replacements);
}

std::exception &WrongUsageException::fillInStackTrace()
{
    return *this;
}
