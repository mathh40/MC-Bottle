#include <optional>
#include <string>
#include <vector>

class CommandResultStatsType;
class NBTTagCompound;
class MinecraftServer;
class ICommandSender;

class CommandResultStats
{
  public:
    CommandResultStats();
    void setCommandStatForSender(MinecraftServer *server, ICommandSender *sender, const CommandResultStatsType &typeIn,
                                 int32_t p_184932_4_);
    void readStatsFromNBT(NBTTagCompound *tagcompound);
    void writeStatsToNBT(NBTTagCompound *tagcompound);
    static void setScoreBoardStat(CommandResultStats stats, const CommandResultStatsType &resultType,
                                  std::optional<std::string_view> entityID,
                                  std::optional<std::string_view> objectiveName);
    void addAllStats(CommandResultStats resultStatsIn);

  private:
    static void removeScoreBoardStat(CommandResultStats resultStatsIn, const CommandResultStatsType &resultTypeIn);

    static int32_t NUM_RESULT_TYPES = CommandResultStatsType::values().size();
    static std::vector<std::string> STRING_RESULT_TYPES;
    std::vector<std::string> entitiesID;
    std::vector<std::string> objectives;
};
