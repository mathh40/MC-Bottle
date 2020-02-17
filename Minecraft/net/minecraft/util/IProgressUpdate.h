#pragma once
#include <string>

class IProgressUpdate
{
public:
	virtual ~IProgressUpdate() = default;
	virtual void displaySavingString(std::string var1) = 0;

	virtual void resetProgressAndMessage(std::string var1) = 0;

	virtual void displayLoadingString(std::string var1) = 0;

	virtual void setLoadingProgress(int var1) = 0;

	virtual void setDoneWorking() = 0;
};
