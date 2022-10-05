#pragma once
#include "ScoreCriteria.h"

class ScoreCriteriaHealth : public ScoreCriteria
{
  public:
    ScoreCriteriaHealth(std::string_view name);
    bool isReadOnly() override;
    IScoreCriteria::EnumRenderType getRenderType() override;

  private:
};
