#pragma once
#include "TextComponentBase.h"
#include <regex>

class TextComponentTranslation :public TextComponentBase
{
public:
private:
	std::string key;
	long lastTranslationUpdateTimeInMilliseconds = -1L;
};