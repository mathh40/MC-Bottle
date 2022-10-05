#pragma once
#include "Score.h"

#include <array>
#include <unordered_map>

class ScoreObjective;
class ScorePlayerTeam;
class Entity;
class IScoreCriteria;

class Scoreboard
{
  public:
    ScoreObjective getObjective(std::string_view name);
    ScoreObjective addScoreObjective(std::string_view name, IScoreCriteria *criteria);
    std::vector<ScoreObjective> getObjectivesFromCriteria(IScoreCriteria *criteria);
    bool entityHasObjective(std::string_view name, ScoreObjective objective);
    Score getOrCreateScore(std::string_view username, ScoreObjective objective);
    std::vector<Score *> getSortedScores(ScoreObjective objective);
    std::vector<ScoreObjective> getScoreObjectives();
    std::vector<std::string> getObjectiveNames();
    void removeObjectiveFromEntity(std::string_view name, ScoreObjective objective);
    Collection getScores();
    std::unordered_map<std::string, std::unordered_map<ScoreObjective, Score *>> getObjectivesForEntity(
        std::string_view name);
    void removeObjective(ScoreObjective objective);
    void setObjectiveInDisplaySlot(int32_t objectiveSlot, ScoreObjective objective);
    ScoreObjective getObjectiveInDisplaySlot(int32_t slotIn);
    ScorePlayerTeam getTeam(std::string_view teamName);
    ScorePlayerTeam createTeam(std::string_view name);
    void removeTeam(ScorePlayerTeam *playerTeam);
    bool addPlayerToTeam(std::string_view player, std::string_view newTeam);
    void removePlayerFromTeam(std::string_view username, ScorePlayerTeam *playerTeam);
    std::vector<std::string> getTeamNames();
    std::vector<ScorePlayerTeam> getTeams();
    ScorePlayerTeam getPlayersTeam(std::string_view username);
    void onScoreObjectiveAdded(ScoreObjective scoreObjectiveIn);
    void onObjectiveDisplayNameChanged(ScoreObjective objective);
    void onScoreObjectiveRemoved(ScoreObjective objective);
    void onScoreUpdated(Score scoreIn);
    void broadcastScoreUpdate(std::string_view scoreName);
    void broadcastScoreUpdate(std::string_view scoreName, ScoreObjective objective);
    void broadcastTeamCreated(ScorePlayerTeam *playerTeam);
    void broadcastTeamInfoUpdate(ScorePlayerTeam *playerTeam);
    void broadcastTeamRemove(ScorePlayerTeam *playerTeam);
    static std::string getObjectiveDisplaySlot(int32_t id);
    static int32_t getObjectiveDisplaySlotNumber(std::string_view name);
    static std::vector<std::string> getDisplaySlotStrings();
    void removeEntity(Entity *entityIn);

  private:
    std::unordered_map<std::string, ScoreObjective> scoreObjectives;
    std::unordered_map<IScoreCriteria *, std::vector<ScoreObjective>> scoreObjectiveCriterias;
    std::unordered_map<std::string, std::unordered_map<ScoreObjective, Score *>> entitiesScoreObjectives;
    std::array<ScoreObjective, 19> objectiveDisplaySlots;
    std::unordered_map<std::string, ScorePlayerTeam> teams;
    std::unordered_map<std::string, ScorePlayerTeam> teamMemberships;
    static std::vector<std::string> displaySlots;
};
