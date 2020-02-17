#pragma once
#include "LanguageMap.h"

class I18n
{
public:
	static std::string translateToLocal(std::string key);

	template<typename... Args>
	static std::string translateToLocalFormatted(std::string key, Args... format);
	static std::string translateToFallback(std::string key);
	static bool canTranslate(std::string key);
	static int64_t getLastTranslationUpdateTimeInMilliseconds();
private:
	static std::shared_ptr<LanguageMap> localizedName;
	static LanguageMap fallbackTranslator;
};

template <typename ... Args>
std::string I18n::translateToLocalFormatted(std::string key, Args... format)
{
	return localizedName->translateKeyFormat(key, format);
}
