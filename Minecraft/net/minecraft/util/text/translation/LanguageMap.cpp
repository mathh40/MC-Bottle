#include "LanguageMap.h"
#include <fstream>
#include "Util.h"

std::regex LanguageMap::NUMERIC_VARIABLE_PATTERN(R"(%(\d+\$)?[\d\.]*[df])");
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
			auto sm = std::regex_replace(s2, NUMERIC_VARIABLE_PATTERN, "%$1s");
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
