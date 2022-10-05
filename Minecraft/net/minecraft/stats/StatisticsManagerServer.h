#pragma once

#include "StatisticsManager.h"
#include "spdlog/logger.h"

#include <filesystem>
#include <set>

class EntityPlayerMP;
class MinecraftServer;
class StatisticsManagerServer : public StatisticsManager
{
  public:
    StatisticsManagerServer(MinecraftServer *serverIn, std::string_view statsFileIn);
    void readStatFile();
    void saveStatFile();
    void unlockAchievement(EntityPlayer *playerIn, StatBase statIn, int32_t p_150873_3_) override;
    std::set<StatBase> getDirty();
    std::unordered_map<> parseJson(std::string_view p_150881_1_);
    std::string dumpJson(std::unordered_map<> &p_150880_0_);
    void markAllDirty();
    void sendStats(EntityPlayerMP *player);

  private:
    static std::shared_ptr<spdlog::logger> LOGGER;
    MinecraftServer *server;
    std::filesystem::path statsFile;
    std::set<StatBase> dirty;
    int32_t lastStatRequest = -300;
};
