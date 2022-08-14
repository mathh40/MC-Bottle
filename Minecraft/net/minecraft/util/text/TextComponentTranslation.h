#pragma once
#include "TextComponentBase.h"
#include <any>

class ITextComponent;

class TextComponentTranslation :public TextComponentBase
{
public:
    TextComponentTranslation(std::string_view translationKey, std::initializer_list<std::any> args);

protected:
    void initializeFromFormat(std::string_view format);

private:
    ITextComponent *getFormatArgumentAsComponent(uint64_t index);

    std::vector<std::any> formatArgs;
	std::string key;
	long lastTranslationUpdateTimeInMilliseconds = -1L;
};