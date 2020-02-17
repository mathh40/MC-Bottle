#pragma once
#include <exception>

class MinecraftException : public std::exception
{
public:
	explicit MinecraftException(const char* msg);
};
