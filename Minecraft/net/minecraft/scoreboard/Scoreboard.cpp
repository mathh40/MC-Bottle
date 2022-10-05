#include "Scoreboard.h"

#include "../world/gen/MapGenBase.h"
#include "Score.h"
#include "spdlog/sinks/null_sink.h"
#include "text/TextFormatting.h"

#include <stdexcept>

ScoreObjective Scoreboard::getObjective(std::string_view name)
{
    return scoreObjectives.at(name);
}

ScoreObjective Scoreboard::addScoreObjective(std::string_view name, IScoreCriteria *criteria)
{
    if (name.length() > 16)
    {
        throw std::logic_error("The objective name '" + name + "' is too long!");
    }
    else
    {
        ScoreObjective scoreobjective = getObjective(name);
        if (scoreobjective != nullptr)
        {
            throw std::logic_error("An objective with the name '" + name + "' already exists!");
        }
        else
        {
            scoreobjective = ScoreObjective(this, name, criteria);
            auto list      = scoreObjectiveCriterias.at(criteria);
            if (list.empty())
            {
                scoreObjectiveCriterias.emplace(criteria, list);
            }

            list.emplace_back(scoreobjective);
            scoreObjectives.emplace(name, scoreobjective);
            onScoreObjectiveAdded(scoreobjective);
            return scoreobjective;
        }
    }
}

std::vector<ScoreObjective> Scoreboard::getObjectivesFromCriteria(IScoreCriteria *criteria)
{
    auto collection = scoreObjectiveCriterias.at(criteria);
    return collection.empty() ? std::vector<ScoreObjective>() : collection;
}

bool Scoreboard::entityHasObjective(std::string_view name, ScoreObjective objective)
{
    auto map = entitiesScoreObjectives.at(name);
    if (map.empty())
    {
        return false;
    }
    auto score = map.at(objective);
    return score != nullptr;
}

Score Scoreboard::getOrCreateScore(std::string_view username, ScoreObjective objective)
{
    if (username.length() > 40)
    {
        throw std::logic_error("The player name '" + username + "' is too long!");
    }
    else
    {
        auto map = entitiesScoreObjectives.at(username);
        if (map == nullptr)
        {
            entitiesScoreObjectives.emplace(username, map);
        }

        auto score = map.at(objective);
        if (score == nullptr)
        {
            score = new Score(this, objective, username);
            map.emplace(objective, score);
        }

        return score;
    }
}

std::vector<Score *> Scoreboard::getSortedScores(ScoreObjective objective)
{
    std::vector<Score *> list;
    Iterator var3 = entitiesScoreObjectives.values().iterator();

    while (var3.hasNext())
    {
        Map map     = var3.next();
        Score score = map.get(objective);
        if (score != nullptr)
        {
            list.add(score);
        }
    }

    Collections.sort(list, Score::SCORE_COMPARATOR);
    return list;
}

std::vector<ScoreObjective> Scoreboard::getScoreObjectives()
{
    return scoreObjectives.values();
}

std::vector<std::string> Scoreboard::getObjectiveNames()
{
    return entitiesScoreObjectives.keySet();
}

void Scoreboard::removeObjectiveFromEntity(std::string_view name, ScoreObjective objective)
{
    Map map2;
    if (objective == nullptr)
    {
        map2 = entitiesScoreObjectives.remove(name);
        if (map2 != nullptr)
        {
            broadcastScoreUpdate(name);
        }
    }
    else
    {
        map2 = entitiesScoreObjectives.at(name);
        if (map2 != nullptr)
        {
            auto score = map2.remove(objective);
            if (map2.size() < 1)
            {
                auto map1 = entitiesScoreObjectives.remove(name);
                if (map1 != nullptr)
                {
                    broadcastScoreUpdate(name);
                }
            }
            else if (score != nullptr)
            {
                broadcastScoreUpdate(name, objective);
            }
        }
    }
}

Collection Scoreboard::getScores()
{
    Collection collection = entitiesScoreObjectives.values();
    List list             = Lists.newArrayList();
    Iterator var3         = collection.iterator();

    while (var3.hasNext())
    {
        Map map = (Map)var3.next();
        list.addAll(map.values());
    }

    return list;
}

std::unordered_map<std::string, std::unordered_map<ScoreObjective, Score *>> Scoreboard::getObjectivesForEntity(
    std::string_view name)
{
    auto map = entitiesScoreObjectives.at(name);
    if (map == nullptr)
    {
        map = std::unordered_map<std::string, std::unordered_map<ScoreObjective, Score *>>();
    }

    return map;
}

void Scoreboard::removeObjective(ScoreObjective objective)
{
    scoreObjectives.remove(objective.getName());

    for (int i = 0; i < 19; ++i)
    {
        if (getObjectiveInDisplaySlot(i) == objective)
        {
            setObjectiveInDisplaySlot(i, nullptr);
        }
    }

    List list = scoreObjectiveCriterias.get(objective.getCriteria());
    if (list != nullptr)
    {
        list.remove(objective);
    }

    Iterator var3 = entitiesScoreObjectives.values().iterator();

    while (var3.hasNext())
    {
        Map map = (Map)var3.next();
        map.remove(objective);
    }

    onScoreObjectiveRemoved(objective);
}

void Scoreboard::setObjectiveInDisplaySlot(int32_t objectiveSlot, ScoreObjective objective)
{
    objectiveDisplaySlots[objectiveSlot] = objective;
}

ScoreObjective Scoreboard::getObjectiveInDisplaySlot(int32_t slotIn)
{
    return objectiveDisplaySlots[slotIn];
}

ScorePlayerTeam Scoreboard::getTeam(std::string_view teamName)
{
    return teams.at(teamName);
}

ScorePlayerTeam Scoreboard::createTeam(std::string_view name)
{
    if (name.length() > 16)
    {
        throw new IllegalArgumentException("The team name '" + name + "' is too long!");
    }
    else
    {
        ScorePlayerTeam scoreplayerteam = getTeam(name);
        if (scoreplayerteam != nullptr)
        {
            throw std::logic_error("A team with the name '" + name + "' already exists!");
        }
        else
        {
            scoreplayerteam = ScorePlayerTeam(this, name);
            teams.put(name, scoreplayerteam);
            broadcastTeamCreated(scoreplayerteam);
            return scoreplayerteam;
        }
    }
}

void Scoreboard::removeTeam(ScorePlayerTeam playerTeam)
{
    teams.remove(playerTeam.getName());
    Iterator var2 = playerTeam.getMembershipCollection().iterator();

    while (var2.hasNext())
    {
        String s = var2.next();
        teamMemberships.remove(s);
    }

    broadcastTeamRemove(playerTeam);
}

bool Scoreboard::addPlayerToTeam(std::string_view player, std::string_view newTeam)
{
    if (player.length() > 40)
    {
        throw new std::logic_error("The player name '" + player + "' is too long!");
    }
    else if (teams.find(newTeam) == teams.end())
    {
        return false;
    }
    else
    {
        ScorePlayerTeam scoreplayerteam = getTeam(newTeam);
        if (getPlayersTeam(player) != nullptr)
        {
            removePlayerFromTeams(player);
        }

        teamMemberships.put(player, scoreplayerteam);
        scoreplayerteam.getMembershipCollection().add(player);
        return true;
    }
}

bool Scoreboard::removePlayerFromTeams(std::string_view playerName)
{
    ScorePlayerTeam scoreplayerteam = getPlayersTeam(playerName);
    if (scoreplayerteam != nullptr)
    {
        removePlayerFromTeam(playerName, scoreplayerteam);
        return true;
    }
    else
    {
        return false;
    }
}

void Scoreboard::removePlayerFromTeam(std::string_view username, ScorePlayerTeam playerTeam)
{
    if (getPlayersTeam(username) != playerTeam)
    {
        throw std::logic_error("Player is either on another team or not on any team. Cannot remove from team '" +
                               playerTeam.getName() + "'.");
    }
    else
    {
        teamMemberships.remove(username);
        playerTeam.getMembershipCollection().remove(username);
    }
}

std::vector<std::string> Scoreboard::getTeamNames()
{
    std::vector<std::string> collect;
    for (auto key : teams)
    {
        collect.emplace_back(key.first());
    }
    return collect;
}

std::vector<ScorePlayerTeam> Scoreboard::getTeams()
{
    std::vector<ScorePlayerTeam> collect;
    for (auto key : teams)
    {
        collect.emplace_back(key.second());
    }
    return collect;
}

ScorePlayerTeam Scoreboard::getPlayersTeam(std::string_view username)
{
    return teamMemberships.get(username);
}

std::string Scoreboard::getObjectiveDisplaySlot(int32_t id)
{
    switch (id)
    {
    case 0:
        return "list";
    case 1:
        return "sidebar";
    case 2:
        return "belowName";
    default:
        if (id >= 3 && id <= 18)
        {
            auto textformatting = TextFormatting::fromColorIndex(id - 3);
            if (textformatting.has_value() && textformatting != TextFormatting::RESET)
            {
                return "sidebar.team." + textformatting.value().getFriendlyName();
            }
        }

        return "";
    }
}

int32_t Scoreboard::getObjectiveDisplaySlotNumber(std::string_view name)
{
    if ("list" == name)
    {
        return 0;
    }
    else if ("sidebar" == name)
    {
        return 1;
    }
    else if ("belowName" == name)
    {
        return 2;
    }
    else
    {
        std::string temp = std::string("sidebar.team.");
        if (name.rfind(temp, 0) == 0)
        {
            auto s              = name.substr(temp.size());
            auto textformatting = TextFormatting::getValueByName(std::string(s));
            if (textformatting.has_value() && textformatting.value().getColorIndex() >= 0)
            {
                return textformatting.value().getColorIndex() + 3;
            }
        }

        return -1;
    }
}

std::vector<std::string> Scoreboard::getDisplaySlotStrings()
{
    if (displaySlots.empty())
    {
        displaySlots.resize(19);

        for (int32_t i = 0; i < 19; ++i)
        {
            displaySlots[i] = getObjectiveDisplaySlot(i);
        }
    }

    return displaySlots;
}

void Scoreboard::removeEntity(Entity *entityIn)
{
    if (entityIn != nullptr && !(Util:: instanceof <EntityPlayer>(entityIn)) && !entityIn->isEntityAlive())
    {
        auto s = entityIn->getCachedUniqueIdString();
        removeObjectiveFromEntity(s, (ScoreObjective)null);
        removePlayerFromTeams(s);
    }
}
