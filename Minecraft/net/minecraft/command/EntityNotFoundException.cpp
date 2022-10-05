#include "EntityNotFoundException.h"

EntityNotFoundException::EntityNotFoundException(std::string_view p_i47332_1_)
    : EntityNotFoundException("commands.generic.entity.notFound", p_i47332_1_)
{
}

EntityNotFoundException::EntityNotFoundException(std::string_view message, std::vector<> args)
    : CommandException(message, args)
{
}

std::exception &EntityNotFoundException::fillInStackTrace()
{
    return *this;
}
