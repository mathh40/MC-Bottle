#pragma once
#include "TextComponentBase.h"


class TextComponentSelector :public  TextComponentBase
{
public:
	TextComponentSelector(std::string selectorIn);
	std::string getSelector();
	std::string getUnformattedText() const override;
	std::string toString() const override;
private:
	std::string selector;
};
