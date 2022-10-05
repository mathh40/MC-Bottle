#include "PlayerNotFoundException.h"

PlayerNotFoundException::PlayerNotFoundException(std::string_view message) : CommandException(message)
{
}

PlayerNotFoundException::PlayerNotFoundException(std::string_view message, std::vector<> &replacements)
    : CommandException(message, replacements)
{
}

std::exception &PlayerNotFoundException::fillInStackTrace()
{
    return *this;
}
