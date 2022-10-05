#pragma once
#include "IProgressUpdate.h"

#include <atomic>
#include <filesystem>
#include <future>
#include <memory>
#include <optional>
#include <spdlog/async_logger.h>

class HttpUtil
{
  public:
    HttpUtil();
    static std::string buildPostString(std::unordered_map<std::string, std::string> data);
    std::future<void> downloadResourcePack(std::filesystem::path saveFile, std::string packUrl,
                                           std::unordered_map<std::string, std::string> p_180192_2_, int maxSize,
                                           std::optional<IProgressUpdate> p_180192_4_);
    static int getSuitableLanPort();

  private:
    static std::atomic_int DOWNLOAD_THREADS_STARTED;
    static std::shared_ptr<spdlog::async_logger> LOGGER;
};
