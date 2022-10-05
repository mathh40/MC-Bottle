#include "I18n.h"

std::shared_ptr<LanguageMap> localizedName = LanguageMap::getInstance();

std::string I18n::translateToLocal(std::string key)
{
    return localizedName->translateKey(key);
}

std::string I18n::translateToFallback(std::string key)
{
    return fallbackTranslator.translateKey(key);
}

bool I18n::canTranslate(std::string key)
{
    return localizedName->isKeyTranslated(key);
}

int64_t I18n::getLastTranslationUpdateTimeInMilliseconds()
{
    return localizedName->getLastUpdateTimeInMilliseconds();
}
