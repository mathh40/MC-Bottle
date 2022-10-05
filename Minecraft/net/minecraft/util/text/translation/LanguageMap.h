#pragma once
#include <mutex>
#include <spdlog/fmt/fmt.h>
#include <unordered_map>

class LanguageMap
{
  public:
    using langmap = std::unordered_map<std::string, std::string>;
    LanguageMap();
    void replace_with(langmap p_135063_0_);
    std::string translateKey(std::string key);

    template <typename... Args> std::string translateKeyFormat(std::string key, Args... formats);
    bool isKeyTranslated(std::string key);

    static std::shared_ptr<LanguageMap> getInstance();

    int64_t getLastUpdateTimeInMilliseconds() const;

  private:
    langmap languageList;
    int64_t lastUpdateTimeInMilliseconds;
    std::mutex m;
    static std::shared_ptr<LanguageMap> instance;
    std::string tryTranslateKey(std::string key);
};

template <typename... Args> std::string LanguageMap::translateKeyFormat(std::string key, Args... formats)
{
    std::unique_lock<std::mutex> lock(m);
    auto s = tryTranslateKey(key);
    return format_to(s, std::forward<Args>(formats));
}
