#include "LanguageMap.h"
#include "../../../../../../compile-time-regular-expressions/single-header/ctre.hpp"
#include "Util.h"
#include <StringUtils.h>
#include <fstream>

static constexpr auto NUMERIC_VARIABLE_PATTERN = ctll::fixed_string{ R"(%(\d+\$)?[\d\.]*[df])"};
std::shared_ptr<LanguageMap> LanguageMap::instance = std::make_shared<LanguageMap>();


LanguageMap::LanguageMap()
{
	std::ifstream inputstream("/assets/minecraft/lang/en_us.lang");
	std::string line;
	while (getline(inputstream, line)) {
		if(!line.empty() && line[0] != '#')
		{
			auto s1 = line.substr(0, line.find_first_of('='));
			auto s2 = line.substr(line.find_first_of('=') + 1);


			auto sm = ctre::search<NUMERIC_VARIABLE_PATTERN>(s2);
            auto replace = sm.to_string();
            StringUtils::replaceFirstOccurrence(replace,s2,"%$1s");
			languageList.emplace(s1, sm);
		}
	}


	lastUpdateTimeInMilliseconds = Util::getcurrent_time();

}

std::string LanguageMap::translateKey(std::string key)
{
	std::lock_guard<std::mutex> lockGuard(m);
	return tryTranslateKey(key);
}

bool LanguageMap::isKeyTranslated(std::string key)
{
	std::lock_guard<std::mutex> lockGuard(m);
	auto s = languageList.find(key);
	return s != languageList.end();
}

std::shared_ptr<LanguageMap> LanguageMap::getInstance()
{
	return instance;
}

int64_t LanguageMap::getLastUpdateTimeInMilliseconds() const
{
	return lastUpdateTimeInMilliseconds;
}

std::string LanguageMap::tryTranslateKey(std::string key)
{
	auto s = languageList.find(key);
	return s != languageList.end() ? key : s->second;
}

void LanguageMap::replace_with(langmap p_135063_0_)
{
	std::lock_guard<std::mutex> lockGuard(m);
	languageList = p_135063_0_;
	lastUpdateTimeInMilliseconds = Util::getcurrent_time();

}
