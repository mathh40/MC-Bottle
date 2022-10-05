#include "../world/storage/WorldSavedData.h"
#include "Scoreboard.h"
#include "spdlog/logger.h"

class NBTTagList;

class ScoreboardSaveData : public WorldSavedData
{
  public:
    ScoreboardSaveData();
    explicit ScoreboardSaveData(std::string_view name);
    void setScoreboard(Scoreboard *scoreboardIn);
    void readFromNBT(NBTTagCompound *nbt) override;
    NBTTagCompound *writeToNBT(NBTTagCompound *compound) override;

  protected:
    void readTeams(NBTTagList *tagList);
    void loadTeamPlayers(ScorePlayerTeam playerTeam, NBTTagList *tagList);
    void readDisplayConfig(NBTTagCompound *compound);
    void readObjectives(NBTTagList *nbt);
    void readScores(NBTTagList *nbt);
    NBTTagList *teamsToNbt();
    void fillInDisplaySlots(NBTTagCompound *compound);
    NBTTagList *objectivesToNbt();
    NBTTagList *scoresToNbt();

  private:
    static std::shared_ptr<spdlog::logger> LOGGER;
    Scoreboard *scoreboard;
    std::shared_ptr<NBTTagCompound> delayedInitNbt;
};
