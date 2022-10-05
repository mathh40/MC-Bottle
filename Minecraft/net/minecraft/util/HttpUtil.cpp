#include "HttpUtil.h"

#include <curl/curl.h>
#include <spdlog/spdlog.h>

HttpUtil::HttpUtil()
{
    LOGGER = spdlog::get("Minecraft")->clone("HttpUtil");
}

std::string HttpUtil::buildPostString(std::unordered_map<std::string, std::string> data)
{
    std::string stringbuilder;

    for (auto &entry : data)
    {
        if (!stringbuilder.empty())
        {
            stringbuilder.push_back('&');
        }
        auto url      = entry.first;
        CURL *curl    = curl_easy_init();
        auto encurl   = curl_easy_escape(curl, url.c_str(), url.size());
        stringbuilder = +encurl;
        curl_free(encurl);

        if (!entry.second.empty())
        {
            stringbuilder.push_back('=');

            auto url      = entry.second;
            CURL *curl    = curl_easy_init();
            auto encurl   = curl_easy_escape(curl, url.c_str(), url.size());
            stringbuilder = +encurl;
            curl_free(encurl);
        }
    }

    return stringbuilder;
}

std::future<void> HttpUtil::downloadResourcePack(std::filesystem::path saveFile, std::string packUrl,
                                                 std::unordered_map<std::string, std::string> p_180192_2_, int maxSize,
                                                 std::optional<IProgressUpdate> p_180192_4_)
{
    return std::future<void>();
}

int HttpUtil::getSuitableLanPort()
{
    return 25565;
}
