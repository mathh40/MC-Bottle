#pragma once
#include "ScoreCriteria.h"

class ScoreCriteriaReadOnly : public ScoreCriteria
{
  public:
    ScoreCriteriaReadOnly(std::string_view name);
    bool isReadOnly() override;

  private:
};
