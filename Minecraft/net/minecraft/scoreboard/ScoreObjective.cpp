#include "ScoreObjective.h"

ScoreObjective::ScoreObjective(Scoreboard* scoreboard, std::string_view nameIn, IScoreCriteria* objectiveCriteriaIn)
   :scoreboard(scoreboard),name(nameIn),objectiveCriteria(objectiveCriteriaIn),displayName(nameIn),renderType(objectiveCriteriaIn->getRenderType())
{
}

Scoreboard* ScoreObjective::getScoreboard() const
{
   return scoreboard;
}

std::string ScoreObjective::getName() const
{
   return name;
}

IScoreCriteria* ScoreObjective::getCriteria() const
{
   return objectiveCriteria;
}

std::string ScoreObjective::getDisplayName() const
{
   return displayName;
}

void ScoreObjective::setDisplayName(std::string_view nameIn)
{
   displayName = nameIn;
   scoreboard->onObjectiveDisplayNameChanged(*this);
}

IScoreCriteria::EnumRenderType ScoreObjective::getRenderType() const
{
   return renderType;
}

void ScoreObjective::setRenderType(IScoreCriteria::EnumRenderType type)
{
   renderType = type;
   scoreboard->onObjectiveDisplayNameChanged(*this);
}
