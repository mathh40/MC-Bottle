#pragma once
#include "ISnooperInfo.h"
#include "Timer.h"
#include "crossguid/guid.hpp"

#include <unordered_map>

class Snooper
{
  public:
    Snooper(std::string_view side, ISnooperInfo *playerStatCollector, int64_t startTime);
    void startSnooper();

  private:
    void addOSData();
    void addJvmArgsToSnooper();

    std::unordered_map<> snooperStats;
    std::unordered_map<> clientStats;
    std::string uniqueID = xg::newGuid().str();
    URL serverUrl;
    ISnooperInfo *playerStatsCollector;
    Timer threadTrigger = Timer("Snooper Timer", true);
    Object syncLock     = new Object();
    int64_t minecraftStartTimeMilis;
    bool isRunning;
    int32_t selfCounter;
};
