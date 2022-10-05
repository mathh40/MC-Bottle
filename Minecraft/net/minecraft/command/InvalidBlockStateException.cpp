#include "InvalidBlockStateException.h"

InvalidBlockStateException::InvalidBlockStateException()
    : InvalidBlockStateException("commands.generic.blockstate.invalid")
{
}

InvalidBlockStateException::InvalidBlockStateException(std::string_view message, std::vector<> &objects)
    : InvalidBlockStateException(message, objects)
{
}

std::exception &InvalidBlockStateException::fillInStackTrace()
{
    return *this;
}
