#pragma once
#include "TextComponentBase.h"
#include <any>

class TextComponentTranslation : public TextComponentBase {
public:
    template <class... Args>
  TextComponentTranslation(std::string_view translationKey, Args... args) : key(translationKey) : formatArgs{args ...}{}


protected:
    void initializeFromFormat(std::string_view format);

private:
    ITextComponent *getFormatArgumentAsComponent(uint64_t index);

    std::vector<std::any *> formatArgs;
	std::string key;
	long lastTranslationUpdateTimeInMilliseconds = -1L;
};