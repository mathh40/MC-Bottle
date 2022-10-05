#include "CommandException.h"

CommandException::CommandException(std::string_view message, const std::vector<> &objects)
    : std::exception(message.data())
{
    errorObjects = objects;
}

std::vector<> CommandException::getErrorObjects()
{
    return errorObjects;
}

std::exception &CommandException::fillInStackTrace()
{
    return *this;
}
