#pragma once
#include <string>
#include <vector>

class ITabCompleter
{
public:
	virtual ~ITabCompleter() = default;
	virtual void setCompletions(std::vector<std::string> var1) = 0;
private:
	
};
