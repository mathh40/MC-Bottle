#include "SyntaxErrorException.h"

SyntaxErrorException::SyntaxErrorException() : SyntaxErrorException("commands.generic.snytax")
{
}

SyntaxErrorException::SyntaxErrorException(std::string_view message, const std::vector<> &replacements)
    : CommandException(message, replacements)
{
}

std::exception &SyntaxErrorException::fillInStackTrace()
{
    return *this;
}
