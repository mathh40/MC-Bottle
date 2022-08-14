#pragma once
#include "SyntaxErrorException.h"

class WrongUsageException :public SyntaxErrorException {
public:
    WrongUsageException(std::string_view message, const std::vector<> &replacements);
    std::exception &fillInStackTrace() override;
};
