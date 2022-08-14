#pragma once
#include "CommandException.h"

class SyntaxErrorException :public CommandException {
public:
    SyntaxErrorException();
    SyntaxErrorException(std::string_view message, const std::vector<> &replacements);
    std::exception &fillInStackTrace() override;
};
