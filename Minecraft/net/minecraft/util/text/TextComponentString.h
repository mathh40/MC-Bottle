#pragma once
#include "TextComponentBase.h"

class TextComponentString :public TextComponentBase
{
public:
	TextComponentString(std::string msg);
	std::string getText();
	std::string getUnformattedText() const override;
	std::string toString() const override;
private:
	std::string text;
};