#include "StatisticsManagerServer.h"
#include "spdlog/spdlog-inl.h"
#include "BlockAnvil.h"
#include <fstream>
#include "StatBase.h"

std::shared_ptr<spdlog::logger> StatisticsManagerServer::LOGGER = spdlog::get("Minecraft")->clone("StatisticsManagerServer");

StatisticsManagerServer::StatisticsManagerServer(MinecraftServer* serverIn, std::string_view statsFileIn)
	:server(serverIn),statsFile(statsFileIn)
{
}

void StatisticsManagerServer::readStatFile()
{
   if (std::filesystem::is_regular_file(statsFile))
   {
      try 
      {
         statsData.clear();
         std::ifstream file(statsFile);
         statsData.put(parseJson(FileUtils.readFileToString(statsFile)));
      }
      catch (IOException var2) 
      {
         LOGGER->error("Couldn't read statistics file {}", statsFile, var2);
      } catch (JsonParseException var3) 
      {
         LOGGER->error("Couldn't parse statistics file {}", statsFile, var3);
      }
   }
}

void StatisticsManagerServer::saveStatFile()
{
   try 
   {
      std::ofstream file(statsFile);
      FileUtils.writeStringToFile(statsFile, dumpJson(statsData));
   }
   catch (IOException var2) 
   {
      LOGGER.error("Couldn't save stats", var2);
   }
}

void StatisticsManagerServer::unlockAchievement(EntityPlayer* playerIn, StatBase statIn, int32_t p_150873_3_)
{
   StatisticsManager::unlockAchievement(playerIn, statIn, p_150873_3_);
   dirty.insert(statIn);
}

std::set<StatBase> StatisticsManagerServer::getDirty()
{
   auto set = dirty;
   dirty.clear();
   return set;
}

void StatisticsManagerServer::markAllDirty()
{
   dirty.addAll(statsData.keySet());
}

void StatisticsManagerServer::sendStats(EntityPlayerMP* player)
{
   auto i = server->getTickCounter();
   std::unordered_map<StatBase,int32_t> map;
   if (i - lastStatRequest > 300) 
   {
      lastStatRequest = i;
      Iterator var4 = getDirty().iterator();

      while(var4.hasNext()) 
      {
         StatBase statbase = (StatBase)var4.next();
         map.insert(statbase, readStat(statbase));
      }
   }

   player->connection.sendPacket(new SPacketStatistics(map));
}
