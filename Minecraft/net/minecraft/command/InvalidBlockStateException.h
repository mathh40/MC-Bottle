#pragma once
#include "CommandException.h"

class InvalidBlockStateException : public CommandException
{
  public:
    InvalidBlockStateException();
    InvalidBlockStateException(std::string_view message, std::vector<> &objects);
    std::exception &fillInStackTrace() override;
};
