#pragma once
#include "CommandException.h"

class PlayerNotFoundException : public CommandException
{
  public:
    PlayerNotFoundException(std::string_view message);
    PlayerNotFoundException(std::string_view message, std::vector<> &replacements);
    std::exception &fillInStackTrace() override;
};
