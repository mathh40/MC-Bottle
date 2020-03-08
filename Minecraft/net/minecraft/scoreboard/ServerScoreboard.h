#pragma once
#include <unordered_set>

#include "../stats/StatisticsManagerServer.h"
#include "Scoreboard.h"

class ServerScoreboard :public Scoreboard
{
public:
  ServerScoreboard(MinecraftServer* mcServer);
  void onScoreUpdated(Score scoreIn);
  void broadcastScoreUpdate(std::string_view scoreName);
  void broadcastScoreUpdate(std::string_view scoreName, ScoreObjective objective);
  void setObjectiveInDisplaySlot(int32_t objectiveSlot, ScoreObjective objective);
  bool addPlayerToTeam(std::string_view  player, std::string_view  newTeam);
  void removePlayerFromTeam(std::string_view username, ScorePlayerTeam playerTeam);
  void onScoreObjectiveAdded(ScoreObjective scoreObjectiveIn);
  void onObjectiveDisplayNameChanged(ScoreObjective objective);
  void onScoreObjectiveRemoved(ScoreObjective objective);
  void broadcastTeamCreated(ScorePlayerTeam playerTeam);
  void broadcastTeamInfoUpdate(ScorePlayerTeam playerTeam);
  void broadcastTeamRemove(ScorePlayerTeam playerTeam);
  void addDirtyRunnable(Runnable runnable);
  std::vector<> getCreatePackets(ScoreObjective objective);
  void addObjective(ScoreObjective objective);
  std::vector<> getDestroyPackets(ScoreObjective p_96548_1_);
  void sendDisplaySlotRemovalPackets(ScoreObjective p_96546_1_);
  int32_t getObjectiveDisplaySlotCount(ScoreObjective p_96552_1_);



protected:
  void markSaveDataDirty();

private:
  MinecraftServer* server;
  std::unordered_set<ScoreObjective> addedObjectives;
  std::vector<Runnable> dirtyRunnables;
};
