#pragma once
#include <stdexcept>

class MinecraftError :public std::exception
{

public:

	explicit MinecraftError(const char* msg);
	explicit MinecraftError(std::string msg);
	char const* what() const override;
	~MinecraftError();
private:
	std::string msg_;
};
