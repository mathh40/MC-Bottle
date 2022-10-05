#pragma once
#include "IScoreCriteria.h"
#include "Scoreboard.h"

class ScoreObjective
{
  public:
    ScoreObjective(Scoreboard *scoreboard, std::string_view, IScoreCriteria *objectiveCriteriaIn);
    Scoreboard *getScoreboard() const;
    std::string getName() const;
    IScoreCriteria *getCriteria() const;
    std::string getDisplayName() const;
    void setDisplayName(std::string_view nameIn);
    IScoreCriteria::EnumRenderType getRenderType() const;
    void setRenderType(IScoreCriteria::EnumRenderType type);

  private:
    Scoreboard *scoreboard;
    std::string name;
    IScoreCriteria *objectiveCriteria;
    IScoreCriteria::EnumRenderType renderType;
    std::string displayName;
};
