#include "Snooper.h"

#include "../scoreboard/ServerScoreboard.h"
#include "HttpUtil.h"

Snooper::Snooper(std::string_view side, ISnooperInfo *playerStatCollector, int64_t startTime)
{
  try {
    serverUrl = new URL("http://snoop.minecraft.net/" + side + "?version=" + 2);
  } catch (std::exception& e) {
    throw std::runtime_error(e.what());
  }

  playerStatsCollector = playerStatCollector;
  minecraftStartTimeMilis = startTime;
}

void Snooper::startSnooper()
{
  if (!isRunning) {
    isRunning = true;
    addOSData();
    threadTrigger.schedule(new TimerTask(){
      void run() {
        if (Snooper::playerStatsCollector.isSnooperEnabled()) {
          HashMap map;
          synchronized(Snooper::syncLock) {
            map = clientStats;
            if (Snooper::selfCounter == 0) {
              map.putAll(Snooper::snooperStats);
            }

            map.put("snooper_count", Snooper::selfCounter++);
            map.put("snooper_token", Snooper::uniqueID);
          }

          MinecraftServer minecraftserver = Snooper::playerStatsCollector instanceof MinecraftServer ? (MinecraftServer)Snooper::playerStatsCollector : nullptr;
          HttpUtil::postMap(Snooper::serverUrl, map, true, minecraftserver == nullptr ? nullptr : minecraftserver.getServerProxy());
        }
      }
    },
    0L, 900000L);
  }
}

void Snooper::addOSData()
{
  addJvmArgsToSnooper();
  addClientStat("snooper_token", uniqueID);
  addStatToSnooper("snooper_token", uniqueID);
  addStatToSnooper("os_name", System.getProperty("os.name"));
  addStatToSnooper("os_version", System.getProperty("os.version"));
  addStatToSnooper("os_architecture", System.getProperty("os.arch"));
  addStatToSnooper("java_version", System.getProperty("java.version"));
  addClientStat("version", "1.12.2");
  playerStatsCollector->addServerTypeToSnooper(this);
}

void Snooper::addJvmArgsToSnooper()
{
  RuntimeMXBean runtimemxbean = ManagementFactory.getRuntimeMXBean();
      List list = runtimemxbean.getInputArguments();
      int i = 0;
      Iterator var4 = list.iterator();

      while(var4.hasNext()) {
         String s = (String)var4.next();
         if (s.startsWith("-X")) {
            this.addClientStat("jvm_arg[" + i++ + "]", s);
         }
      }

      this.addClientStat("jvm_args", i);
}
