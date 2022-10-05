#pragma once
#include "CommandException.h"

class EntityNotFoundException : public CommandException
{
  public:
    EntityNotFoundException(std::string_view p_i47332_1_);
    EntityNotFoundException(std::string_view message, std::vector<> args);
    std::exception &fillInStackTrace() override;
};
