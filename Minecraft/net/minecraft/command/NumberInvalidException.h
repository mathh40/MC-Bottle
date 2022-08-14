#pragma once
#include "CommandException.h"

class NumberInvalidException :public CommandException {
public:
    NumberInvalidException();
    NumberInvalidException(std::string_view message, std::vector<> &replacements);
    std::exception &fillInStackTrace() override;
};
