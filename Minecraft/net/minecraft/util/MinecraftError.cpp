#include "MinecraftError.h"

#include <utility>

MinecraftError::MinecraftError(const char *msg) : msg_(msg)
{
}

MinecraftError::MinecraftError(std::string msg) : msg_(std::move(msg))
{
}

char const *MinecraftError::what() const
{
    return msg_.c_str();
}

MinecraftError::~MinecraftError()
{
}
